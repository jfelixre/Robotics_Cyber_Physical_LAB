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
//#include <std_msgs/msg/bool.hpp>
//#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <interfaces/msg/img_data.hpp>
#include "/opt/opencv_contrib/modules/aruco/samples/aruco_samples_utility.hpp"

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2_ros/transform_broadcaster.h"



using std::placeholders::_1;


using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;
namespace enc = sensor_msgs::image_encodings;

cv::Mat img_original;
cv::Mat img_mod;
cv::Mat cameraMatrix;
cv::Mat distCoeffs;
cv::Vec<double, 3> tvec_origin, rvec_origin;

double pi = 3.14159265358979323846;



class Aruco_Detector : public rclcpp::Node
{
  public:
    Aruco_Detector() : Node("aruco_detector")
    {
      bool readOk = readCameraParameters("src/img_proc_pkg/config/camera_calib_charuco.yaml", cameraMatrix, distCoeffs);

      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "cameras/camera1/image_raw", 10, std::bind(&Aruco_Detector::topic_callback, this, _1));
      //publisher = this->create_publisher<interfaces::msg::ImgData>("image_data",1);
      
      // Initialize the transform broadcaster
      //tf_cam_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      //tf_aruco_0_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      //tf_aruco_1_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      //tf_aruco_2_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      //tf_aruco_3_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      //tf_aruco_4_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      //tf_aruco_5_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
      tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);


    }

  private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
      RCLCPP_INFO(this->get_logger(), "Received image" );
      
      cv_bridge::CvImageConstPtr image_bridge;
	    //cv_bridge::toCvCopy(msg, RGB8)->image;
      image_bridge=cv_bridge::toCvCopy(msg);
      img_original = image_bridge->image;
      cv::cvtColor(img_original,img_original,cv::COLOR_BGR2RGB);
      //cv::imwrite("image.jpg",img_original);
      int n_cols = img_original.cols;
      int n_rows = img_original.rows;

      img_mod=img_original.clone();
      //publisher->publish(data_image);

      std::vector<int> markerIds;
      std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
      cv::aruco::DetectorParameters parameters = cv::aruco::DetectorParameters();
      cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
      cv::aruco::ArucoDetector detector(dictionary, parameters);
      detector.detectMarkers(img_original, markerCorners, markerIds, rejectedCandidates);

      if (markerIds.size() > 0){

        cv::aruco::drawDetectedMarkers(img_mod, markerCorners, markerIds); 
        //cv::aruco::drawDetectedMarkers(img_mod, rejectedCandidates); 

        //std::vector<cv::Vec3d> rvecs, tvecs;
        std::vector<cv::Vec3d> rvecs(markerIds.size()), tvecs(markerIds.size());
        //cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.10, cameraMatrix, distCoeffs, rvecs, tvecs);

        float markerLength = 0.10;

        // Set coordinate system
        cv::Mat objPoints(4, 1, CV_32FC3);
        objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength/2.f, markerLength/2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength/2.f, markerLength/2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength/2.f, -markerLength/2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength/2.f, -markerLength/2.f, 0);
        // Calculate pose for each marker
        
        for (int i = 0; i < markerIds.size(); i++) {
            solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i));
        }


        
         //std::vector<std::vector<cv::Point2f>> imgpoints;   //No sirve esta declaración de imgpoints
        //std::cout << "aqui no"<<std::endl;
        //imgpoints[0]=vector<Point3f>(0,0,0);
        //cv::solvePnP(imgpoints, markerCorners, cameraMatrix, distCoeffs,rvecs,tvecs);
        //std::cout << "aqui"<<std::endl;
 
        for (int i = 0; i < rvecs.size(); ++i) {
          auto rvec = rvecs[i];
          cv::Vec<double, 3> tvec = tvecs[i];
          auto Id= markerIds[i];
          cv::drawFrameAxes(img_mod, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
          std::cout << "Id = " << std::endl << " "  << Id << std::endl << std::endl;
          std::cout << "rvec = " << std::endl << " "  << rvec << std::endl << std::endl;
          std::cout << "tvec = " << std::endl << " "  << tvec << std::endl << std::endl;

          //rvec[1]=rvec[1]+pi; 
          //rvec[0]=rvec[0]-(pi/2);
          /*
          geometry_msgs::msg::TransformStamped t;
          // Read message content and assign it to
          // corresponding tf variables
          //rclcpp::Time now = this->get_clock()->now();
          t.header.stamp = this->get_clock()->now();
          

          t.transform.translation.x = tvec[0];
          t.transform.translation.y = tvec[1];
          t.transform.translation.z = tvec[2];

          tf2::Quaternion q;
          q.setRPY(rvec[2], rvec[1], rvec[0]);
          t.transform.rotation.x = q.x();
          t.transform.rotation.y = q.y();
          t.transform.rotation.z = q.z();
          t.transform.rotation.w = q.w();

          std::cout << "rx = " << std::endl << " "  << t.transform.rotation.x << std::endl << std::endl;
          std::cout << "ry = " << std::endl << " "  << t.transform.rotation.y << std::endl << std::endl;
          std::cout << "rz = " << std::endl << " "  << t.transform.rotation.z << std::endl << std::endl;
          std::cout << "rw = " << std::endl << " "  << t.transform.rotation.w << std::endl << std::endl;
          */

          if (Id==0){
            /*t.transform.translation.x = tvec[0];
            t.transform.translation.y = tvec[1];
            t.transform.translation.z = tvec[2];

            q.setRPY(rvec[2], rvec[1], rvec[0]);
            t.transform.rotation.x = q.x();
            t.transform.rotation.y = q.y();
            t.transform.rotation.z = q.z();
            t.transform.rotation.w = q.w();

            t.header.frame_id = "world";
            t.child_frame_id = "cam";

            // Send the transformation
            tf_cam_->sendTransform(t);

            t.transform.translation.x = tvec[0];
            t.transform.translation.y = tvec[1];
            t.transform.translation.z = tvec[2];

            q.setRPY(rvec[2], rvec[1], rvec[0]);
            t.transform.rotation.x = q.x();
            t.transform.rotation.y = q.y();
            t.transform.rotation.z = q.z();
            t.transform.rotation.w = q.w();
            */
            //t.header.frame_id = "cam";
            //t.child_frame_id = "origin_check";

            rvec_origin(0)=rvec[0];
            rvec_origin(1)=rvec[1];
            rvec_origin(2)=rvec[2];
            tvec_origin(0)=tvec[0];
            tvec_origin(1)=tvec[1];
            tvec_origin(2)=tvec[2];



            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);
            cv::Mat camera_rotation_matrix = rmat.t();
            cv::Mat camera_translation_vector = -camera_rotation_matrix * tvec;

            geometry_msgs::msg::TransformStamped camera_tf;
            camera_tf.header.stamp = msg->header.stamp;
            camera_tf.header.frame_id = "origin_aruco_tag";
            camera_tf.child_frame_id = "cam";
            
           //   std::cout << "rotation org = " << std::endl << " "  << rmat << std::endl << std::endl;
            //std::cout << "rotation = " << std::endl << " "  << camera_rotation_matrix << std::endl << std::endl;
            //std::cout << "translation = " << std::endl << " "  << camera_translation_vector << std::endl << std::endl;


            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));
            tf2::Quaternion qu;
            mat.getRotation(qu);


            camera_tf.transform.rotation.x = qu.x();
            camera_tf.transform.rotation.y = qu.y();
            camera_tf.transform.rotation.z = qu.z();
            camera_tf.transform.rotation.w = qu.w();
            camera_tf.transform.translation.x = camera_translation_vector.at<double>(0);
            camera_tf.transform.translation.y = camera_translation_vector.at<double>(1);
            camera_tf.transform.translation.z = camera_translation_vector.at<double>(2);




            // Send the transformation
            tf_broadcaster->sendTransform(camera_tf);

          }
          if (Id==1){

            geometry_msgs::msg::TransformStamped r1_tf;
            r1_tf.header.stamp = msg->header.stamp;
            r1_tf.header.frame_id = "cam";
            r1_tf.child_frame_id = "Robot1";

            //std::cout << "tvec_origin = " << std::endl << " "  << tvec_origin << std::endl << std::endl;
            //t.header.frame_id = "cam";
            //t.child_frame_id = "Robot1";
            //cv::Vec<double, 3> position = tvec;
            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat rmat_origin;
            cv::Rodrigues(rvec_origin,rmat_origin);

            cv::Mat camera_rotation_matrix = rmat.t();
            cv::Mat camera_rotation_matrix_origin = rmat_origin.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;

            //std::cout << "rotation = " << std::endl << " "  << camera_rotation_matrix << std::endl << std::endl;
            //std::cout << "translation = " << std::endl << " "  << camera_translation_vector << std::endl << std::endl;


            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            r1_tf.transform.rotation.x = qu.x();
            r1_tf.transform.rotation.y = qu.y();
            r1_tf.transform.rotation.z = qu.z();
            r1_tf.transform.rotation.w = qu.w();
            //r1_tf.transform.translation.x = camera_translation_vector.at<double>(0);
            //r1_tf.transform.translation.y = camera_translation_vector.at<double>(1);
            //r1_tf.transform.translation.z = camera_translation_vector.at<double>(2);
            r1_tf.transform.translation.x = tvec(0);
            r1_tf.transform.translation.y = tvec(1);
            r1_tf.transform.translation.z = tvec(2);

            // Send the transformation
            tf_broadcaster->sendTransform(r1_tf);

          }

          
          if (Id==2){
            geometry_msgs::msg::TransformStamped r2_tf;
            r2_tf.header.stamp = msg->header.stamp;
            r2_tf.header.frame_id = "cam";
            r2_tf.child_frame_id = "Robot2";

            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat camera_rotation_matrix = rmat.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;

    
            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            r2_tf.transform.rotation.x = qu.x();
            r2_tf.transform.rotation.y = qu.y();
            r2_tf.transform.rotation.z = qu.z();
            r2_tf.transform.rotation.w = qu.w();
            r2_tf.transform.translation.x = tvec(0);
            r2_tf.transform.translation.y = tvec(1);
            r2_tf.transform.translation.z = tvec(2);

            // Send the transformation
            tf_broadcaster->sendTransform(r2_tf);


          }
          if (Id==3){
            geometry_msgs::msg::TransformStamped o1_tf;
            o1_tf.header.stamp = msg->header.stamp;
            o1_tf.header.frame_id = "cam";
            o1_tf.child_frame_id = "Object1";

            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat camera_rotation_matrix = rmat.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;

    
            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            o1_tf.transform.rotation.x = qu.x();
            o1_tf.transform.rotation.y = qu.y();
            o1_tf.transform.rotation.z = qu.z();
            o1_tf.transform.rotation.w = qu.w();
            o1_tf.transform.translation.x = tvec(0);
            o1_tf.transform.translation.y = tvec(1);
            o1_tf.transform.translation.z = tvec(2);

            // Send the transformation
            tf_broadcaster->sendTransform(o1_tf);

          }
          if (Id==4){
            geometry_msgs::msg::TransformStamped o2_tf;
            o2_tf.header.stamp = msg->header.stamp;
            o2_tf.header.frame_id = "cam";
            o2_tf.child_frame_id = "Object2";

            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat camera_rotation_matrix = rmat.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;

    
            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            o2_tf.transform.rotation.x = qu.x();
            o2_tf.transform.rotation.y = qu.y();
            o2_tf.transform.rotation.z = qu.z();
            o2_tf.transform.rotation.w = qu.w();
            o2_tf.transform.translation.x = tvec(0);
            o2_tf.transform.translation.y = tvec(1);
            o2_tf.transform.translation.z = tvec(2);

            // Send the transformation
            tf_broadcaster->sendTransform(o2_tf);

          }

          if (Id==5){
            geometry_msgs::msg::TransformStamped tj_tf;
            tj_tf.header.stamp = msg->header.stamp;
            tj_tf.header.frame_id = "cam";
            tj_tf.child_frame_id = "Target";

            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat camera_rotation_matrix = rmat.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;

    
            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            tj_tf.transform.rotation.x = qu.x();
            tj_tf.transform.rotation.y = qu.y();
            tj_tf.transform.rotation.z = qu.z();
            tj_tf.transform.rotation.w = qu.w();
            tj_tf.transform.translation.x = tvec(0);
            tj_tf.transform.translation.y = tvec(1);
            tj_tf.transform.translation.z = tvec(2);

            // Send the transformation
            tf_broadcaster->sendTransform(tj_tf);

          }
          
        






        }


        //RCLCPP_INFO(this->get_logger(), "markerCorners %f", markerCorners);
        //cv::Ptr<cv::Formatter> fmt = cv::Formatter::get(cv::Formatter::FMT_DEFAULT);
        //fmt->set64fPrecision(4);
        //fmt->set32fPrecision(4);
        //std::cout << fmt->format(markerCorners) << std::endl;
        std::cout << "markerCorners = " << std::endl << " "  << markerCorners[0,0] << std::endl << std::endl;
        //std::cout << "markerIds = " << std::endl << " "  << markerIds.size << std::endl << std::endl;
        //RCLCPP_INFO(this->get_logger(), "markerIds %f", markerIds);
        //RCLCPP_INFO(this->get_logger(), "Rotation Vector %f", rvecs);
        //RCLCPP_INFO(this->get_logger(), "Traslation Vector %f", tvecs);
      }

  
  	  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  	  cv::imshow("Display Image", img_mod);
  	  cv::waitKey(1);
      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    //rclcpp::Publisher<interfaces::msg::ImgData>::SharedPtr publisher;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_cam_;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_aruco_0_;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_aruco_1_;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_aruco_2_;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_aruco_3_;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_aruco_4_;
    //std::unique_ptr<tf2_ros::TransformBroadcaster> tf_aruco_5_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Aruco_Detector>());
  rclcpp::shutdown();
  return 0;
}