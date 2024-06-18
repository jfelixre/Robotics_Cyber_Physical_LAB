#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.hpp>
#include "tf2_ros/buffer.h"
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/srv/event_control.hpp>
#include <interfaces/msg/arm_joints_positions.hpp>
#include <interfaces/msg/trajectory_control.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <visualization_msgs/msg/marker.hpp>
#include <interfaces/msg/arm_objective.hpp>

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

float Xobj, Yobj, Zobj, Angobj;
float angle_goal;
interfaces::msg::RobotObjective objective;
geometry_msgs::msg::TransformStamped objective_transform;
interfaces::msg::ArmObjective arm_objective;

class Event_Driven_Control : public rclcpp::Node
{
	public:
		Event_Driven_Control() : Node("event_driven_control")
		{
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/task_assigned";
            std::string topic_name = ss_topic_name.str();

            subscription_task_robot = this->create_subscription<interfaces::msg::TaskDescription>(
                topic_name, 10, std::bind(&Event_Driven_Control::task_robot_callback, this, _1));

            //subscription_positions = this->create_subscription<interfaces::msg::Positions>(
            //    "positions", 10, std::bind(&Event_Driven_Control::positions_callback, this, _1));

            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

            
            std::stringstream ss_topic_name_2;
            ss_topic_name_2 << "/robot_0" << robot_id << "/robot_state";
            std::string topic_name_2 = ss_topic_name_2.str();

            publisher_robot_state = this->create_publisher<interfaces::msg::RobotState>(topic_name_2,1);
            
            robot_state.robot_state = 0;

            publisher_robot_state -> publish(robot_state);


            std::stringstream ss_topic_name_3;
            ss_topic_name_3 << "/robot_0" << robot_id << "/control_finish";
            std::string topic_name_3 = ss_topic_name_3.str();


            subscription_control_finish = this->create_subscription<interfaces::msg::ControlFinish>(
                topic_name_3, 10, std::bind(&Event_Driven_Control::control_finish_callback, this, _1));


            std::stringstream ss_topic_name_4;
            ss_topic_name_4 << "/robot_0" << robot_id << "/objective";
            std::string topic_name_4 = ss_topic_name_4.str();

            publisher_robot_objective = create_publisher<interfaces::msg::RobotObjective>(topic_name_4, 1);

            //Create Transform staped for objective position using /tf2
            
            objective_transform.header.frame_id = "marker_id_00";
            std::stringstream ss_frame_objective;
            ss_frame_objective << "objective_" << robot_id;
            std::string objective_frame = ss_frame_objective.str();
            objective_transform.child_frame_id = objective_frame;

            timer_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            timer_ = this->create_wall_timer(
                100ms, std::bind(&Event_Driven_Control::timer_callback, this), timer_cb_group_);

            std::stringstream ss_topic_arm_obj;
            ss_topic_arm_obj << "/robot_0" << robot_id << "/arm_objective";
            std::string topic_arm_obj = ss_topic_arm_obj.str();

            publisher_arm_objective = create_publisher<interfaces::msg::ArmObjective>(topic_arm_obj, 1);

		}


	private:

        void timer_callback()   //CONTROL PID//
        {
            std::stringstream ss_frame_name;
            
            ss_frame_name << "robot_0" << robot_id << "/base_link";

           
            std::string frame_name = ss_frame_name.str();

           try{
            geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", "marker_id_01", tf2::TimePointZero);

            objective_transform.header.stamp = transform.header.stamp;


            tf_broadcaster_->sendTransform(objective_transform);

            //RCLCPP_INFO(this->get_logger(), "Transform sent with header stamp %d", transform.header.stamp.sec);

            } catch (tf2::LookupException& ex) {
                RCLCPP_ERROR(this->get_logger(), "Lookup exception: %s", ex.what());
                //return;
            } catch (tf2::ConnectivityException& ex) {
                RCLCPP_ERROR(this->get_logger(), "Connectivity exception: %s", ex.what());
                //return;
            } catch (tf2::ExtrapolationException& ex) {
                RCLCPP_ERROR(this->get_logger(), "Extrapolation exception: %s", ex.what());
                //return;
            }
        }

