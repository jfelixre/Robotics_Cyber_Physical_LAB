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
#include <stdio.h>

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <geometry_msgs/msg/pose.hpp>
#include <interfaces/msg/positions.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;

geometry_msgs::msg::Pose Robot1, Robot2, Object1, Object2, Target;

float x_grid = 0.10;  //All dimensions in meters
float y_grid = 0.10;
float x_world = 3.1;            //Set a number that is odd after dividing it by x_grid.
float y_world = 3.1;            //Set a number that is ood after dividing it by y_grid.

int n_x_spaces = (int)x_world/x_grid;
float n_y_spaces = y_world/y_grid;

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
            Robot1=pos_msg->pos_robot1;
            Robot2=pos_msg->pos_robot2;
            Object1=pos_msg->pos_object1;
            Object2=pos_msg->pos_object2;
            Target=pos_msg->pos_target;

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