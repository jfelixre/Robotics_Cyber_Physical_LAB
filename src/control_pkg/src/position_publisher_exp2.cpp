#include "rclcpp/rclcpp.hpp"
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

class PositionPublisherExp2 : public rclcpp::Node {
public:
    PositionPublisherExp2() : Node("position_publisher_exp2") {
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        publisher_marker_id_01_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/marker/id_01", 10);
        publisher_robot_01_base_link_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/robot_01/base_link", 10);
        publisher_robot_01_gr_ref_link_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/robot_01/gr_ref_link", 10);

        publisher_marker_id_02_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/marker/id_02", 10);
        publisher_robot_02_base_link_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/robot_02/base_link", 10);
        publisher_robot_02_gr_ref_link_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/robot_02/gr_ref_link", 10);

        publisher_marker_id_21_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/marker/id_21", 10);
        publisher_cube_id_21_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/positions/cube/id_21", 10);

        timer_ = this->create_wall_timer(std::chrono::milliseconds(200), std::bind(&PositionPublisherExp2::publish_positions, this));
    }

private:
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_marker_id_01_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_robot_01_base_link_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_robot_01_gr_ref_link_;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_marker_id_02_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_robot_02_base_link_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_robot_02_gr_ref_link_;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_marker_id_21_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_cube_id_21_;


    rclcpp::TimerBase::SharedPtr timer_;

    void publish_positions() {
        publish_position("marker_id_01", publisher_marker_id_01_);
        publish_position("robot_01/base_link", publisher_robot_01_base_link_);
        publish_position("robot_01/gr_ref_link", publisher_robot_01_gr_ref_link_);

        publish_position("marker_id_02", publisher_marker_id_02_);
        publish_position("robot_02/base_link", publisher_robot_02_base_link_);
        publish_position("robot_02/gr_ref_link", publisher_robot_02_gr_ref_link_);

        publish_position("marker_id_21", publisher_marker_id_21_);
        publish_position("cube_id_21/cube_link", publisher_cube_id_21_);
        
    }

    void publish_position(const std::string &child_frame, const rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr &publisher) {
        try {
            geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", child_frame, tf2::TimePointZero);

            double roll, pitch, yaw;
            tf2::Quaternion quat(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
            tf2::Matrix3x3(quat).getRPY(roll, pitch, yaw);

            std_msgs::msg::Float32MultiArray position_msg;
            position_msg.data = {static_cast<float>(transform.transform.translation.x),
                                 static_cast<float>(transform.transform.translation.y),
                                 static_cast<float>(yaw)};

            publisher->publish(position_msg);
        } catch (const tf2::TransformException &ex) {
            RCLCPP_WARN(this->get_logger(), "Could not transform %s: %s", child_frame.c_str(), ex.what());
        }
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PositionPublisherExp2>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