        void task_robot_callback(interfaces::msg::TaskDescription::SharedPtr msg)
            {
                task = *msg;
                RCLCPP_INFO(this->get_logger(), "Task ID %d received by event_control of Robot %d", task.task_id, robot_id);

                robot_state.robot_state = 1;
                publisher_robot_state -> publish(robot_state);

                event_control(); //Call for new step in event driven control
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

            // for (auto& tag_pos : tags_positions.tag_pos) {                   //Save object position
            //         if (tag_pos.tag_id == task.obj_id){
            //             Xobj = tag_pos.position.position.x;
            //             Yobj = tag_pos.position.position.y;
            //             tf2::Quaternion Obj_quat(tag_pos.position.orientation.x, tag_pos.position.orientation.y, tag_pos.position.orientation.z, tag_pos.position.orientation.w);
            //             tf2::Matrix3x3 Obj_m(Obj_quat);
            //             double Obj_orientation_x, Obj_orientation_y, Obj_orientation_z;
            //             Obj_m.getRPY(Obj_orientation_x, Obj_orientation_y, Obj_orientation_z);
            //             Angobj= Obj_orientation_z;
            //         }
            //     }


            //Save object position


            std::stringstream ss_frame_name;

            if (task.obj_id<10)
                ss_frame_name << "marker_id_0" << task.obj_id;
              else{
                ss_frame_name << "marker_id_" << task.obj_id;
              }
            
            std::string frame_name = ss_frame_name.str();

            try{
                geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", frame_name, tf2::TimePointZero);
                Xobj = transform.transform.translation.x;
                Yobj = transform.transform.translation.y;
                Zobj = transform.transform.translation.z;

                tf2::Quaternion Obj_quat(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
                tf2::Matrix3x3 Obj_m(Obj_quat);
                double Obj_orientation_x, Obj_orientation_y, Obj_orientation_z;
                Obj_m.getRPY(Obj_orientation_x, Obj_orientation_y, Obj_orientation_z);
                Angobj= Obj_orientation_z;

                std::stringstream ss_frame_cube;
                ss_frame_cube << "cube_id_" << task.obj_id << "/cube_link";
                std::string frame_cube = ss_frame_cube.str();

                try{
                    geometry_msgs::msg::TransformStamped transform_cube = tf_buffer_->lookupTransform("marker_id_00", frame_cube, tf2::TimePointZero);
                    Zobj = transform_cube.transform.translation.z;

                } catch (tf2::LookupException& ex) {
                    RCLCPP_ERROR(this->get_logger(), "Lookup exception: %s", ex.what());
                    //return;
                } catch (tf2::ConnectivityException& ex) {
                    RCLCPP_ERROR(this->get_logger(), "Connectivity exception: %s", ex.what());
                    //return;
                } catch (tf2::ExtrapolationException& ex) {
                    RCLCPP_ERROR(this->get_logger(), "Extrapolation exception: %s", ex.what());
                    //return;
                }

            } catch (tf2::LookupException& ex) {
                RCLCPP_ERROR(this->get_logger(), "Lookup exception: %s", ex.what());
                //return;
            } catch (tf2::ConnectivityException& ex) {
                RCLCPP_ERROR(this->get_logger(), "Connectivity exception: %s", ex.what());
                //return;
            } catch (tf2::ExtrapolationException& ex) {
                RCLCPP_ERROR(this->get_logger(), "Extrapolation exception: %s", ex.what());
                //return;
            }

            angle_goal=task.angle_goal;

            



            //Check if robot is leader or follower and start control

                //Start control when robot is leader
            if (task.leader_robot_id == 0){
                switch(robot_state.robot_state){   //CHECK CASE WHEN OBJECT SIZE IS 2, must take object from different angle

                    case 0:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d waiting for a new task", robot_id);
                        break;

                    case 1:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 1 Approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj - (0.5 * cos(Angobj));   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj - (0.5 * sin(Angobj));
                        objective.point.z = Zobj;
                        objective.angle = Angobj;       //
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);

                        //Send objective position to /tf2
                        objective_transform.transform.translation.x = objective.point.x;
                        objective_transform.transform.translation.y = objective.point.y;
                        objective_transform.transform.translation.z = objective.point.z;

                        arm_objective.home_pos = false;
                        arm_objective.gripper = false;
                        arm_objective.send_finish = false;
                        publisher_arm_objective->publish(arm_objective);                        


                        break;

                    case 2:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 2 Last approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj;
                        objective.point.z = Zobj;
                        objective.angle = Angobj;       //
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);

                        //Send objective position to /tf2
                        objective_transform.transform.translation.x = objective.point.x;
                        objective_transform.transform.translation.y = objective.point.y;
                        objective_transform.transform.translation.z = objective.point.z;

                        arm_objective.home_pos = false;
                        arm_objective.gripper = false;
                        arm_objective.send_finish = false;
                        publisher_arm_objective->publish(arm_objective); 

                        break;

                    case 3:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 3 Taking object %d", robot_id, task.obj_id);
                        //COMPLETE OBJECT PICK
                        //Send objective position to /tf2
                        objective_transform.transform.translation.x = objective.point.x;
                        objective_transform.transform.translation.y = objective.point.y;
                        objective_transform.transform.translation.z = objective.point.z;

                        arm_objective.home_pos = false;
                        arm_objective.gripper = true;
                        arm_objective.send_finish = true;
                        publisher_arm_objective->publish(arm_objective); 

                        break;

                    case 4:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 4 Approach to objective point, x= %d, y= %d", robot_id, task.goal.x, task.goal.y);

                        objective.point.x = task.goal.x - (0.5 * cos(angle_goal));   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = task.goal.y - (0.5 * sin(angle_goal));
                        objective.angle = angle_goal;       // Define if i can select goal angle
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);
                        break;

                    
                    case 5:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 5 Placing object on point, x= %d, y= %d", robot_id, task.goal.x, task.goal.y);

                        //COMPLETE OBJECT PLACE
                        objective.point.x = Xobj;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj;
                        objective.point.z = Zobj;
                        objective.angle = angle_goal;       //
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);


