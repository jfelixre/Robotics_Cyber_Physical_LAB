#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/image_encodings.hpp>
//#include <std_msgs/msg/bool.hpp>
//#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

using std::placeholders::_1;


using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;
namespace enc = sensor_msgs::image_encodings;

cv::Mat image1;

class Img_Subs_Test_Node : public rclcpp::Node
{
  public:
    Img_Subs_Test_Node() : Node("img_subs_test_node")
    {
      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "demo_cam/camera1/image_raw", 10, std::bind(&Img_Subs_Test_Node::topic_callback, this, _1));
      //topic "imagen_original" para img_publ_test_node
      //topic "demo_cam/camera1/image_raw" para gazebo camera_test.world
    }

  private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg) const
    {
      RCLCPP_INFO(this->get_logger(), "Received image" );
      
      cv_bridge::CvImageConstPtr image_bridge;
	  //cv_bridge::toCvCopy(msg, RGB8)->image;
      image_bridge=cv_bridge::toCvCopy(msg);
      image1 = image_bridge->image;
      cv::cvtColor(image1,image1,cv::COLOR_BGR2RGB);
      cv::imwrite("image.jpg",image1);

	  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
	  cv::imshow("Display Image", image1);
	  cv::waitKey(1);
      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<Img_Subs_Test_Node>());



  rclcpp::shutdown();



  return 0;
}