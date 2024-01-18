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
#include <interfaces/msg/position_tag.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;


geometry_msgs::msg::Pose tag_pos;
interfaces::msg::Positions msg_pos;
interfaces::msg::PositionTag tag;

class Compute_Position : public rclcpp::Node
{
    public:
        Compute_Position() : Node("compute_position")
        {
            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            publisher_ = this->create_publisher<interfaces::msg::Positions>("positions",10);

             timer_ = this->create_wall_timer(
             50ms, std::bind(&Compute_Position::timer_callback, this));

        }

    private:

     void timer_callback()
    {
        for (int i = 1; i<=20; i++){
            try{
            std::stringstream ss_frame_name;
            ss_frame_name << "tag_" << i;
            std::string frame_name = ss_frame_name.str();

            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(  
                "origin_aruco_tag",
                frame_name,
                tf2::TimePointZero);

            tag_pos.position.x = transform_stamped.transform.translation.x;
            tag_pos.position.y = transform_stamped.transform.translation.y;
            tag_pos.position.z = transform_stamped.transform.translation.z;
            tag_pos.orientation.x = transform_stamped.transform.rotation.x;
            tag_pos.orientation.y = transform_stamped.transform.rotation.y;
            tag_pos.orientation.z = transform_stamped.transform.rotation.z;
            tag_pos.orientation.w = transform_stamped.transform.rotation.w;

            tag.position = tag_pos;
            tag.tag_id=i;

            msg_pos.tag_pos.push_back(tag);

            }
            catch (tf2::TransformException &ex)
            {
            }
           
        }

        publisher_->publish(msg_pos);
    }


    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<interfaces::msg::Positions>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Compute_Position>());
    rclcpp::shutdown();
    return 0;
}
