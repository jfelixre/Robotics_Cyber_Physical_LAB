#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/srv/event_control.hpp>
#include <interfaces/msg/arm_joints_positions.hpp>
#include <interfaces/msg/trajectory_control.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/robot_objective.hpp>

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

int robot_id = 0;
interfaces::msg::TaskDescription task;
interfaces::msg::Positions tags_positions;

interfaces::msg::RobotState robot_state;

float Xobj, Yobj, Angobj;

interfaces::msg::RobotObjective objective;

class Event_Driven_Control : public rclcpp::Node
{
	public:
		Event_Driven_Control() : Node("event_driven_control")
		{
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_" << robot_id << "/task_assigned";
            std::string topic_name = ss_topic_name.str();

            subscription_task_robot = this->create_subscription<interfaces::msg::TaskDescription>(
                topic_name, 10, std::bind(&Event_Driven_Control::task_robot_callback, this, _1));

            subscription_positions = this->create_subscription<interfaces::msg::Positions>(
                "positions", 10, std::bind(&Event_Driven_Control::positions_callback, this, _1));

            
            std::stringstream ss_topic_name_2;
            ss_topic_name_2 << "/robot_" << robot_id << "/robot_state";
            std::string topic_name_2 = ss_topic_name_2.str();

            publisher_robot_state = this->create_publisher<interfaces::msg::RobotState>(topic_name_2,1);
            
            robot_state.robot_state = 0;

            publisher_robot_state -> publish(robot_state);


            std::stringstream ss_topic_name_3;
            ss_topic_name_3 << "/robot_" << robot_id << "/control_finish";
            std::string topic_name_3 = ss_topic_name_3.str();


            subscription_control_finish = this->create_subscription<interfaces::msg::ControlFinish>(
                topic_name_3, 10, std::bind(&Event_Driven_Control::control_finish_callback, this, _1));


            std::stringstream ss_topic_name_4;
            ss_topic_name_4 << "/robot_" << robot_id << "/objective";
            std::string topic_name_4 = ss_topic_name_4.str();

            publisher_robot_objective = create_publisher<interfaces::msg::RobotObjective>(topic_name_4, 1);

		}


	private:

        void task_robot_callback(interfaces::msg::TaskDescription::SharedPtr msg)
            {
                task = *msg;
                RCLCPP_INFO(this->get_logger(), "Task ID %d received by event_control of Robot %d", task.task_id, robot_id);

                robot_state.robot_state = 1;
                publisher_robot_state -> publish(robot_state);

                event_control(); //Call for new step in event driven control
            }

        void positions_callback(interfaces::msg::Positions::SharedPtr msg)
            {
                tags_positions = *msg;
                
            }

        void control_finish_callback(interfaces::msg::ControlFinish::SharedPtr msg)
            {
                if (msg->finish_confirm == 1){      //Go to next state
                    robot_state.robot_state++;
                    publisher_robot_state -> publish(robot_state);
                }

                event_control();            //Call for new step in event driven control
                
            }

