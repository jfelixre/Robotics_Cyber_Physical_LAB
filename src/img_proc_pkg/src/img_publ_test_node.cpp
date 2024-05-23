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



using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;

//typedef boost::shared_ptr< ::sensor_msgs::Image> ImagePtr;

class Img_Publ_Test_Node : public rclcpp::Node
{
	public:
		Img_Publ_Test_Node() : Node("img_publ_test_node")
		{
			publisher_ = this->create_publisher<sensor_msgs::msg::Image>("original_image",10);
			timer_ = this->create_wall_timer(16ms, std::bind(&Img_Publ_Test_Node::timer_callback, this));
		}

	private:
		void timer_callback()
		{
			cv::Mat image = cv::imread("/home/javierfr/Downloads/image.jpeg");

			//sensor_msgs::msg::Image image_message;
			cv_bridge::CvImage image_bridge(std_msgs::msg::Header(),RGB8,image);
			
			image_bridge.image=image;
			sensor_msgs::msg::Image::SharedPtr msg = image_bridge.toImageMsg();

			//cv_bridge::CvImagePtr & image_message=image_bridge.toImageMsg;

			//& image_message=image_bridge.toImageMsg();	
      		//message = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
      		RCLCPP_INFO(this->get_logger(), "Publishing image");
      		publisher_->publish(*msg);
		}

		rclcpp::TimerBase::SharedPtr timer_;
   		rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
};



int main(int argc, char ** argv)
{
  //(void) argc;
  //(void) argv;

	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<Img_Publ_Test_Node>());
	rclcpp::shutdown();
	return 0;


}