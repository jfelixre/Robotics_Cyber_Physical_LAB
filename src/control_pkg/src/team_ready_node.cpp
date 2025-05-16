#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/msg/arm_objective.hpp>
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/waiting_team.hpp>
#include <interfaces/srv/robot_status.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Matrix3x3.h>
#include <memory>
#include <string>
#include <sstream>
#include <cmath>
#include <std_msgs/msg/bool.hpp>

class TeamStatusSubscriber : public rclcpp::Node {
public:
    TeamStatusSubscriber() : Node("team_status_subscriber"), robot_01_ready(false), robot_02_ready(false) {
        sub_robot_01 = this->create_subscription<interfaces::msg::WaitingTeam>(
            "/robot_01/waiting_team", 10, std::bind(&TeamStatusSubscriber::robot01_callback, this, std::placeholders::_1));

        sub_robot_02 = this->create_subscription<interfaces::msg::WaitingTeam>(
            "/robot_02/waiting_team", 10, std::bind(&TeamStatusSubscriber::robot02_callback, this, std::placeholders::_1));
    }

    bool is_team_ready() const {
        return robot_01_ready && robot_02_ready;
    }

private:
    void robot01_callback(const interfaces::msg::WaitingTeam::SharedPtr msg) {
        robot_01_ready = msg->waiting_team;
        RCLCPP_INFO(this->get_logger(), "Robot 01 ready: %s", robot_01_ready ? "true" : "false");
    }

    void robot02_callback(const interfaces::msg::WaitingTeam::SharedPtr msg) {
        robot_02_ready = msg->waiting_team;
        RCLCPP_INFO(this->get_logger(), "Robot 02 ready: %s", robot_02_ready ? "true" : "false");
    }

    bool robot_01_ready;
    bool robot_02_ready;

    rclcpp::Subscription<interfaces::msg::WaitingTeam>::SharedPtr sub_robot_01;
    rclcpp::Subscription<interfaces::msg::WaitingTeam>::SharedPtr sub_robot_02;
};

class TeamReadyPublisher : public rclcpp::Node {
public:
    TeamReadyPublisher(std::shared_ptr<TeamStatusSubscriber> team_status_subscriber)
        : Node("team_ready_publisher"), team_status_subscriber_(team_status_subscriber) {
        pub_team_ready = this->create_publisher<std_msgs::msg::Bool>("/team_ready", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&TeamReadyPublisher::timer_callback, this));
    }

private:
    void timer_callback() {
        auto msg = std_msgs::msg::Bool();
        msg.data = team_status_subscriber_->is_team_ready();
        if (msg.data){
            pub_team_ready->publish(msg);
        }
       // RCLCPP_INFO(this->get_logger(), "Team ready: %s", msg.data ? "true" : "false");
    }

    std::shared_ptr<TeamStatusSubscriber> team_status_subscriber_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_team_ready;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);

    auto team_status_subscriber = std::make_shared<TeamStatusSubscriber>();
    auto team_ready_publisher = std::make_shared<TeamReadyPublisher>(team_status_subscriber);

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(team_status_subscriber);
    executor.add_node(team_ready_publisher);

    executor.spin();
    rclcpp::shutdown();
    return 0;
}
