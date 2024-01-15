#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/new_task_msg.hpp>
#include <interfaces/msg/task_report.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/task_msg.hpp>

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
int task_id_count = 0;

class Task_Scheduler_Node : public rclcpp::Node
{
	public:
		Task_Scheduler_Node() : Node("task_scheduler_node")
		{
     		publisher_test = this->create_publisher<interfaces::msg::TaskMsg>("/taks_scheduler/task_queue",1);
			timer_ = this->create_wall_timer(2000ms, std::bind(&Task_Scheduler_Node::timer_callback, this));
		}

	private:

		void timer_callback()
			{
			publisher_test->publish(task_list);
			}


		rclcpp::Publisher<interfaces::msg::TaskMsg>::SharedPtr publisher_test;
		rclcpp::TimerBase::SharedPtr timer_;

};

class Reg_Tasks_Node : public rclcpp::Node
{
	public:
		Reg_Tasks_Node() : Node("reg_tasks_node")
		{
     		
     		new_task_subscriber = create_subscription<interfaces::msg::NewTaskMsg>(
        "/task_scheduler/new_task", 1, std::bind(&Reg_Tasks_Node::new_task_subs,this,_1));

			update_task_subscriber = create_subscription<interfaces::msg::TaskReport>(
        "/task_scheduler/update_task", 1, std::bind(&Reg_Tasks_Node::update_task_subs,this,_1));
		}

	private:

		void new_task_subs(const interfaces::msg::NewTaskMsg::SharedPtr msg)
		{	
			task_id_count++;
			interfaces::msg::TaskDescription temp_task;
  			temp_task.task_id = task_id_count;   //Set id task in order
			temp_task.obj_id = msg->obj_id;
			temp_task.obj_size = msg->obj_size;
			temp_task.goal = msg->goal;
			temp_task.leader_follower = msg->leader_follower;
			temp_task.priority = msg->priority;
			temp_task.state = 0;  //Set state to pending
			rclcpp::Time time = Node::get_clock()->now();
			temp_task.idle_time_start = time.seconds(); //Initial time

			task_list.task_queue.push_back(temp_task);
			
		}

		void update_task_subs(const interfaces::msg::TaskReport::SharedPtr msg)
			{	
				
				int task_to_update = msg->task_id;
				rclcpp::Time time = Node::get_clock()->now();

				for (auto& task : task_list.task_queue) {
					if (task.task_id == task_to_update) {  
						task.state = msg->state;
						task.robot_id = msg->robot_id;

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
			}

		rclcpp::Subscription<interfaces::msg::NewTaskMsg>::SharedPtr new_task_subscriber;
		rclcpp::Subscription<interfaces::msg::TaskReport>::SharedPtr update_task_subscriber;


};



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  

	auto task_scheduler_node = std::make_shared<Task_Scheduler_Node>();
  auto reg_tasks_node = std::make_shared<Reg_Tasks_Node>();
	//auto node_subs_state_r2 = std::make_shared<Node_Subs_State_R2>();

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(task_scheduler_node);
  executor.add_node(reg_tasks_node);
	//executor.add_node(node_subs_state_r2);
  executor.spin();

 	rclcpp::shutdown();
  
}