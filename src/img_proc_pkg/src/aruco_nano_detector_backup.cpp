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

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/aruco.hpp> 
#include <stdio.h>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/image_encodings.hpp>
#include <std_msgs/msg/bool.hpp>
#include <image_transport/image_transport.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <interfaces/msg/img_data.hpp>
#include "opencv2/aruco.hpp"
#include "../include/img_proc_pkg/aruco_nano.h"

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2_ros/transform_broadcaster.h"
#include <opencv2/video/tracking.hpp>
#include <fstream>
#include <map>

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;
namespace enc = sensor_msgs::image_encodings;

// Variables globales (Mantengo tu estructura original)
cv::Mat img_original;
cv::Mat cameraMatrix;
cv::Mat distCoeffs;
float markerSize=0.11;    
int image_width, image_height;
double pi = 3.14159265358979323846;
float Q= 1e-1;  
float R= 0.5;    
geometry_msgs::msg::TransformStamped saved_tag;
bool register_tag_00 = false;

class Aruco_Nano_Detector : public rclcpp::Node
{
  public:
    Aruco_Nano_Detector() : Node("aruco_nano_detector")
    {
      std::cout<<"Aruco Nano Detector Node Started"<<std::endl;
      
      // Parametros
      this->declare_parameter<std::string>("calibration_file", "/home/javierfr/Robotics_Cyber_Physical_LAB/src/img_proc_pkg/config/camera_calib_charuco.yaml");
      std::string calib_file = this->get_parameter("calibration_file").as_string();

      this->declare_parameter<std::string>("camera_topic", "/cameras/cam_1");
      this->declare_parameter<std::string>("camera_frame", "cam_1");
      this->declare_parameter<float>("marker_size", 0.0938); 

      camera_topic = this->get_parameter("camera_topic").as_string();
      camera_frame_id = this->get_parameter("camera_frame").as_string();
      markerSize = this->get_parameter("marker_size").as_double();

      // Parametros de Failover
      this->declare_parameter<std::string>("master_camera_status_topic", "");
      std::string master_topics_str = this->get_parameter("master_camera_status_topic").as_string();

      // Publishers
      status_pub_ = this->create_publisher<std_msgs::msg::Bool>("~/status", 10);
      
      // NUEVO: Publisher de Imagen de Salida (para RViz)
      // Se publicará en el namespace del nodo, ej: /aruco_nano_detector/result_image
      result_img_pub_ = this->create_publisher<sensor_msgs::msg::Image>("~/result_image", 10);

      // Suscriptores a Maestros
      if (!master_topics_str.empty()) {
          std::stringstream ss(master_topics_str);
          std::string segment;
          while (std::getline(ss, segment, ',')) {
              segment.erase(0, segment.find_first_not_of(' '));
              segment.erase(segment.find_last_not_of(' ') + 1);
              if (segment.empty()) continue;

              auto topic = segment;
              auto sub = this->create_subscription<std_msgs::msg::Bool>(
                  topic, 10, 
                  [this, topic](const std_msgs::msg::Bool::SharedPtr msg) {
                      this->master_status_map_[topic] = msg->data;
                      this->master_heartbeat_map_[topic] = this->now();
                  });
              master_subs_.push_back(sub);
              master_status_map_[topic] = false;
              master_heartbeat_map_[topic] = rclcpp::Time(0);
              RCLCPP_INFO(this->get_logger(), "Monitoring Master: %s", topic.c_str());
          }
          RCLCPP_INFO(this->get_logger(), "Configured as Secondary/Backup Node.");
      } else {
          RCLCPP_INFO(this->get_logger(), "Configured as Primary (or Standalone).");
      }

      RCLCPP_INFO(this->get_logger(), "Configured for Camera: %s. Marker Size: %.4f", camera_frame_id.c_str(), markerSize);

      // Cargar calibracion
      cv::FileStorage fs(calib_file, cv::FileStorage::READ);
      if(!fs.isOpened()){
        RCLCPP_ERROR(this->get_logger(), "Could not open calibration file: %s", calib_file.c_str());
        cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        cameraMatrix.at<double>(0, 0) = 1000.0;
        cameraMatrix.at<double>(1, 1) = 1000.0;
        cameraMatrix.at<double>(0, 2) = 640.0;
        cameraMatrix.at<double>(1, 2) = 360.0;
        distCoeffs = cv::Mat::zeros(1, 5, CV_64F);
      } else {
        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distCoeffs;
        fs["image_widht"] >> image_width;
        fs["image_height"] >> image_height;
        fs.release();
      }
      
      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      camera_topic, 10, std::bind(&Aruco_Nano_Detector::topic_callback, this, _1));

      tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

