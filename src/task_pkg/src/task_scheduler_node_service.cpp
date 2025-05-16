#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/new_task_msg.hpp>
#include <interfaces/msg/task_report.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/task_msg.hpp>
#include <interfaces/srv/task_list_service.hpp>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;

interfaces::msg::TaskDescription tasks;
interfaces::msg::TaskMsg task_list;
int32_t task_id_count = 0;

/*class Task_Scheduler_Node : public rclcpp::Node
{
	public:
		Task_Scheduler_Node() : Node("task_scheduler_node")
		{
     		publisher_test = this->create_publisher<interfaces::msg::TaskMsg>("/taks_scheduler/task_queue",1);
			timer_ = this->create_wall_timer(1000ms, std::bind(&Task_Scheduler_Node::timer_callback, this));
			
		}

	private:

		void timer_callback()
			{
			publisher_test->publish(task_list);
			}

		rclcpp::Publisher<interfaces::msg::TaskMsg>::SharedPtr publisher_test;
		rclcpp::TimerBase::SharedPtr timer_;
		

};
*/
class Reg_Tasks_Node : public rclcpp::Node
{
	public:
		Reg_Tasks_Node() : Node("reg_tasks_node")
		{
     		
     		new_task_subscriber = create_subscription<interfaces::msg::NewTaskMsg>(
        "/task_scheduler/new_task", 1, std::bind(&Reg_Tasks_Node::new_task_subs,this,_1));

			update_task_subscriber = create_subscription<interfaces::msg::TaskReport>(
        "/task_scheduler/update_task", 1, std::bind(&Reg_Tasks_Node::update_task_subs,this,_1));

		publisher_test = this->create_publisher<interfaces::msg::TaskMsg>("/task_scheduler/task_queue",1);
		}

	private:

		void new_task_subs(const interfaces::msg::NewTaskMsg::SharedPtr msg)
		{	
			task_id_count++;
			interfaces::msg::TaskDescription temp_task;
  			temp_task.task_id = task_id_count;   //Set id task in order
			temp_task.obj_id = msg->obj_id;
			if(temp_task.obj_id < 20){
				temp_task.obj_size=1;
			}
			else{
				temp_task.obj_size=2;
			}
			//temp_task.obj_size = msg->obj_size;
			temp_task.goal = msg->goal;
			temp_task.angle_goal = msg->angle_goal;
			temp_task.leader_robot_id = msg->leader_robot_id;
			temp_task.priority = msg->priority;
			temp_task.state = 0;  //Set state to pending
			rclcpp::Time time = Node::get_clock()->now();
			temp_task.idle_time_start = time.seconds(); //Initial time

			task_list.task_queue.push_back(temp_task);

			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "New task registered");

			publisher_test->publish(task_list);
			
		}

		void update_task_subs(const interfaces::msg::TaskReport::SharedPtr msg)
			{	
				
				int task_to_update = msg->task_id;
				rclcpp::Time time = Node::get_clock()->now();

				for (auto& task : task_list.task_queue) {
					if (task.task_id == task_to_update) {  
						task.state = msg->state;
						task.robot_id = msg->robot_id;

						if (task.obj_size == 2 && task.leader_robot_id == 0){
							task.leader_robot_id = task.robot_id;
						}
						

						if (msg->state==1){
							task.in_progress_time_start = time.seconds(); //Initial in progress time
							task.idle_time = task.in_progress_time_start - task.idle_time_start;
						}

						else if (msg->state==2){
							task.finish_time = time.seconds();
							task.task_time = task.finish_time - task.in_progress_time_start;
							task.total_time = task.idle_time+task.task_time;
						}

						break;  
					}
				}

				RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Task updated");

				publisher_test->publish(task_list);
			}

		rclcpp::Subscription<interfaces::msg::NewTaskMsg>::SharedPtr new_task_subscriber;
		rclcpp::Subscription<interfaces::msg::TaskReport>::SharedPtr update_task_subscriber;
		rclcpp::Publisher<interfaces::msg::TaskMsg>::SharedPtr publisher_test;


};

void list_handler(const std::shared_ptr<interfaces::srv::TaskListService::Request> request,
			std::shared_ptr<interfaces::srv::TaskListService::Response> response){
				response->task_list = task_list.task_queue;
			}

void assign_task_handler(const std::shared_ptr<interfaces::srv::TaskListService::Request> request,
                         std::shared_ptr<interfaces::srv::TaskListService::Response> response) {
    for (auto& task : task_list.task_queue) {
        if (task.state == 0) {  // Find the first pending task
            task.state = 1;  // Mark task as in progress
            task.robot_id = request->robot_id;  // Assign robot ID
            rclcpp::Time time = rclcpp::Clock().now();
            task.in_progress_time_start = time.seconds();  // Set in-progress start time
            response->task_list.push_back(task);  // Add task to response
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Task assigned to robot %d", request->robot_id);
            break;
        }
    }
    if (response->task_list.empty()) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "No pending tasks available for robot %d", request->robot_id);
    }
}

int main(int argc, char * argv[])
{
	rclcpp::init(argc, argv);

  

	//auto task_scheduler_node = std::make_shared<Task_Scheduler_Node>();
	auto reg_tasks_node = std::make_shared<Reg_Tasks_Node>();
	
	std::shared_ptr<rclcpp::Node> task_list_node = rclcpp::Node::make_shared("task_list_server");
	rclcpp::Service<interfaces::srv::TaskListService>::SharedPtr service=
		task_list_node->create_service<interfaces::srv::TaskListService>("/task_scheduler/task_list", &list_handler);

    std::shared_ptr<rclcpp::Node> task_scheduler_service = rclcpp::Node::make_shared("task_scheduler_service");
    rclcpp::Service<interfaces::srv::TaskListService>::SharedPtr assign_service =
        task_scheduler_service->create_service<interfaces::srv::TaskListService>("/task_scheduler/assign_task", &assign_task_handler);

  rclcpp::executors::MultiThreadedExecutor executor;
  //executor.add_node(task_scheduler_node);
  executor.add_node(reg_tasks_node);
  executor.add_node(task_list_node);
  executor.add_node(task_scheduler_service);
//executor.add_node(node_subs_state_r2);
  executor.spin();

 	rclcpp::shutdown();
  
}



