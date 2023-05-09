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
#include <tf2_sensor_msgs/tf2_sensor_msgs.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <interfaces/msg/positions.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;


geometry_msgs::msg::Pose Robot1, Robot2, Object1, Object2, Target;
interfaces::msg::Positions msg_pos;

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
        try{
            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(  
                "origin_aruco_tag",
                "Robot1",
                tf2::TimePointZero);

            Robot1.position.x = transform_stamped.transform.translation.x;
            Robot1.position.y = transform_stamped.transform.translation.y;
            Robot1.position.z = transform_stamped.transform.translation.z;
            Robot1.orientation.x = transform_stamped.transform.rotation.x;
            Robot1.orientation.y = transform_stamped.transform.rotation.y;
            Robot1.orientation.z = transform_stamped.transform.rotation.z;
            Robot1.orientation.w = transform_stamped.transform.rotation.w;

        }
        catch (tf2::TransformException &ex)
        {
           
        }

        try{
            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(  
                "origin_aruco_tag",
                "Robot2",
                tf2::TimePointZero);

            Robot2.position.x = transform_stamped.transform.translation.x;
            Robot2.position.y = transform_stamped.transform.translation.y;
            Robot2.position.z = transform_stamped.transform.translation.z;
            Robot2.orientation.x = transform_stamped.transform.rotation.x;
            Robot2.orientation.y = transform_stamped.transform.rotation.y;
            Robot2.orientation.z = transform_stamped.transform.rotation.z;
            Robot2.orientation.w = transform_stamped.transform.rotation.w;
        }
        catch (tf2::TransformException &ex)
        {
           
        }

        try{
            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(  
                "origin_aruco_tag",
                "Object1",
                tf2::TimePointZero);

            Object1.position.x = transform_stamped.transform.translation.x;
            Object1.position.y = transform_stamped.transform.translation.y;
            Object1.position.z = transform_stamped.transform.translation.z;
            Object1.orientation.x = transform_stamped.transform.rotation.x;
            Object1.orientation.y = transform_stamped.transform.rotation.y;
            Object1.orientation.z = transform_stamped.transform.rotation.z;
            Object1.orientation.w = transform_stamped.transform.rotation.w;
        }
        catch (tf2::TransformException &ex)
        {
           
        }

        try{
            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(  
                "origin_aruco_tag",
                "Object2",
                tf2::TimePointZero);

            Object2.position.x = transform_stamped.transform.translation.x;
            Object2.position.y = transform_stamped.transform.translation.y;
            Object2.position.z = transform_stamped.transform.translation.z;
            Object2.orientation.x = transform_stamped.transform.rotation.x;
            Object2.orientation.y = transform_stamped.transform.rotation.y;
            Object2.orientation.z = transform_stamped.transform.rotation.z;
            Object2.orientation.w = transform_stamped.transform.rotation.w;
        }
        catch (tf2::TransformException &ex)
        {
           
        }

        try{
            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(  
                "origin_aruco_tag",
                "Target",
                tf2::TimePointZero);

            Target.position.x = transform_stamped.transform.translation.x;
            Target.position.y = transform_stamped.transform.translation.y;
            Target.position.z = transform_stamped.transform.translation.z;
            Target.orientation.x = transform_stamped.transform.rotation.x;
            Target.orientation.y = transform_stamped.transform.rotation.y;
            Target.orientation.z = transform_stamped.transform.rotation.z;
            Target.orientation.w = transform_stamped.transform.rotation.w;
        }
        catch (tf2::TransformException &ex)
        {
           
        }

        msg_pos.pos_robot1 = Robot1;
        msg_pos.pos_robot2 = Robot2;
        msg_pos.pos_object1 = Object1;
        msg_pos.pos_object2 = Object2;
        msg_pos.pos_target = Target;

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