        void event_control(){

            for (auto& tag_pos : tags_positions.tag_pos) {                   //Save object position
                    if (tag_pos.tag_id == task.obj_id){
                        Xobj = tag_pos.position.position.x;
                        Yobj = tag_pos.position.position.y;
                        tf2::Quaternion Obj_quat(tag_pos.position.orientation.x, tag_pos.position.orientation.y, tag_pos.position.orientation.z, tag_pos.position.orientation.w);
                        tf2::Matrix3x3 Obj_m(Obj_quat);
                        double Obj_orientation_x, Obj_orientation_y, Obj_orientation_z;
                        Obj_m.getRPY(Obj_orientation_x, Obj_orientation_y, Obj_orientation_z);
                        Angobj= Obj_orientation_z;
                    }
                }

            if (task.leader_robot_id == 0){
                switch(robot_state.robot_state){   //CHECK CASE WHEN OBJECT SIZE IS 2, must take object from different angle

                    case 0:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d waiting for a new task", robot_id);
                        break;

                    case 1:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 1 Approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj - 2;
                        objective.angle = Angobj;       //
                        publisher_robot_objective->publish(objective);
                        break;

                    case 2:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 2 Last approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj - 2;
                        objective.angle = Angobj;       //
                        publisher_robot_objective->publish(objective);
                        break;

                    case 3:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 3 Taking object %d", robot_id, task.obj_id);
                        //COMPLETE OBJECT PICK
                        break;

                    case 4:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 4 Approach to objective point, x= %d, y= %d", robot_id, task.goal.x, task.goal.y);

                        objective.point.x = task.goal.x - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = task.goal.y - 2;
                        objective.angle = 0;       // Define if i can select goal angle
                        publisher_robot_objective->publish(objective);
                        break;

                    
                    case 5:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 5 Placing object on point, x= %d, y= %d", robot_id, task.goal.x, task.goal.y);

                        //COMPLETE OBJECT PLACE
                        break;

                    case 6:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 6 Leaving object", robot_id);

                        objective.point.x = task.goal.x - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = task.goal.y - 2;
                        objective.angle = 0;       // Define if i can select goal angle
                        publisher_robot_objective->publish(objective);
                        break;

                    case 7:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 7 Back to home position", robot_id);

                        objective.point.x = 0;   //Define home position***
                        objective.point.y = 0;
                        objective.angle = 0;
                        publisher_robot_objective->publish(objective);
                        break;

                    case 8:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 8 Task_ID %d Finished", robot_id, task.task_id);

                        robot_state.robot_state = 0;
                        publisher_robot_state -> publish(robot_state);
                        break;
                }
            }

            else {
                switch(robot_state.robot_state){  //CHECK CASE 1, 2 AND 3, to take object from different angle ??

                    case 0:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d waiting for a new task", robot_id);
                        break;

                    case 1:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 1 Approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj - 2;
                        objective.angle = Angobj;       //
                        publisher_robot_objective->publish(objective);
                        break;

                    case 2:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 2 Last approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj - 2;
                        objective.angle = Angobj;       //
                        publisher_robot_objective->publish(objective);
                        break;

                    case 3:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 3 Taking object %d", robot_id, task.obj_id);
                        //COMPLETE OBJECT PICK
                        break;

                    case 4:
                        //MIRROR CONTROL OF LEADER ROBOT
                        break;

                    
                    case 5:
                        //MIRROR CONTROL OF LEADER ROBOT
                        break;

                    case 6:
                        //MIRROR CONTROL OF LEADER ROBOT
                        break;

                    case 7:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 7 Back to home position", robot_id);

                        objective.point.x = 0;   //Define home position***
                        objective.point.y = 0;
                        objective.angle = 0;
                        publisher_robot_objective->publish(objective);
                        break;

                    case 8:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 8 Task_ID %d Finished", robot_id, task.task_id);

                        robot_state.robot_state = 0;
                        publisher_robot_state -> publish(robot_state);
                        break;
                }
            }

        }

        rclcpp::Subscription<interfaces::msg::TaskDescription>::SharedPtr subscription_task_robot;
        rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subscription_positions;
        rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr publisher_robot_state;
        rclcpp::Subscription<interfaces::msg::ControlFinish>::SharedPtr subscription_control_finish;
        rclcpp::Publisher<interfaces::msg::RobotObjective>::SharedPtr publisher_robot_objective;
};




int main(int argc, char * argv[])
{
  	rclcpp::init(argc, argv);

	 auto node = std::make_shared<Event_Driven_Control>();
    // auto node_subs_pose_r1 = std::make_shared<Node_Subs_Pose_R1>();
	// auto node_subs_state_r2 = std::make_shared<Node_Subs_State_R2>();

     rclcpp::executors::MultiThreadedExecutor executor;
     executor.add_node(node);
    // executor.add_node(node_subs_pose_r1);
	// executor.add_node(node_subs_state_r2);
     executor.spin();

 	rclcpp::shutdown();
  
}