      // Timer Timeout
      last_image_time_ = this->now();
      status_timeout_timer_ = this->create_wall_timer(
          500ms, [this]() {
              if ((this->now() - last_image_time_).seconds() > 2.0) {
                  std_msgs::msg::Bool status_msg;
                  status_msg.data = false;
                  status_pub_->publish(status_msg);
              }
          });

      // Inicializar marcadores fijos
      fixed_markers[0] = cv::Point3f(0.0, 0.0, 0.0);
      fixed_markers[99] = cv::Point3f(-2.0, 1.0, 0.0);
      fixed_markers[98] = cv::Point3f(0.0, 1.0, 0.0);
      fixed_markers[97] = cv::Point3f(2.0, 1.0, 0.0);
      fixed_markers[96] = cv::Point3f(-2.0, 0.0, 0.0);
      fixed_markers[95] = cv::Point3f(2.0, 0.0, 0.0);
      fixed_markers[94] = cv::Point3f(-2.0, -1.0, 0.0);
      fixed_markers[93] = cv::Point3f(0.0, -1.0, 0.0);
      fixed_markers[92] = cv::Point3f(2.0, -1.0, 0.0);

      // Kalman
      int state_dim_cam = 14; 
      int meas_dim_cam = 7;
      kf_cam = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
      cv::setIdentity(kf_cam.measurementMatrix);
      cv::setIdentity(kf_cam.processNoiseCov, cv::Scalar::all(Q));
      cv::setIdentity(kf_cam.measurementNoiseCov, cv::Scalar::all(R));
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
    // NUEVO: Publisher de imagen
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

        if (image_bridge->image.empty()) {
          RCLCPP_ERROR(this->get_logger(), "Empty image received");
          return;
        }
        else{
          // --- DETECCION DE MARCADORES ---
          auto markers = aruconano::MarkerDetector::detect(img_original);
          
          // Dibujar marcadores en la imagen original para visualizacion
          for(const auto &m:markers)
            m.draw(img_original);

          // Verificar visibilidad de marcadores fijos
          bool fixed_marker_visible = false;
          for(const auto &m:markers){
              if(fixed_markers.count(m.id)){
                  fixed_marker_visible = true;
                  break;
              }
          }

          // Publicar estado de visibilidad
          std_msgs::msg::Bool status_msg;
          status_msg.data = fixed_marker_visible;
          status_pub_->publish(status_msg);

          // --- LOGICA DE PRIORIDAD (FAILOVER) ---
          bool any_master_healthy = false;
          if (!master_subs_.empty()) {
              for (auto const& [topic, is_alive] : master_status_map_) {
                  double seconds_since_heartbeat = (this->now() - master_heartbeat_map_[topic]).seconds();
                  // Si algun maestro esta vivo y reportando recientemente -> Entramos en Standby
                  if (is_alive && seconds_since_heartbeat < 2.0) {
                      any_master_healthy = true;
                      break;
                  }
              }

              if (any_master_healthy) {
                  // MODO STANDBY: 
                  // 1. NO publicamos TF.
                  // 2. NO publicamos la imagen procesada (para ahorrar ancho de banda y no saturar RViz).
                  return; 
              }
          }

          // SI LLEGAMOS AQUI, SOMOS LA CAMARA ACTIVA (PRIORITARIA)

          // 1. PUBLICAR IMAGEN A ROS (RViz)
          // Convertir cv::Mat de vuelta a sensor_msgs::msg::Image
          std_msgs::msg::Header header = msg->header; // Usar el mismo header que la entrada (timestamp importante)
          cv_bridge::CvImage img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::BGR8, img_original);
          result_img_pub_->publish(*img_bridge.toImageMsg());

          // 2. CALCULO Y PUBLICACION DE TF
          register_tag_00=false;
        