                        break;

                    case 6:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 6 Leaving object", robot_id);

                        objective.point.x = task.goal.x - (0.5 * cos(angle_goal));   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = task.goal.y - (0.5 * sin(angle_goal));
                        objective.angle = angle_goal;       // Define if i can select goal angle
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);
                        break;

                    case 7:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 7 Back to home position", robot_id);

                        objective.point.x = 0;   //Define home position***
                        objective.point.y = 0;
                        objective.angle = 0;
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);
                        break;

                    case 8:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 8 Task_ID %d Finished", robot_id, task.task_id);

                        robot_state.robot_state = 0;
                        publisher_robot_state -> publish(robot_state);
                        break;
                }
            }
                //Start control when robot is follower
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
                        objective.obj_id = task.obj_id;
                        publisher_robot_objective->publish(objective);
                        break;

                    case 2:
                        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 2 Last approach to object %d", robot_id, task.obj_id);

                        objective.point.x = Xobj - 2;   //Check to match, maybe using trigonometry depending of angle
                        objective.point.y = Yobj - 2;
                        objective.angle = Angobj;       //
                        objective.obj_id = task.obj_id;
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
                        objective.obj_id = task.obj_id;
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
        //rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subscription_positions;
        rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr publisher_robot_state;
        rclcpp::Subscription<interfaces::msg::ControlFinish>::SharedPtr subscription_control_finish;
        rclcpp::Publisher<interfaces::msg::RobotObjective>::SharedPtr publisher_robot_objective;
        std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
        std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::CallbackGroup::SharedPtr timer_cb_group_;
        rclcpp::Publisher<interfaces::msg::ArmObjective>::SharedPtr publisher_arm_objective;

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