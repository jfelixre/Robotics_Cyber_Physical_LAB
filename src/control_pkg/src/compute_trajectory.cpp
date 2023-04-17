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
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <stdio.h>

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <geometry_msgs/msg/pose.hpp>
#include <interfaces/msg/positions.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;

geometry_msgs::msg::Pose Robot1, Robot2, Object1, Object2, Target;

float x_grid = 0.025;  //All dimensions in meters
float y_grid = 0.025;
float x_world = 3;        
float y_world = 3;          

int n_x_spaces = (int)x_world/x_grid;
float n_y_spaces = (int)y_world/y_grid;




class Compute_Trajectory : public rclcpp::Node
{
    public:
        Compute_Trajectory() : Node("compute_trajectory")
        {
           

            subs_position = this->create_subscription<interfaces::msg::Positions>(
                "/positions", 1, std::bind(&Compute_Trajectory::subs_callback,this,_1));

            std::cout<<n_x_spaces<< std::endl;
        }
    

    private:


        void subs_callback(const interfaces::msg::Positions::SharedPtr pos_msg)
        {

            cv::Mat map = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_bin = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);

             map = cv::Scalar(255);


            Robot1=pos_msg->pos_robot1;
            Robot2=pos_msg->pos_robot2;
            Object1=pos_msg->pos_object1;
            Object2=pos_msg->pos_object2;
            Target=pos_msg->pos_target;




            //ROBOT 1
            int R1_x_map = ((int)((Robot1.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int R1_y_map = 120 - (((int)((Robot1.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            
            cv::rectangle(map,cv::Point(R1_x_map-2,R1_y_map+2),cv::Point(R1_x_map+2,R1_y_map-2), cv::Scalar(0), cv::FILLED);

            std::cout<<R1_x_map<< std::endl;
            std::cout<<R1_y_map<< std::endl;


            //ROBOT 2






            //OBJECT 1






            //OBJECT 2






            //TARGET






            cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
            cv::imshow("Display Image", map);
            cv::waitKey(1);

        }







    rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subs_position;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Compute_Trajectory>());
    rclcpp::shutdown();
    return 0;
}