#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/aruco.hpp> 
#include <opencv2/video/tracking.hpp>
#include <opencv2/calib3d.hpp> // Necesario para Rodrigues y solvePnP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/image_encodings.hpp>
#include <std_msgs/msg/bool.hpp>
#include <image_transport/image_transport.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <interfaces/msg/img_data.hpp>
#include "../include/img_proc_pkg/aruco_nano.h"

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2_ros/transform_broadcaster.h"

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;

// Variables globales
cv::Mat img_original;
cv::Mat cameraMatrix;
cv::Mat distCoeffs;
float markerSize=0.11;    
int image_width, image_height;
geometry_msgs::msg::TransformStamped saved_cam_tf; // Guardamos la TF de la camara
bool camera_located = false; 

class Aruco_Nano_Detector : public rclcpp::Node
{
  public:
    Aruco_Nano_Detector() : Node("aruco_nano_detector")
    {
      std::cout<<"Global Vision Node: Multi-Marker Floor Levelling Enabled"<<std::endl;
      
      // Parametros
      this->declare_parameter<std::string>("calibration_file", "/home/javierfr/Robotics_Cyber_Physical_LAB/src/img_proc_pkg/config/camera_calib_charuco.yaml");
      std::string calib_file = this->get_parameter("calibration_file").as_string();

      this->declare_parameter<std::string>("camera_topic", "/cameras/cam_1");
      this->declare_parameter<std::string>("camera_frame", "cam_1");
      this->declare_parameter<float>("marker_size", 0.0938); 

      camera_topic = this->get_parameter("camera_topic").as_string();
      camera_frame_id = this->get_parameter("camera_frame").as_string();
      markerSize = this->get_parameter("marker_size").as_double();

      // Redundancia
      this->declare_parameter<std::string>("master_camera_status_topic", "");
      std::string master_topics_str = this->get_parameter("master_camera_status_topic").as_string();

      // Publishers
      status_pub_ = this->create_publisher<std_msgs::msg::Bool>("~/status", 10);
      result_img_pub_ = this->create_publisher<sensor_msgs::msg::Image>("~/result_image", 10);

      // Suscriptores Maestros
      if (!master_topics_str.empty()) {
          std::stringstream ss(master_topics_str);
          std::string segment;
          while (std::getline(ss, segment, ',')) {
              segment.erase(0, segment.find_first_not_of(' '));
              segment.erase(segment.find_last_not_of(' ') + 1);
              if (segment.empty()) continue;
              auto topic = segment;
              auto sub = this->create_subscription<std_msgs::msg::Bool>(topic, 10, [this, topic](const std_msgs::msg::Bool::SharedPtr msg) {
                      this->master_status_map_[topic] = msg->data;
                      this->master_heartbeat_map_[topic] = this->now();
                  });
              master_subs_.push_back(sub);
              master_status_map_[topic] = false;
              master_heartbeat_map_[topic] = rclcpp::Time(0);
          }
      }

      // Calibracion
      cv::FileStorage fs(calib_file, cv::FileStorage::READ);
      if(!fs.isOpened()){
        RCLCPP_ERROR(this->get_logger(), "No calibration file.");
        cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        cameraMatrix.at<double>(0, 0) = 1000.0; cameraMatrix.at<double>(1, 1) = 1000.0;
        cameraMatrix.at<double>(0, 2) = 640.0;  cameraMatrix.at<double>(1, 2) = 360.0;
        distCoeffs = cv::Mat::zeros(1, 5, CV_64F);
      } else {
        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distCoeffs;
        fs.release();
      }
      
      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      camera_topic, 10, std::bind(&Aruco_Nano_Detector::topic_callback, this, _1));

      tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

      // Timeout Timer
      last_image_time_ = this->now();
      status_timeout_timer_ = this->create_wall_timer(
          500ms, [this]() {
              if ((this->now() - last_image_time_).seconds() > 2.0) {
                  std_msgs::msg::Bool status_msg; status_msg.data = false; status_pub_->publish(status_msg);
              }
          });

