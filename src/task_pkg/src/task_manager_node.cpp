#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
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

#include <interfaces/msg/new_task_msg.hpp>
#include <interfaces/msg/task_report.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/task_msg.hpp>
#include <interfaces/srv/task_list_service.hpp>
#include <interfaces/msg/positions.hpp>
#include <interfaces/msg/robot_state.hpp>


using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;

int robot_id = 0;
int busy = 0;
int robot_state = 0;

class Task_Manager_Node : public rclcpp::Node
{
    public:
        Task_Manager_Node() : Node("task_manager_node")
        {
            //publisher_ = this->create_publisher<interfaces::msg::RobotState>("/positions",10);

            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_" << robot_id << "/robot_state";
            std::string topic_name = ss_topic_name.str();

            subscription_robot_state = this->create_subscription<interfaces::msg::RobotState>(
                topic_name, 10, std::bind(&Task_Manager_Node::robot_state_callback, this, _1));

             timer_ = this->create_wall_timer(
             8000ms, std::bind(&Task_Manager_Node::timer_callback, this));

            client_task = this->create_client<interfaces::srv::TaskListService>("/task_scheduler/task_list");

            

        }

    private:

     void robot_state_callback(const interfaces::msg::RobotState::SharedPtr msg)
        {
            robot_state = msg->robot_state;
            if (robot_state == 8){
                busy = 0;
            }
        }


     void timer_callback()
    {  
        if (busy == 0){
            
            if (!client_task->wait_for_service(std::chrono::seconds(1))) {
                RCLCPP_ERROR(this->get_logger(), "Service not available, waiting...");
                return;
            }

            auto request = std::make_shared<interfaces::srv::TaskListService::Request>();
            
            request->robot_id = robot_id;
            RCLCPP_INFO(this->get_logger(), "Request Robot_ID: %d", robot_id);

            auto future = client_task->async_send_request(request);
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
            auto response = future.get();

            RCLCPP_INFO(this->get_logger(), "Ready Robot_ID: %d", robot_id);

        }
    }

    //rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subscription_robot_state;
    rclcpp::Client<interfaces::srv::TaskListService>::SharedPtr client_task;

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);

    auto node = std::make_shared<Task_Manager_Node>();

    // // Declare the parameter with a default value
    // node->declare_parameter<int>("robot_id", 0);

    // // Retrieve the parameter value
    // robot_id = node->get_parameter("robot_id").as_int();

    // RCLCPP_INFO(node->get_logger(), "Received Robot_ID: %d", robot_id);

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    
    executor.spin();
    rclcpp::shutdown();
}