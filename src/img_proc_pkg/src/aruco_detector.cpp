#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>

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

using std::placeholders::_1;


using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;
namespace enc = sensor_msgs::image_encodings;

cv::Mat img_original;
cv::Mat img_mod;
cv::Mat cameraMatrix;
cv::Mat distCoeffs;




class Aruco_Detector : public rclcpp::Node
{
  public:
    Aruco_Detector() : Node("aruco_detector")
    {
      bool readOk = readCameraParameters("src/img_proc_pkg/config/camera_calib_charuco.yaml", cameraMatrix, distCoeffs);

      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "/image_raw", 10, std::bind(&Aruco_Detector::topic_callback, this, _1));
      //publisher = this->create_publisher<interfaces::msg::ImgData>("image_data",1);

    }

  private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg) const
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
      cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
      cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
      cv::aruco::detectMarkers(img_original, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);

      if (markerIds.size() > 0){

        cv::aruco::drawDetectedMarkers(img_mod, markerCorners, markerIds); 

        std::vector<cv::Vec3d> rvecs, tvecs;
        cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
 
        for (int i = 0; i < rvecs.size(); ++i) {
          auto rvec = rvecs[i];
          auto tvec = tvecs[i];
          cv::drawFrameAxes(img_mod, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
        }
      }

  
  	  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  	  cv::imshow("Display Image", img_mod);
  	  cv::waitKey(1);
      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    //rclcpp::Publisher<interfaces::msg::ImgData>::SharedPtr publisher;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Aruco_Detector>());
  rclcpp::shutdown();
  return 0;
}