      // === DEFINICIÓN DEL MUNDO (Asegúrate que estas coordenadas sean precisas) ===
      fixed_markers[0] = cv::Point3f(0.0, 0.0, 0.0);
      fixed_markers[99] = cv::Point3f(-2.0, 1.0, 0.0);
      fixed_markers[98] = cv::Point3f(0.0, 1.0, 0.0);
      fixed_markers[97] = cv::Point3f(2.0, 1.0, 0.0);
      fixed_markers[96] = cv::Point3f(-2.0, 0.0, 0.0);
      fixed_markers[95] = cv::Point3f(2.0, 0.0, 0.0);
      fixed_markers[94] = cv::Point3f(-2.0, -1.0, 0.0);
      fixed_markers[93] = cv::Point3f(0.0, -1.0, 0.0);
      fixed_markers[92] = cv::Point3f(2.0, -1.0, 0.0);

      // Kalman Camara (Muy lento y estable)
      kf_cam = cv::KalmanFilter(7, 7, 0);
      cv::setIdentity(kf_cam.measurementMatrix);
      cv::setIdentity(kf_cam.processNoiseCov, cv::Scalar::all(1e-4)); // Confiar mucho en el modelo
      cv::setIdentity(kf_cam.measurementNoiseCov, cv::Scalar::all(0.1)); // Desconfiar de mediciones rapidas
      cv::setIdentity(kf_cam.errorCovPost, cv::Scalar::all(1));
    }

  private:

    cv::KalmanFilter kf_cam;
    cv::Mat state_cam;
    std::map<int, cv::Point3f> fixed_markers;
    std::map<int, cv::Point3f> last_dynamic_pos;
    
    std::string camera_topic;
    std::string camera_frame_id;

    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr status_pub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr result_img_pub_;
    std::vector<rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr> master_subs_;
    std::map<std::string, bool> master_status_map_;
    std::map<std::string, rclcpp::Time> master_heartbeat_map_;

    rclcpp::TimerBase::SharedPtr status_timeout_timer_;
    rclcpp::Time last_image_time_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;

    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    { 
      last_image_time_ = this->now();
      cv_bridge::CvImageConstPtr image_bridge;
      try{
        image_bridge=cv_bridge::toCvCopy(msg);
        img_original = image_bridge->image;
        if (image_bridge->image.empty()) return;

        // Deteccion
        auto markers = aruconano::MarkerDetector::detect(img_original);
        for(const auto &m:markers) m.draw(img_original);

        // Failover Check
        bool fixed_marker_visible = false;
        for(const auto &m:markers) { if(fixed_markers.count(m.id)) { fixed_marker_visible = true; break; } }
        
        std_msgs::msg::Bool status_msg; status_msg.data = fixed_marker_visible; status_pub_->publish(status_msg);

        bool any_master_healthy = false;
        if (!master_subs_.empty()) {
            for (auto const& [topic, is_alive] : master_status_map_) {
                double sec = (this->now() - master_heartbeat_map_[topic]).seconds();
                if (is_alive && sec < 2.0) { any_master_healthy = true; break; }
            }
            if (any_master_healthy) return; 
        }

        std_msgs::msg::Header header = msg->header; 
        cv_bridge::CvImage img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::BGR8, img_original);
        result_img_pub_->publish(*img_bridge.toImageMsg());

        // =================================================================================
        // PASO 1: LOCALIZACIÓN GLOBAL DE LA CÁMARA (SolvePnP con TODOS los fijos)
        // =================================================================================
        std::vector<cv::Point3f> objPoints;
        std::vector<cv::Point2f> imgPoints;

        for(const auto &m:markers){
            if(fixed_markers.count(m.id)){
                // Definir las 4 esquinas del marcador en el MUNDO 3D
                float s = markerSize / 2.0;
                cv::Point3f center = fixed_markers[m.id];
                // Orden esquinas ArUco: TL, TR, BR, BL
                objPoints.push_back(center + cv::Point3f(-s, s, 0));
                objPoints.push_back(center + cv::Point3f(s, s, 0));
                objPoints.push_back(center + cv::Point3f(s, -s, 0));
                objPoints.push_back(center + cv::Point3f(-s, -s, 0));

                // Agregar las 4 esquinas detectadas en IMAGEN 2D
                // "m" actúa como el vector de esquinas
                for(const auto &p : m) imgPoints.push_back(p);
            }
        }

        if(objPoints.size() >= 4) { // Necesitamos al menos 1 marcador (4 esquinas)
            cv::Mat rvec_cam, tvec_cam;
            // Calculamos dónde está la cámara respecto al mundo completo
            bool pnp_ok = cv::solvePnP(objPoints, imgPoints, cameraMatrix, distCoeffs, rvec_cam, tvec_cam);
            
            if(pnp_ok) {
                // Invertimos: de (Mundo->Camara) a (Camara->Mundo) para publicar TF
                cv::Mat rmat; cv::Rodrigues(rvec_cam, rmat);
                cv::Mat rmat_inv = rmat.t();
                cv::Mat tvec_inv = -rmat_inv * tvec_cam;

                tf2::Matrix3x3 mat(rmat_inv.at<double>(0,0), rmat_inv.at<double>(0,1), rmat_inv.at<double>(0,2),
                                   rmat_inv.at<double>(1,0), rmat_inv.at<double>(1,1), rmat_inv.at<double>(1,2),
                                   rmat_inv.at<double>(2,0), rmat_inv.at<double>(2,1), rmat_inv.at<double>(2,2));
                tf2::Quaternion qu; mat.getRotation(qu);

                // --- KALMAN FILTER PARA LA CAMARA ---
                if (state_cam.empty()) {
                    state_cam = cv::Mat::zeros(7, 1, CV_32F);
                    kf_cam.statePost.at<float>(0,0) = tvec_inv.at<double>(0);
                    kf_cam.statePost.at<float>(1,0) = tvec_inv.at<double>(1);
                    kf_cam.statePost.at<float>(2,0) = tvec_inv.at<double>(2);
                    kf_cam.statePost.at<float>(3,0) = qu.x();
                    kf_cam.statePost.at<float>(4,0) = qu.y();
                    kf_cam.statePost.at<float>(5,0) = qu.z();
                    kf_cam.statePost.at<float>(6,0) = qu.w();
                }

                // Filtrar saltos
                double dist = sqrt(pow(tvec_inv.at<double>(0) - kf_cam.statePost.at<float>(0,0), 2) +
                                   pow(tvec_inv.at<double>(1) - kf_cam.statePost.at<float>(1,0), 2) +
                                   pow(tvec_inv.at<double>(2) - kf_cam.statePost.at<float>(2,0), 2));
                
                if (dist > 0.2) kf_cam.predict(); // Salto grande, ignorar
                else {
                     cv::Mat meas(7, 1, CV_32F);
                     meas.at<float>(0,0) = tvec_inv.at<double>(0);
                     meas.at<float>(1,0) = tvec_inv.at<double>(1);
                     meas.at<float>(2,0) = tvec_inv.at<double>(2);
                     meas.at<float>(3,0) = qu.x(); meas.at<float>(4,0) = qu.y();
                     meas.at<float>(5,0) = qu.z(); meas.at<float>(6,0) = qu.w();
                     kf_cam.predict();
                     kf_cam.correct(meas);
                }

                saved_cam_tf.header.stamp = msg->header.stamp;
                saved_cam_tf.header.frame_id = "marker_id_00";
                saved_cam_tf.child_frame_id = camera_frame_id;
                saved_cam_tf.transform.translation.x = kf_cam.statePost.at<float>(0,0);
                saved_cam_tf.transform.translation.y = kf_cam.statePost.at<float>(1,0);
                saved_cam_tf.transform.translation.z = kf_cam.statePost.at<float>(2,0);
                saved_cam_tf.transform.rotation.x = kf_cam.statePost.at<float>(3,0);
                saved_cam_tf.transform.rotation.y = kf_cam.statePost.at<float>(4,0);
                saved_cam_tf.transform.rotation.z = kf_cam.statePost.at<float>(5,0);
                saved_cam_tf.transform.rotation.w = kf_cam.statePost.at<float>(6,0);
                
                camera_located = true;
            }
        }

        if (camera_located) {
            tf_broadcaster->sendTransform(saved_cam_tf);
        } else {
            return; // Sin camara no podemos ubicar robots
        }

        // =================================================================================
        // PASO 2: UBICAR OBJETOS DINÁMICOS (Relativo a la cámara ubicada)
        // =================================================================================
        for(const auto &m:markers){
            if(fixed_markers.count(m.id)) continue; // Ya usados para la cámara

            auto r_t=m.estimatePose(cameraMatrix,distCoeffs,markerSize);
            auto rotation_matrix = r_t.first;
            auto traslation_vector = r_t.second;

            geometry_msgs::msg::TransformStamped tag_tf;
            tag_tf.header.stamp = msg->header.stamp;
            tag_tf.header.frame_id = camera_frame_id; // Hijo de la camara
            
            // ROBOTS (IDs 1-9) -> FORZAR 2D Y CORREGIR "FLIP" DE PISO
            if (m.id >= 1 && m.id <= 9) {
                std::stringstream ss; ss << "marker_id_0" << m.id;
                tag_tf.child_frame_id = ss.str();

                cv::Mat new_rmat; cv::Rodrigues(rotation_matrix, new_rmat);
                tf2::Matrix3x3 mat(new_rmat.at<double>(0,0), new_rmat.at<double>(0,1), new_rmat.at<double>(0,2),
                        new_rmat.at<double>(1,0), new_rmat.at<double>(1,1), new_rmat.at<double>(1,2),
                        new_rmat.at<double>(2,0), new_rmat.at<double>(2,1), new_rmat.at<double>(2,2));

                double r, p, y; mat.getRPY(r, p, y);
                // CORRECCION 180 GRADOS EN ROLL PARA ROBOTS EN EL PISO
                tf2::Quaternion qu; qu.setRPY(3.1415926535, 0.0, y); 

                tag_tf.transform.rotation.x = qu.x(); tag_tf.transform.rotation.y = qu.y();
                tag_tf.transform.rotation.z = qu.z(); tag_tf.transform.rotation.w = qu.w();

                // FILTRO SUAVE
                double rx = traslation_vector.at<double>(0);
                double ry = traslation_vector.at<double>(1);
                double rz = traslation_vector.at<double>(2);
                if (last_dynamic_pos.count(m.id)) {
                    double alpha = 0.3;
                    tag_tf.transform.translation.x = (rx * alpha) + (last_dynamic_pos[m.id].x * (1.0 - alpha));
                    tag_tf.transform.translation.y = (ry * alpha) + (last_dynamic_pos[m.id].y * (1.0 - alpha));
                    tag_tf.transform.translation.z = (rz * alpha) + (last_dynamic_pos[m.id].z * (1.0 - alpha));
                } else {
                    tag_tf.transform.translation.x = rx; tag_tf.transform.translation.y = ry; tag_tf.transform.translation.z = rz;
                }
                last_dynamic_pos[m.id] = cv::Point3f(tag_tf.transform.translation.x, tag_tf.transform.translation.y, tag_tf.transform.translation.z);
                tf_broadcaster->sendTransform(tag_tf);
            }
            // CUBOS (IDs > 10) -> FULL 3D
            else {
                std::stringstream ss; ss << "marker_id_" << m.id;
                tag_tf.child_frame_id = ss.str();

                cv::Mat new_rmat; cv::Rodrigues(rotation_matrix, new_rmat);
                tf2::Matrix3x3 mat(new_rmat.at<double>(0,0), new_rmat.at<double>(0,1), new_rmat.at<double>(0,2),
                        new_rmat.at<double>(1,0), new_rmat.at<double>(1,1), new_rmat.at<double>(1,2),
                        new_rmat.at<double>(2,0), new_rmat.at<double>(2,1), new_rmat.at<double>(2,2));
                tf2::Quaternion qu; mat.getRotation(qu);

                tag_tf.transform.rotation.x = qu.x(); tag_tf.transform.rotation.y = qu.y();
                tag_tf.transform.rotation.z = qu.z(); tag_tf.transform.rotation.w = qu.w();

                double rx = traslation_vector.at<double>(0);
                double ry = traslation_vector.at<double>(1);
                double rz = traslation_vector.at<double>(2);
                // Filtro ligero
                if (last_dynamic_pos.count(m.id)) {
                    double alpha = 0.4;
                    tag_tf.transform.translation.x = (rx * alpha) + (last_dynamic_pos[m.id].x * (1.0 - alpha));
                    tag_tf.transform.translation.y = (ry * alpha) + (last_dynamic_pos[m.id].y * (1.0 - alpha));
                    tag_tf.transform.translation.z = (rz * alpha) + (last_dynamic_pos[m.id].z * (1.0 - alpha));
                } else {
                    tag_tf.transform.translation.x = rx; tag_tf.transform.translation.y = ry; tag_tf.transform.translation.z = rz;
                }
                last_dynamic_pos[m.id] = cv::Point3f(tag_tf.transform.translation.x, tag_tf.transform.translation.y, tag_tf.transform.translation.z);
                tf_broadcaster->sendTransform(tag_tf);
            }
        }
      } catch (cv_bridge::Exception& e){
         RCLCPP_ERROR(this->get_logger(), "CV ERROR: %s", e.what());
      }
    }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Aruco_Nano_Detector>());
  rclcpp::shutdown();
  return 0;
}