          for(const auto &m:markers){
            auto r_t=m.estimatePose(cameraMatrix,distCoeffs,markerSize);
            // Opcional: reducir logs en produccion
            // std::cout << "ID: " << m.id << std::endl;

            auto rotation_matrix = r_t.first;
            auto traslation_vector = r_t.second;

            geometry_msgs::msg::TransformStamped tag_tf;
            tag_tf.header.stamp = msg->header.stamp;
            
            // --- LOGICA DE MARCADORES FIJOS (LOCALIZACION CAMARA) ---
            if(fixed_markers.count(m.id)){
              tag_tf.header.frame_id = "marker_id_00"; 
              tag_tf.child_frame_id = camera_frame_id;

              cv::Mat rvec = rotation_matrix; 
              cv::Mat tvec = traslation_vector; 

              cv::Mat rmat;
              cv::Rodrigues(rvec, rmat); 

              cv::Mat rmat_inv = rmat.t();
              cv::Mat tvec_inv = -rmat_inv * tvec; 
              
              cv::Point3f marker_pos_world = fixed_markers[m.id];
              
              tvec_inv.at<double>(0) += marker_pos_world.x;
              tvec_inv.at<double>(1) += marker_pos_world.y;
              tvec_inv.at<double>(2) += marker_pos_world.z;

              // --- AQUI ESTA EL CAMBIO CRITICO: FORZAR 2D (APLANAR) ---
              tf2::Matrix3x3 mat(rmat_inv.at<double>(0,0), rmat_inv.at<double>(0,1), rmat_inv.at<double>(0,2),
                    rmat_inv.at<double>(1,0), rmat_inv.at<double>(1,1), rmat_inv.at<double>(1,2),
                    rmat_inv.at<double>(2,0), rmat_inv.at<double>(2,1), rmat_inv.at<double>(2,2));
              
              double roll, pitch, yaw;
              mat.getRPY(roll, pitch, yaw);

              // REGLA DE ORO: Un robot de ruedas no vuela ni se entierra.
              // Ignoramos Roll y Pitch (que vienen del ruido/vibración) y solo usamos Yaw.
              tf2::Quaternion qu;
              qu.setRPY(0.0, 0.0, yaw); 

              // --- KALMAN FILTER (Ahora recibe datos limpios y planos) ---
              if (state_cam.empty())
              {
                  state_cam = cv::Mat::zeros(kf_cam.statePre.rows, kf_cam.statePre.cols, kf_cam.statePre.type());
                  kf_cam.statePost.at<float>(0, 0) = tvec_inv.at<double>(0);
                  kf_cam.statePost.at<float>(1, 0) = tvec_inv.at<double>(1);
                  kf_cam.statePost.at<float>(2, 0) = tvec_inv.at<double>(2);
                  kf_cam.statePost.at<float>(3, 0) = qu.x();
                  kf_cam.statePost.at<float>(4, 0) = qu.y();
                  kf_cam.statePost.at<float>(5, 0) = qu.z();
                  kf_cam.statePost.at<float>(6, 0) = qu.w();
              }

              // FILTRO DE SALTOS BRUSCOS (Outliers)
              double dist_sq = 0.0;
              dist_sq += pow(tvec_inv.at<double>(0) - kf_cam.statePost.at<float>(0, 0), 2);
              dist_sq += pow(tvec_inv.at<double>(1) - kf_cam.statePost.at<float>(1, 0), 2);
              dist_sq += pow(tvec_inv.at<double>(2) - kf_cam.statePost.at<float>(2, 0), 2); // Z
              double dist = sqrt(dist_sq);

              // Si el salto es mayor a 30cm, es un error de detección (glitch)
              if (dist > 0.3) { 
                  // Usamos la predicción del modelo (historia), ignoramos la lectura actual
                  kf_cam.predict();
                  tag_tf.transform.translation.x = kf_cam.statePost.at<float>(0,0);
                  tag_tf.transform.translation.y = kf_cam.statePost.at<float>(1,0);
                  // Opcional: Forzar Z fijo si el piso es plano
                  // tag_tf.transform.translation.z = 0.15; // Altura fija de camara
                  tag_tf.transform.translation.z = kf_cam.statePost.at<float>(2,0);
                  
                  tag_tf.transform.rotation.x = kf_cam.statePost.at<float>(3,0);
                  tag_tf.transform.rotation.y = kf_cam.statePost.at<float>(4,0);
                  tag_tf.transform.rotation.z = kf_cam.statePost.at<float>(5,0);
                  tag_tf.transform.rotation.w = kf_cam.statePost.at<float>(6,0);
              } else {
                  // Lectura válida: Actualizamos el Kalman
                  cv::Mat measurement_cam(7, 1, CV_32F);
                  measurement_cam.at<float>(0, 0) = tvec_inv.at<double>(0);
                  measurement_cam.at<float>(1, 0) = tvec_inv.at<double>(1);
                  measurement_cam.at<float>(2, 0) = tvec_inv.at<double>(2);
                  measurement_cam.at<float>(3, 0) = qu.x();
                  measurement_cam.at<float>(4, 0) = qu.y();
                  measurement_cam.at<float>(5, 0) = qu.z();
                  measurement_cam.at<float>(6, 0) = qu.w();

                  kf_cam.predict();
                  cv::Mat corrected_state = kf_cam.correct(measurement_cam);

                  tag_tf.transform.translation.x = corrected_state.at<float>(0,0);
                  tag_tf.transform.translation.y = corrected_state.at<float>(1,0);
                  tag_tf.transform.translation.z = corrected_state.at<float>(2,0);
                  
                  tag_tf.transform.rotation.x = corrected_state.at<float>(3,0);
                  tag_tf.transform.rotation.y = corrected_state.at<float>(4,0);
                  tag_tf.transform.rotation.z = corrected_state.at<float>(5,0);
                  tag_tf.transform.rotation.w = corrected_state.at<float>(6,0);
              }
              // --- KALMAN FILTER END ---

              tf_broadcaster->sendTransform(tag_tf);
              saved_tag=tag_tf;
              register_tag_00=true;

              // Publicar TF de los marcadores fijos detectados para visualizacion
              if (m.id != 0) {
                  geometry_msgs::msg::TransformStamped marker_vis_tf;
                  marker_vis_tf.header.stamp = msg->header.stamp;
                  marker_vis_tf.header.frame_id = camera_frame_id;
                  
                  std::stringstream ss_vis_name;
                  ss_vis_name << "marker_id_" << m.id;
                  marker_vis_tf.child_frame_id = ss_vis_name.str();

                  cv::Mat rmat_vis;
                  cv::Rodrigues(rvec, rmat_vis);
                  
                  tf2::Matrix3x3 mat_vis(rmat_vis.at<double>(0,0), rmat_vis.at<double>(0,1), rmat_vis.at<double>(0,2),
                        rmat_vis.at<double>(1,0), rmat_vis.at<double>(1,1), rmat_vis.at<double>(1,2),
                        rmat_vis.at<double>(2,0), rmat_vis.at<double>(2,1), rmat_vis.at<double>(2,2));
                  
                  tf2::Quaternion qu_vis;
                  mat_vis.getRotation(qu_vis);

                  marker_vis_tf.transform.translation.x = tvec.at<double>(0);
                  marker_vis_tf.transform.translation.y = tvec.at<double>(1);
                  marker_vis_tf.transform.translation.z = tvec.at<double>(2);
                  marker_vis_tf.transform.rotation.x = qu_vis.x();
                  marker_vis_tf.transform.rotation.y = qu_vis.y();
                  marker_vis_tf.transform.rotation.z = qu_vis.z();
                  marker_vis_tf.transform.rotation.w = qu_vis.w();

                  tf_broadcaster->sendTransform(marker_vis_tf);
              }
            }
            else{
              // --- LOGICA DE MARCADORES DINAMICOS (Objetos) ---
              tag_tf.header.frame_id = camera_frame_id;
              std::stringstream ss_frame_name;

              if (m.id<10)
                ss_frame_name << "marker_id_0" << m.id;
              else{
                ss_frame_name << "marker_id_" << m.id;
              }

              std::string frame_name = ss_frame_name.str();
              tag_tf.child_frame_id = frame_name;

              cv::Mat new_rmat;
              cv::Rodrigues(rotation_matrix,new_rmat);
              cv::Mat camera_translation_vector = traslation_vector;

              tf2::Matrix3x3 mat(new_rmat.at<double>(0,0), new_rmat.at<double>(0,1), new_rmat.at<double>(0,2),
                    new_rmat.at<double>(1,0), new_rmat.at<double>(1,1), new_rmat.at<double>(1,2),
                    new_rmat.at<double>(2,0), new_rmat.at<double>(2,1), new_rmat.at<double>(2,2));

              tf2::Quaternion qu;
              mat.getRotation(qu);

              tag_tf.transform.rotation.x = qu.x();
              tag_tf.transform.rotation.y = qu.y();
              tag_tf.transform.rotation.z = qu.z();
              tag_tf.transform.rotation.w = qu.w();
              tag_tf.transform.translation.x = camera_translation_vector.at<double>(0);
              tag_tf.transform.translation.y = camera_translation_vector.at<double>(1);
              tag_tf.transform.translation.z = camera_translation_vector.at<double>(2);

              bool publish_dynamic = true;
              if (last_dynamic_pos.count(m.id)) {
                  double dist_sq = 0.0;
                  dist_sq += pow(camera_translation_vector.at<double>(0) - last_dynamic_pos[m.id].x, 2);
                  dist_sq += pow(camera_translation_vector.at<double>(1) - last_dynamic_pos[m.id].y, 2);
                  dist_sq += pow(camera_translation_vector.at<double>(2) - last_dynamic_pos[m.id].z, 2);
                  double dist = sqrt(dist_sq);

                  if (dist > 0.5) {
                      publish_dynamic = false;
                  }
              }

              if (publish_dynamic) {
                  tf_broadcaster->sendTransform(tag_tf);
                  last_dynamic_pos[m.id] = cv::Point3f(
                      camera_translation_vector.at<double>(0),
                      camera_translation_vector.at<double>(1),
                      camera_translation_vector.at<double>(2)
                  );
              }
            } 
          } // End for loop

          if (register_tag_00==false){
            tf_broadcaster->sendTransform(saved_tag);
          }
        }
      }
       catch (cv_bridge::Exception& e){
         RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
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