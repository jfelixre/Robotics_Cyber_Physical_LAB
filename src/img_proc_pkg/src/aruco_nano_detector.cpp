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
#include "../include/img_proc_pkg/aruco_nano.h"


#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2_ros/transform_broadcaster.h"
#include <opencv2/video/tracking.hpp>



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

float Q= 1e-1;  //prediction

float R= 0.5;    //mesurement




void drawAxis(cv::Mat& img, cv::InputArrayOfArrays corners, cv::Vec3d rvec, cv::Vec3d tvec, float length = 0.1) {
   cv::drawFrameAxes(img, cameraMatrix, distCoeffs, rvec, tvec, length);
}



class Aruco_Nano_Detector : public rclcpp::Node
{
  public:
    Aruco_Nano_Detector() : Node("aruco_nano_detector")
    {
      bool readOk = readCameraParameters("src/img_proc_pkg/config/camera_calib_charuco.yaml", cameraMatrix, distCoeffs);

      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "cameras/camera1/image_raw", 10, std::bind(&Aruco_Nano_Detector::topic_callback, this, _1));

      // Initialize the transform broadcaster
      tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
           
      
    }

  private:



    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    { 

std::cout<< "aqui0" << std::endl;
      std::cout << msg->encoding << std::endl;
      //std::cout << msg->data << std::endl;
      std::cout << msg->step << std::endl;
      std::cout << msg->height << std::endl;
      std::cout << msg->width << std::endl;
      //RCLCPP_INFO(this->get_logger(), "Received image" );
      
      std::cout<< "aqui1" << std::endl;

      cv_bridge::CvImageConstPtr image_bridge;
	    //cv_bridge::toCvCopy(msg, RGB8)->image;
      try{
        image_bridge=cv_bridge::toCvCopy(msg);
        std::cout<< "aqui2" << std::endl;
        //std::cout<< image_bridge->image.channels << std::endl;
      
        img_original = image_bridge->image;

        std::cout<< "aqui3" << std::endl;

        //cv::cvtColor(img_original,img_original,cv::COLOR_BGR2RGB);
        //cv::imwrite("image.jpg",img_original);
        int n_cols = img_original.cols;
        std::cout<< "aqui4" << std::endl;
        int n_rows = img_original.rows;
        std::cout<< "aqui5" << std::endl;
        //img_mod=img_original.clone();
        std::cout<< "aqui6" << std::endl;
        //publisher->publish(data_image);
        std::cout<< img_original.channels() << std::endl;
        std::cout<< "aqui7" << std::endl;

              if (image_bridge->image.empty()) {
        RCLCPP_ERROR(this->get_logger(), "Empty image received");
        return;
            }

            else{
              RCLCPP_INFO(this->get_logger(), "IMAGE OK");
              // Define a smaller resolution
              int new_width = 640;
              int new_height = 480;

              // Resize the original image
              cv::resize(img_original, img_mod, cv::Size(new_width, new_height));


                        cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
          std::cout<< "aqui8" << std::endl;
         //cv::resizeWindow("Display Image" 1280,720);
         
          cv::imshow("Display Image", img_mod);
          cv::waitKey(1);
          
           std::cout<< "aqui9" << std::endl;
          //img_original.release();
          //img_mod.release();
          std::cout<< "aqui10" << std::endl;
          
                  auto markers = aruconano::MarkerDetector::detect(img_mod);
        std::cout<< "aqui11" << std::endl;
            }

      

      }
       catch (cv_bridge::Exception& e){
         RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
       return;
       }

     


      // std::vector<int> markerIds;
      // std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
      // cv::aruco::DetectorParameters parameters = cv::aruco::DetectorParameters();
      // cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
      // cv::aruco::ArucoDetector detector(dictionary, parameters);
      // detector.detectMarkers(img_original, markerCorners, markerIds, rejectedCandidates);

      // auto markers = aruconano::MarkerDetector::detect(img_original);

      // for(const auto &m:markers)
      //  m.draw(img_mod);

      
/*       if (markerIds.size() > 0){

        cv::aruco::drawDetectedMarkers(img_mod, markerCorners, markerIds); 
        //cv::aruco::drawDetectedMarkers(img_mod, rejectedCandidates); 

        std::vector<cv::Vec3d> rvecs, tvecs;

        cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.10, cameraMatrix, distCoeffs, rvecs, tvecs);

        for (int i = 0; i < rvecs.size(); ++i) {
          auto rvec = rvecs[i];
          cv::Vec<double, 3> tvec = tvecs[i];
          auto Id= markerIds[i];

          //MAKE A FILTER HERE FOR TVEC AND RVEC



          cv::drawFrameAxes(img_mod, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
          std::cout << "Id = " << std::endl << " "  << Id << std::endl << std::endl;
          std::cout << "rvec = " << std::endl << " "  << rvec << std::endl << std::endl;
          std::cout << "tvec = " << std::endl << " "  << tvec << std::endl << std::endl;

          geometry_msgs::msg::TransformStamped tag_tf;
          tag_tf.header.stamp = msg->header.stamp;
          
          if (Id==0){
            tag_tf.header.frame_id = "origin_aruco_tag";
            tag_tf.child_frame_id = "cam";

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

            
            
   

            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));
            
            tf2::Quaternion qu;
            
            mat.getRotation(qu);


            tag_tf.transform.rotation.x = qu.x();
            tag_tf.transform.rotation.y = qu.y();
            tag_tf.transform.rotation.z = qu.z();
            tag_tf.transform.rotation.w = qu.w();
            tag_tf.transform.translation.x = camera_translation_vector.at<double>(0);
            tag_tf.transform.translation.y = camera_translation_vector.at<double>(1);
            tag_tf.transform.translation.z = camera_translation_vector.at<double>(2);

            tf_broadcaster->sendTransform(tag_tf);


          }
          else{
            tag_tf.header.frame_id = "cam";
            std::stringstream ss_frame_name;
            ss_frame_name << "tag_" << Id;
            std::string frame_name = ss_frame_name.str();
            tag_tf.child_frame_id = frame_name;



            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat rmat_origin;
            cv::Rodrigues(rvec_origin,rmat_origin);

            cv::Mat camera_rotation_matrix = rmat;
            cv::Mat camera_rotation_matrix_origin = rmat_origin.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;


            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            tag_tf.transform.rotation.x = qu.x();
            tag_tf.transform.rotation.y = qu.y();
            tag_tf.transform.rotation.z = qu.z();
            tag_tf.transform.rotation.w = qu.w();
            tag_tf.transform.translation.x = tvec(0);
            tag_tf.transform.translation.y = tvec(1);
            tag_tf.transform.translation.z = tvec(2);

            tf_broadcaster->sendTransform(tag_tf);
            
          }



        }

      } */

      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Aruco_Nano_Detector>());
  rclcpp::shutdown();
  return 0;
}

