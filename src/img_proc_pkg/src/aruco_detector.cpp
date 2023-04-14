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






class Aruco_Detector : public rclcpp::Node
{
  public:
    Aruco_Detector() : Node("aruco_detector")
    {
      bool readOk = readCameraParameters("src/img_proc_pkg/config/camera_calib_charuco.yaml", cameraMatrix, distCoeffs);

      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "cameras/camera1/image_raw", 10, std::bind(&Aruco_Detector::topic_callback, this, _1));
           
      // Initialize the transform broadcaster
      tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);


       // Initialize Kalman filter variables
        int state_dim_cam = 14;  // (x, y, z, rx, ry, rz, rw, vx, vy, vz, vrx, vry, vrz, vrw)
        int meas_dim_cam = 7;   // (x, y, z, rx, ry,rz, rw)
        kf_cam = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
        cv::setIdentity(kf_cam.measurementMatrix);
        cv::setIdentity(kf_cam.processNoiseCov, cv::Scalar::all(Q));
        cv::setIdentity(kf_cam.measurementNoiseCov, cv::Scalar::all(R));
        cv::setIdentity(kf_cam.errorCovPost, cv::Scalar::all(1));

        int state_dim_r1 = 14;  // (x, y, z, rx, ry, rz, rw, vx, vy, vz, vrx, vry, vrz, vrw)
        int meas_dim_r1 = 7;   // (x, y, z, rx, ry,rz, rw)
        kf_r1 = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
        cv::setIdentity(kf_r1.measurementMatrix);
        cv::setIdentity(kf_r1.processNoiseCov, cv::Scalar::all(Q));
        cv::setIdentity(kf_r1.measurementNoiseCov, cv::Scalar::all(R));
        cv::setIdentity(kf_r1.errorCovPost, cv::Scalar::all(1));

        int state_dim_r2 = 14;  // (x, y, z, rx, ry, rz, rw, vx, vy, vz, vrx, vry, vrz, vrw)
        int meas_dim_r2 = 7;   // (x, y, z, rx, ry,rz, rw)
        kf_r2 = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
        cv::setIdentity(kf_r2.measurementMatrix);
        cv::setIdentity(kf_r2.processNoiseCov, cv::Scalar::all(Q));
        cv::setIdentity(kf_r2.measurementNoiseCov, cv::Scalar::all(R));
        cv::setIdentity(kf_r2.errorCovPost, cv::Scalar::all(1));

        int state_dim_o1 = 14;  // (x, y, z, rx, ry, rz, rw, vx, vy, vz, vrx, vry, vrz, vrw)
        int meas_dim_o1 = 7;   // (x, y, z, rx, ry,rz, rw)
        kf_o1 = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
        cv::setIdentity(kf_o1.measurementMatrix);
        cv::setIdentity(kf_o1.processNoiseCov, cv::Scalar::all(Q));
        cv::setIdentity(kf_o1.measurementNoiseCov, cv::Scalar::all(R));
        cv::setIdentity(kf_o1.errorCovPost, cv::Scalar::all(1));

        int state_dim_o2 = 14;  // (x, y, z, rx, ry, rz, rw, vx, vy, vz, vrx, vry, vrz, vrw)
        int meas_dim_o2 = 7;   // (x, y, z, rx, ry,rz, rw)
        kf_o2 = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
        cv::setIdentity(kf_o2.measurementMatrix);
        cv::setIdentity(kf_o2.processNoiseCov, cv::Scalar::all(Q));
        cv::setIdentity(kf_o2.measurementNoiseCov, cv::Scalar::all(R));
        cv::setIdentity(kf_o2.errorCovPost, cv::Scalar::all(1));

        int state_dim_tj = 14;  // (x, y, z, rx, ry, rz, rw, vx, vy, vz, vrx, vry, vrz, vrw)
        int meas_dim_tj = 7;   // (x, y, z, rx, ry,rz, rw)
        kf_tj = cv::KalmanFilter(state_dim_cam, meas_dim_cam, 0);
        cv::setIdentity(kf_tj.measurementMatrix);
        cv::setIdentity(kf_tj.processNoiseCov, cv::Scalar::all(Q));
        cv::setIdentity(kf_tj.measurementNoiseCov, cv::Scalar::all(R));
        cv::setIdentity(kf_tj.errorCovPost, cv::Scalar::all(1));

    }

  private:

    cv::KalmanFilter kf_cam;
    cv::Mat state_cam;
    cv::KalmanFilter kf_r1;
    cv::Mat state_r1;
    cv::KalmanFilter kf_r2;
    cv::Mat state_r2;
    cv::KalmanFilter kf_o1;
    cv::Mat state_o1;
    cv::KalmanFilter kf_o2;
    cv::Mat state_o2;
    cv::KalmanFilter kf_tj;
    cv::Mat state_tj;


    geometry_msgs::msg::TransformStamped camera_tf;
    geometry_msgs::msg::TransformStamped r1_tf;
    geometry_msgs::msg::TransformStamped r2_tf;
    geometry_msgs::msg::TransformStamped o1_tf;
    geometry_msgs::msg::TransformStamped o2_tf;
    geometry_msgs::msg::TransformStamped tj_tf;




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


      
        camera_tf.header.stamp = msg->header.stamp;
        camera_tf.header.frame_id = "origin_aruco_tag";
        camera_tf.child_frame_id = "cam";

      
        r1_tf.header.stamp = msg->header.stamp;
        r1_tf.header.frame_id = "cam";
        r1_tf.child_frame_id = "Robot1";

      
        r2_tf.header.stamp = msg->header.stamp;
        r2_tf.header.frame_id = "cam";
        r2_tf.child_frame_id = "Robot2";

      
        o1_tf.header.stamp = msg->header.stamp;
        o1_tf.header.frame_id = "cam";
        o1_tf.child_frame_id = "Object1";

      
        o2_tf.header.stamp = msg->header.stamp;
        o2_tf.header.frame_id = "cam";
        o2_tf.child_frame_id = "Object2";

      
        tj_tf.header.stamp = msg->header.stamp;
        tj_tf.header.frame_id = "cam";
        tj_tf.child_frame_id = "Target";




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


 
        for (int i = 0; i < rvecs.size(); ++i) {
          auto rvec = rvecs[i];
          cv::Vec<double, 3> tvec = tvecs[i];
          auto Id= markerIds[i];
          cv::drawFrameAxes(img_mod, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
          std::cout << "Id = " << std::endl << " "  << Id << std::endl << std::endl;
          std::cout << "rvec = " << std::endl << " "  << rvec << std::endl << std::endl;
          std::cout << "tvec = " << std::endl << " "  << tvec << std::endl << std::endl;


          

          


          if (Id==0){


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


            camera_tf.transform.rotation.x = qu.x();
            camera_tf.transform.rotation.y = qu.y();
            camera_tf.transform.rotation.z = qu.z();
            camera_tf.transform.rotation.w = qu.w();
            camera_tf.transform.translation.x = camera_translation_vector.at<double>(0);
            camera_tf.transform.translation.y = camera_translation_vector.at<double>(1);
            camera_tf.transform.translation.z = camera_translation_vector.at<double>(2);




            

          }
          if (Id==1){

            


            cv::Mat rmat;
            cv::Rodrigues(rvec,rmat);

            cv::Mat rmat_origin;
            cv::Rodrigues(rvec_origin,rmat_origin);

            cv::Mat camera_rotation_matrix = rmat.t();
            cv::Mat camera_rotation_matrix_origin = rmat_origin.t();

            cv::Mat camera_translation_vector =   camera_rotation_matrix * tvec ;


            tf2::Matrix3x3 mat(camera_rotation_matrix.at<double>(0,0), camera_rotation_matrix.at<double>(0,1), camera_rotation_matrix.at<double>(0,2),
                   camera_rotation_matrix.at<double>(1,0), camera_rotation_matrix.at<double>(1,1), camera_rotation_matrix.at<double>(1,2),
                   camera_rotation_matrix.at<double>(2,0), camera_rotation_matrix.at<double>(2,1), camera_rotation_matrix.at<double>(2,2));



            tf2::Quaternion qu;
            mat.getRotation(qu);

            r1_tf.transform.rotation.x = qu.x();
            r1_tf.transform.rotation.y = qu.y();
            r1_tf.transform.rotation.z = qu.z();
            r1_tf.transform.rotation.w = qu.w();
            r1_tf.transform.translation.x = tvec(0);
            r1_tf.transform.translation.y = tvec(1);
            r1_tf.transform.translation.z = tvec(2);

            

          }

          
          if (Id==2){
            

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




          }
          if (Id==3){
            

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



          }
          if (Id==4){
            

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

            

          }

          if (Id==5){
            

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

            

          }
          
        






        }







        // Convert PoseStamped message to OpenCV Mat
        cv::Mat measurement_cam(7, 1, CV_32F);
        measurement_cam.at<float>(0, 0) = camera_tf.transform.translation.x;
        measurement_cam.at<float>(1, 0) = camera_tf.transform.translation.y;
        measurement_cam.at<float>(2, 0) = camera_tf.transform.translation.z;
        measurement_cam.at<float>(3, 0) = camera_tf.transform.rotation.x;
        measurement_cam.at<float>(4, 0) = camera_tf.transform.rotation.y;
        measurement_cam.at<float>(5, 0) = camera_tf.transform.rotation.z;
        measurement_cam.at<float>(6, 0) = camera_tf.transform.rotation.w;

        cv::Mat measurement_r1(7, 1, CV_32F);
        measurement_r1.at<float>(0, 0) = r1_tf.transform.translation.x;
        measurement_r1.at<float>(1, 0) = r1_tf.transform.translation.y;
        measurement_r1.at<float>(2, 0) = r1_tf.transform.translation.z;
        measurement_r1.at<float>(3, 0) = r1_tf.transform.rotation.x;
        measurement_r1.at<float>(4, 0) = r1_tf.transform.rotation.y;
        measurement_r1.at<float>(5, 0) = r1_tf.transform.rotation.z;
        measurement_r1.at<float>(6, 0) = r1_tf.transform.rotation.w;

        cv::Mat measurement_r2(7, 1, CV_32F);
        measurement_r2.at<float>(0, 0) = r2_tf.transform.translation.x;
        measurement_r2.at<float>(1, 0) = r2_tf.transform.translation.y;
        measurement_r2.at<float>(2, 0) = r2_tf.transform.translation.z;
        measurement_r2.at<float>(3, 0) = r2_tf.transform.rotation.x;
        measurement_r2.at<float>(4, 0) = r2_tf.transform.rotation.y;
        measurement_r2.at<float>(5, 0) = r2_tf.transform.rotation.z;
        measurement_r2.at<float>(6, 0) = r2_tf.transform.rotation.w;

        cv::Mat measurement_o1(7, 1, CV_32F);
        measurement_o1.at<float>(0, 0) = o1_tf.transform.translation.x;
        measurement_o1.at<float>(1, 0) = o1_tf.transform.translation.y;
        measurement_o1.at<float>(2, 0) = o1_tf.transform.translation.z;
        measurement_o1.at<float>(3, 0) = o1_tf.transform.rotation.x;
        measurement_o1.at<float>(4, 0) = o1_tf.transform.rotation.y;
        measurement_o1.at<float>(5, 0) = o1_tf.transform.rotation.z;
        measurement_o1.at<float>(6, 0) = o1_tf.transform.rotation.w;

        cv::Mat measurement_o2(7, 1, CV_32F);
        measurement_o2.at<float>(0, 0) = o2_tf.transform.translation.x;
        measurement_o2.at<float>(1, 0) = o2_tf.transform.translation.y;
        measurement_o2.at<float>(2, 0) = o2_tf.transform.translation.z;
        measurement_o2.at<float>(3, 0) = o2_tf.transform.rotation.x;
        measurement_o2.at<float>(4, 0) = o2_tf.transform.rotation.y;
        measurement_o2.at<float>(5, 0) = o2_tf.transform.rotation.z;
        measurement_o2.at<float>(6, 0) = o2_tf.transform.rotation.w;

        cv::Mat measurement_tj(7, 1, CV_32F);
        measurement_tj.at<float>(0, 0) = tj_tf.transform.translation.x;
        measurement_tj.at<float>(1, 0) = tj_tf.transform.translation.y;
        measurement_tj.at<float>(2, 0) = tj_tf.transform.translation.z;
        measurement_tj.at<float>(3, 0) = tj_tf.transform.rotation.x;
        measurement_tj.at<float>(4, 0) = tj_tf.transform.rotation.y;
        measurement_tj.at<float>(5, 0) = tj_tf.transform.rotation.z;
        measurement_tj.at<float>(6, 0) = tj_tf.transform.rotation.w;
        


         // Initialize state and measurement if not set yet
        if (state_cam.empty())
        {
            state_cam = cv::Mat::zeros(kf_cam.statePre.rows, kf_cam.statePre.cols, kf_cam.statePre.type());
        }
        if (state_r1.empty())
        {
            state_r1 = cv::Mat::zeros(kf_r1.statePre.rows, kf_r1.statePre.cols, kf_r1.statePre.type());
        }
        if (state_r2.empty())
        {
            state_r2 = cv::Mat::zeros(kf_r2.statePre.rows, kf_r2.statePre.cols, kf_r2.statePre.type());
        }
        if (state_o1.empty())
        {
            state_o1 = cv::Mat::zeros(kf_o1.statePre.rows, kf_o1.statePre.cols, kf_o1.statePre.type());
        }
        if (state_o2.empty())
        {
            state_o2 = cv::Mat::zeros(kf_o2.statePre.rows, kf_o2.statePre.cols, kf_o2.statePre.type());
        }
        if (state_tj.empty())
        {
            state_tj = cv::Mat::zeros(kf_tj.statePre.rows, kf_tj.statePre.cols, kf_tj.statePre.type());
        }


        // Update Kalman filter
        kf_cam.predict();
        kf_cam.correct(measurement_cam);
        kf_r1.predict();
        kf_r1.correct(measurement_r1);
        kf_r2.predict();
        kf_r2.correct(measurement_r2);
        kf_o1.predict();
        kf_o1.correct(measurement_o1);
        kf_o2.predict();
        kf_o2.correct(measurement_o2);
        kf_tj.predict();
        kf_tj.correct(measurement_tj);



        /*
        // Get filtered pose
        cv::Mat state_post_cam = kf_cam.statePost;
        camera_tf.transform.translation.x = state_post_cam.at<float>(0,0);
        camera_tf.transform.translation.y = state_post_cam.at<float>(1,0);
        camera_tf.transform.translation.z = state_post_cam.at<float>(2,0);
        camera_tf.transform.rotation.x = state_post_cam.at<float>(3,0);
        camera_tf.transform.rotation.y = state_post_cam.at<float>(4,0);
        camera_tf.transform.rotation.z = state_post_cam.at<float>(5,0);
        camera_tf.transform.rotation.w = state_post_cam.at<float>(6,0);

        cv::Mat state_post_r1 = kf_r1.statePost;
        r1_tf.transform.translation.x = state_post_r1.at<float>(0,0);
        r1_tf.transform.translation.y = state_post_r1.at<float>(1,0);
        r1_tf.transform.translation.z = state_post_r1.at<float>(2,0);
        r1_tf.transform.rotation.x = state_post_r1.at<float>(3,0);
        r1_tf.transform.rotation.y = state_post_r1.at<float>(4,0);
        r1_tf.transform.rotation.z = state_post_r1.at<float>(5,0);
        r1_tf.transform.rotation.w = state_post_r1.at<float>(6,0);

        cv::Mat state_post_r2 = kf_r2.statePost;
        r2_tf.transform.translation.x = state_post_r2.at<float>(0,0);
        r2_tf.transform.translation.y = state_post_r2.at<float>(1,0);
        r2_tf.transform.translation.z = state_post_r2.at<float>(2,0);
        r2_tf.transform.rotation.x = state_post_r2.at<float>(3,0);
        r2_tf.transform.rotation.y = state_post_r2.at<float>(4,0);
        r2_tf.transform.rotation.z = state_post_r2.at<float>(5,0);
        r2_tf.transform.rotation.w = state_post_r2.at<float>(6,0);

        cv::Mat state_post_o1 = kf_o1.statePost;
        o1_tf.transform.translation.x = state_post_o1.at<float>(0,0);
        o1_tf.transform.translation.y = state_post_o1.at<float>(1,0);
        o1_tf.transform.translation.z = state_post_o1.at<float>(2,0);
        o1_tf.transform.rotation.x = state_post_o1.at<float>(3,0);
        o1_tf.transform.rotation.y = state_post_o1.at<float>(4,0);
        o1_tf.transform.rotation.z = state_post_o1.at<float>(5,0);
        o1_tf.transform.rotation.w = state_post_o1.at<float>(6,0);

        cv::Mat state_post_o2 = kf_o2.statePost;
        o2_tf.transform.translation.x = state_post_o2.at<float>(0,0);
        o2_tf.transform.translation.y = state_post_o2.at<float>(1,0);
        o2_tf.transform.translation.z = state_post_o2.at<float>(2,0);
        o2_tf.transform.rotation.x = state_post_o2.at<float>(3,0);
        o2_tf.transform.rotation.y = state_post_o2.at<float>(4,0);
        o2_tf.transform.rotation.z = state_post_o2.at<float>(5,0);
        o2_tf.transform.rotation.w = state_post_o2.at<float>(6,0);

        cv::Mat state_post_tj = kf_tj.statePost;
        tj_tf.transform.translation.x = state_post_tj.at<float>(0,0);
        tj_tf.transform.translation.y = state_post_tj.at<float>(1,0);
        tj_tf.transform.translation.z = state_post_tj.at<float>(2,0);
        tj_tf.transform.rotation.x = state_post_tj.at<float>(3,0);
        tj_tf.transform.rotation.y = state_post_tj.at<float>(4,0);
        tj_tf.transform.rotation.z = state_post_tj.at<float>(5,0);
        tj_tf.transform.rotation.w = state_post_tj.at<float>(6,0);

        */

        // Send the transformation
            tf_broadcaster->sendTransform(camera_tf);
        // Send the transformation
            tf_broadcaster->sendTransform(r1_tf);
        // Send the transformation
            tf_broadcaster->sendTransform(r2_tf);
        // Send the transformation
            tf_broadcaster->sendTransform(o1_tf);
        // Send the transformation
            tf_broadcaster->sendTransform(o2_tf);
        // Send the transformation
            tf_broadcaster->sendTransform(tj_tf);

        //std::cout << "markerCorners = " << std::endl << " "  << markerCorners[0,0] << std::endl << std::endl;
       
      }

  
  	  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  	  cv::imshow("Display Image", img_mod);
  	  cv::waitKey(1);
      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
 
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Aruco_Detector>());
  rclcpp::shutdown();
  return 0;
}