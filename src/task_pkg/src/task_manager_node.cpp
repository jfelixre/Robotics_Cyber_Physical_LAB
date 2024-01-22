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
#include <rclcpp/executors/single_threaded_executor.hpp>


using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;

int robot_id = 0;
bool busy = true;
int robot_state = -1;
int leader_follower = 0;    //0 Leader   /   1 Follower

interfaces::msg::TaskMsg task_list;
interfaces::msg::TaskDescription selected_task;


class Task_Manager_Node : public rclcpp::Node
{
    public:
        Task_Manager_Node() : Node("task_manager_node")
        {
            selected_task.priority = 15;
            selected_task.task_id = 0;

            publisher_task_update = this->create_publisher<interfaces::msg::TaskReport>("/task_scheduler/update_task",10);
            publisher_new_task = this->create_publisher<interfaces::msg::NewTaskMsg>("/task_scheduler/new_task",10);

            

            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_" << robot_id << "/robot_state";
            std::string topic_name = ss_topic_name.str();

            subscription_robot_state = this->create_subscription<interfaces::msg::RobotState>(
                topic_name, 10, std::bind(&Task_Manager_Node::robot_state_callback, this, _1));

            subscription_task_list = this->create_subscription<interfaces::msg::TaskMsg>(
                "/task_scheduler/task_queue", 10, std::bind(&Task_Manager_Node::task_list_callback, this, _1));

             timer_ = this->create_wall_timer(
             5000ms, std::bind(&Task_Manager_Node::timer_callback, this));

            std::stringstream ss_topic_name_2;
            ss_topic_name_2 << "/robot_" << robot_id << "/task_assigned";
            std::string topic_name_2 = ss_topic_name_2.str();

            publisher_task_robot = this->create_publisher<interfaces::msg::TaskDescription>(topic_name_2,10);

            

        }

    private:

     void robot_state_callback(const interfaces::msg::RobotState::SharedPtr msg)
        {
            robot_state = msg->robot_state;

            if (robot_state == 0){
                busy=false;
            }

            else if (robot_state == 8){              //True: Robot finish task
                
                interfaces::msg::TaskReport msg_update;
                msg_update.robot_id = robot_id;
                msg_update.task_id = selected_task.task_id;
                msg_update.state = 2;
                publisher_task_update->publish(msg_update);   //Send message to update status to finish
                RCLCPP_INFO(this->get_logger(), "Task ID %d finished by Robot %d", selected_task.task_id, robot_id);
                leader_follower = 0;   //Reset leader/follower status
                selected_task.priority = 15;    //To reset task selected
                selected_task.task_id = 0;

            }
           
        }

    void task_list_callback(const interfaces::msg::TaskMsg::SharedPtr msg)
        {
            task_list.task_queue = msg->task_queue;
        }


     void timer_callback()
    {  
        if (busy == false){
            if (!task_list.task_queue.empty()){

                for (auto& task : task_list.task_queue) {
                    if (task.state==0){
                        if (selected_task.priority > task.priority){
                            if (task.priority!=0){
                                selected_task=task;
                            }
                        }
                    }
                }

                if (selected_task.task_id == 0){
                    RCLCPP_INFO(this->get_logger(), "No tasks available");
                }

                else{
                    interfaces::msg::TaskReport msg_update;
                    msg_update.robot_id = robot_id;
                    msg_update.task_id = selected_task.task_id;
                    msg_update.state = 1;
                    publisher_task_update->publish(msg_update);

                    RCLCPP_INFO(this->get_logger(), "Task ID %d selected by Robot %d", selected_task.task_id, robot_id);

                    busy = true;

                    if (selected_task.obj_size == 2){
                        if (selected_task.leader_follower == 0){

                            interfaces::msg::NewTaskMsg msg_new_task;
                            msg_new_task.priority = selected_task.priority;
                            msg_new_task.obj_id = selected_task.obj_id;
                            msg_new_task.obj_size = selected_task.obj_size;
                            msg_new_task.goal = selected_task.goal;
                            msg_new_task.leader_follower = 1;             //ask for help with task

                            publisher_new_task->publish(msg_new_task);

                        }

                        else if(selected_task.leader_follower == 1){
                            RCLCPP_INFO(this->get_logger(), "Go to help with principal task");   //Help to do principal task
                            leader_follower = 1;
                        }
                    }

                    //Send task to event_driven_control
                    publisher_task_robot->publish(selected_task);
                    
                    
                    
                }


            }

            else{
                RCLCPP_INFO(this->get_logger(), "Task list empty");
            }
           

        }
    
    }

    rclcpp::Publisher<interfaces::msg::TaskReport>::SharedPtr publisher_task_update;
    rclcpp::Publisher<interfaces::msg::NewTaskMsg>::SharedPtr publisher_new_task;
    rclcpp::Publisher<interfaces::msg::TaskDescription>::SharedPtr publisher_task_robot;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subscription_robot_state;
    rclcpp::Subscription<interfaces::msg::TaskMsg>::SharedPtr subscription_task_list;


};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);

    auto node = std::make_shared<Task_Manager_Node>();

    // // Declare the parameter with a default value
    // node->declare_parameter<int>("robot_id", 0);

    // // Retrieve the parameter value
    // robot_id = node->get_parameter("robot_id").as_int();

    // RCLCPP_INFO(node->get_logger(), "Received Robot_ID: %d", robot_id);

    //rclcpp::executors::MultiThreadedExecutor executor;
    //executor.add_node(node);
    
    //executor.spin();
    rclcpp::spin(node);
    rclcpp::shutdown();
}