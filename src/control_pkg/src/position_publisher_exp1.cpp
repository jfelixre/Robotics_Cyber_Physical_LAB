#include "rclcpp/rclcpp.hpp"
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>

class PositionPublisherExp1 : public rclcpp::Node {
public:
    PositionPublisherExp1() : Node("position_publisher_exp1") {
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        publisher_robot_01_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("/robot_01/position", 10);
        publisher_robot_02_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("/robot_02/position", 10);

        timer_ = this->create_wall_timer(std::chrono::milliseconds(200), std::bind(&PositionPublisherExp1::publish_positions, this));
    }

private:
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_robot_01_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_robot_02_;
    rclcpp::TimerBase::SharedPtr timer_;

    void publish_positions() {
        publish_position("marker_id_01", publisher_robot_01_);
        publish_position("robot_01/base_link", publisher_robot_01_);
        publish_position("robot_01/gr_ref_link", publisher_robot_01_);
        publish_position("marker_id_02", publisher_robot_01_);
        publish_position("robot_02/base_link", publisher_robot_01_);
        publish_position("robot_02/gr_ref_link", publisher_robot_01_);
        publish_position("marker_id_11", publisher_robot_01_);
        publish_position("cube_id_11/cube_link", publisher_robot_01_);
        publish_position("marker_id_12", publisher_robot_01_);
        publish_position("cube_id_12/cube_link", publisher_robot_01_);
        
    }

    void publish_position(const std::string &child_frame, const rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr &publisher) {
        try {
            geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", child_frame, tf2::TimePointZero);

            geometry_msgs::msg::PoseStamped pose;
            pose.header.stamp = transform.header.stamp;
            pose.header.frame_id = "marker_id_00";
            pose.pose.position.x = transform.transform.translation.x;
            pose.pose.position.y = transform.transform.translation.y;
            pose.pose.position.z = transform.transform.translation.z;
            pose.pose.orientation = transform.transform.rotation;

            publisher->publish(pose);
        } catch (const tf2::TransformException &ex) {
            RCLCPP_WARN(this->get_logger(), "Could not transform %s: %s", child_frame.c_str(), ex.what());
        }
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PositionPublisherExp1>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
