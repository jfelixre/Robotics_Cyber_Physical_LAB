#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/waiting_team.hpp>
#include <interfaces/srv/robot_status.hpp>
#include <sstream>
#include <string>

using namespace std::chrono_literals;

class RobotStateService : public rclcpp::Node
{
public:
    RobotStateService() : Node("robot_state_service")
    {
        this->declare_parameter<int>("robot_id", 0);
        robot_id = this->get_parameter("robot_id").as_int();
        RCLCPP_INFO(this->get_logger(), "Robot State Service initialized for Robot_ID: %d", robot_id);

        // Create topic names dynamically based on robot_id
        std::stringstream ss_state_topic, ss_waiting_topic;
        ss_state_topic << "/robot_0" << robot_id << "/robot_state";
        ss_waiting_topic << "/robot_0" << robot_id << "/waiting_team";

        // Subscribers
        subscription_robot_state = this->create_subscription<interfaces::msg::RobotState>(
            ss_state_topic.str(), 10, std::bind(&RobotStateService::robot_state_callback, this, std::placeholders::_1));

        subscription_waiting_team = this->create_subscription<interfaces::msg::WaitingTeam>(
            ss_waiting_topic.str(), 10, std::bind(&RobotStateService::waiting_team_callback, this, std::placeholders::_1));

        // Service
        service_robot_status = this->create_service<interfaces::srv::RobotStatus>(
            "/robot_0" + std::to_string(robot_id) + "/robot_status",
            std::bind(&RobotStateService::handle_robot_status_request, this, std::placeholders::_1, std::placeholders::_2));
    }

private:
    void robot_state_callback(const interfaces::msg::RobotState::SharedPtr msg)
    {
        robot_state = msg->robot_state;
        RCLCPP_INFO(this->get_logger(), "Received Robot State: %d", robot_state);
    }

    void waiting_team_callback(const interfaces::msg::WaitingTeam::SharedPtr msg)
    {
        waiting_team = msg->waiting_team;
        RCLCPP_INFO(this->get_logger(), "Received Waiting Team Status: %s", waiting_team ? "true" : "false");
    }

    void handle_robot_status_request(
        const std::shared_ptr<interfaces::srv::RobotStatus::Request> request,
        std::shared_ptr<interfaces::srv::RobotStatus::Response> response)
    {
        (void)request; // Unused
        response->robot_state = robot_state;
        response->waiting_team = waiting_team;
        RCLCPP_INFO(this->get_logger(), "Responded with Robot State: %d, Waiting Team: %s",
                    robot_state, waiting_team ? "true" : "false");
    }

    int robot_id;
    int robot_state = 0;
    bool waiting_team = false;

    rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subscription_robot_state;
    rclcpp::Subscription<interfaces::msg::WaitingTeam>::SharedPtr subscription_waiting_team;
    rclcpp::Service<interfaces::srv::RobotStatus>::SharedPtr service_robot_status;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotStateService>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
