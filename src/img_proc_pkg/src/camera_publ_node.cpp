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

class Camera_Publ_Node : public rclcpp::Node
{
	public:
		Camera_Publ_Node() : Node("camera_publ_node")
		{
			publisher_ = this->create_publisher<sensor_msgs::msg::Image>("cameras/camera1/image_raw",10);
			//timer_ = this->create_wall_timer(16ms, std::bind(&Camera_Publ_Node::timer_callback, this));

			cv::VideoCapture cam;
			int deviceID = 2; // 0 = open default camera
			int apiID = cv::CAP_ANY; // 0 = autodetect default API
			
			cam.open(deviceID,apiID);

			 if (!cam.isOpened()) {
			std::cout << "ERROR! Unable to open camera\n";
			cam.release();
			rclcpp::shutdown();
			

			}
			else{
				while(true){
					cv::Mat image;
					cam.read(image);

					cv_bridge::CvImage image_bridge(std_msgs::msg::Header(),RGB8,image);
					image_bridge.image=image;
					sensor_msgs::msg::Image::SharedPtr msg = image_bridge.toImageMsg();

					RCLCPP_INFO(this->get_logger(), "Publishing image");
					publisher_->publish(*msg);

				}
			}
			cam.release();
			rclcpp::shutdown();

		}

	private:
		// void timer_callback()
		// {
		// 	cv::Mat image;
			
		// 	cam.read(image);


		// 	//sensor_msgs::msg::Image image_message;
		// 	cv_bridge::CvImage image_bridge(std_msgs::msg::Header(),RGB8,image);
			
		// 	image_bridge.image=image;
		// 	sensor_msgs::msg::Image::SharedPtr msg = image_bridge.toImageMsg();

		// 	//cv_bridge::CvImagePtr & image_message=image_bridge.toImageMsg;

		// 	//& image_message=image_bridge.toImageMsg();	
      	// 	//message = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
      	// 	RCLCPP_INFO(this->get_logger(), "Publishing image");
      	// 	publisher_->publish(*msg);
		// }

		//rclcpp::TimerBase::SharedPtr timer_;
   		rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
};



int main(int argc, char ** argv)
{
  //(void) argc;
  //(void) argv;
	
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<Camera_Publ_Node>());
	rclcpp::shutdown();

	return 0;


}