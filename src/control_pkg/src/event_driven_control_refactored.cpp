#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/msg/arm_objective.hpp>
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/waiting_team.hpp>
#include <interfaces/msg/follower_robot.hpp>
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

using namespace std::chrono_literals;
bool team_ready = false;

class EventDrivenControl : public rclcpp::Node {
public:
    EventDrivenControl() : Node("event_driven_control") {
        this->declare_parameter<int>("robot_id", 0);
        robot_id = this->get_parameter("robot_id").as_int();
        RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

        setup_publishers_and_subscribers();
        setup_tf_components();
        setup_timer();
    }

private:
    int robot_id;
    int object_id;

    std::string frame_name;
    float Xobj, Yobj, Zobj, Angobj;
    float angle_goal;
    float Robx, Roby, Robz, Robang;

    interfaces::msg::RobotObjective objective;
    geometry_msgs::msg::TransformStamped objective_transform;
    interfaces::msg::ArmObjective arm_objective;
    float Z_saved = 0;

    interfaces::msg::RobotObjective initial_position;


    int team_robot_id = 0;
    std::stringstream ss_topic_waiting;
    std::string topic_waiting;
    interfaces::msg::WaitingTeam waiting_msg;
    std::stringstream ss_topic_follower;
    std::string gripper_name;

    interfaces::msg::TaskDescription task;
    interfaces::msg::RobotState robot_state;
    interfaces::msg::RobotObjective objective_;
    interfaces::msg::ArmObjective arm_objective_;
    geometry_msgs::msg::TransformStamped objective_transform_;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr publisher_robot_state;
    rclcpp::Publisher<interfaces::msg::RobotObjective>::SharedPtr publisher_robot_objective;
    rclcpp::Publisher<interfaces::msg::ArmObjective>::SharedPtr publisher_arm_objective;
    rclcpp::Subscription<interfaces::msg::TaskDescription>::SharedPtr subscription_taskrobot_;
    rclcpp::Subscription<interfaces::msg::ControlFinish>::SharedPtr subscription_control_finish_;
    rclcpp::Publisher<interfaces::msg::FollowerRobot>::SharedPtr publisher_follower_robot;
    rclcpp::Publisher<interfaces::msg::WaitingTeam>::SharedPtr publisher_waiting_robot;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_copy_control;

    void setup_publishers_and_subscribers() {
        std::string tasktopic = "/robot_0" + std::to_string(robot_id) + "/task_assigned";
        subscription_taskrobot_ = this->create_subscription<interfaces::msg::TaskDescription>(
            tasktopic, 10, std::bind(&EventDrivenControl::task_robot_callback, this, std::placeholders::_1));

        std::string state_topic = "/robot_0" + std::to_string(robot_id) + "/robot_state";
        publisher_robot_state = this->create_publisher<interfaces::msg::RobotState>(state_topic, 1);

        std::string objective_topic = "/robot_0" + std::to_string(robot_id) + "/objective";
        publisher_robot_objective = this->create_publisher<interfaces::msg::RobotObjective>(objective_topic, 1);

        std::string arm_objective_topic = "/robot_0" + std::to_string(robot_id) + "/arm_objective";
        publisher_arm_objective = this->create_publisher<interfaces::msg::ArmObjective>(arm_objective_topic, 1);

        std::string control_finish_topic = "/robot_0" + std::to_string(robot_id) + "/control_finish";
        subscription_control_finish_ = this->create_subscription<interfaces::msg::ControlFinish>(
            control_finish_topic, 10, std::bind(&EventDrivenControl::control_finish_callback, this, std::placeholders::_1));


        std::string publisher_waiting_topic = "/robot_0" + std::to_string(robot_id) + "/waiting_team";
        publisher_waiting_robot = this->create_publisher<interfaces::msg::WaitingTeam>(publisher_waiting_topic,10);

        std::string publisher_copy_control_topic = "/robot_0" + std::to_string(robot_id) + "/copy_control";
        publisher_copy_control = this->create_publisher<std_msgs::msg::Bool>(publisher_copy_control_topic, 10);
    }

    void setup_tf_components() {
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
        objective_transform_.header.frame_id = "marker_id_00";
        objective_transform_.child_frame_id = "objective_" + std::to_string(robot_id);
    }

    void setup_timer() {
        timer_ = this->create_wall_timer(100ms, std::bind(&EventDrivenControl::timer_callback, this));
    }

    void timer_callback() {
        try {
            auto transform = tf_buffer_->lookupTransform("marker_id_00", "marker_id_0" + std::to_string(robot_id), tf2::TimePointZero);
            objective_transform_.header.stamp = transform.header.stamp;
            tf_broadcaster_->sendTransform(objective_transform_);
        } catch (const tf2::TransformException &ex) {
            RCLCPP_WARN(this->get_logger(), "Failed to lookup transform: %s", ex.what());
        }
    }

    void task_robot_callback(const interfaces::msg::TaskDescription::SharedPtr msg) {
        task = *msg;
        RCLCPP_INFO(this->get_logger(), "Task ID %d received by Robot %d", task.task_id, robot_id);
        robot_state.robot_state = 1;
        publisher_robot_state->publish(robot_state);
        handle_event_control();
    }

    void control_finish_callback(const interfaces::msg::ControlFinish::SharedPtr msg) {
        if (msg->finish_confirm == 1) {
            robot_state.robot_state++;
            publisher_robot_state->publish(robot_state);
        }
        handle_event_control();
    }

    void handle_event_control() {
        //Checking tasks for make transformations
        std::stringstream ss_frame_name;

            if (task.obj_id<10)
                ss_frame_name << "marker_id_0" << task.obj_id;
              else{
                ss_frame_name << "marker_id_" << task.obj_id;
              }
            
            std::string frame_name = ss_frame_name.str();

            //RCLCPP_INFO(this->get_logger(), "Frame name %s", frame_name.c_str());

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

                //RCLCPP_INFO(this->get_logger(), "Frame cube %s", frame_cube.c_str());

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

            std::stringstream ss_gripper;
            ss_gripper << "robot_0" << robot_id << "/gr_ref_link";
            gripper_name = ss_gripper.str();


        switch (robot_state.robot_state) {
            case 0:
                phase_zero_initialization();
                break;
            case 1:
                phase_one_approach();
                break;
            case 2:
                phase_two_last_approach();
                break;
            case 3:
                phase_three_take_object();
                break;
            case 4:
                phase_four_transport();
                break;
            case 5:
                phase_five_place_object();
                break;
            case 6:
                phase_six_release_object();
                break;
            case 7:
                phase_seven_return_to_start();
                break;
            case 8:
                phase_eight_finalize_task();
                break;
            case 9:
                phase_nine_reset_state();
                break;
            default:
                RCLCPP_INFO(this->get_logger(), "Unhandled state: %d", robot_state.robot_state);
        }
    }


    // PHASE ZERO: Initialization ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_zero_initialization() {
        RCLCPP_INFO(this->get_logger(), "Phase 0: Initialization");
        RCLCPP_INFO(this->get_logger(), "Robot %d waiting for a new task", robot_id);

        // Obtain robot position
        try {
            geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", "robot_0" + std::to_string(robot_id) + "/gr_ref_link", tf2::TimePointZero);
            Robx = transform.transform.translation.x;
            Roby = transform.transform.translation.y;
            Robz = transform.transform.translation.z;

            tf2::Quaternion Rob_quat(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
            tf2::Matrix3x3 Rob_m(Rob_quat);
            double Rob_orientation_x, Rob_orientation_y, Rob_orientation_z;
            Rob_m.getRPY(Rob_orientation_x, Rob_orientation_y, Rob_orientation_z);
            Robang = Rob_orientation_z;

        } catch (tf2::LookupException& ex) {
            RCLCPP_ERROR(this->get_logger(), "Lookup exception: %s", ex.what());
        } catch (tf2::ConnectivityException& ex) {
            RCLCPP_ERROR(this->get_logger(), "Connectivity exception: %s", ex.what());
        } catch (tf2::ExtrapolationException& ex) {
            RCLCPP_ERROR(this->get_logger(), "Extrapolation exception: %s", ex.what());
        }

        initial_position.point.x = Robx;
        initial_position.point.y = Roby;
        initial_position.angle = Robang;
    }

    // PHASE ONE: Approaching to object ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_one_approach() {
        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 1 Approaching to object %d", robot_id, task.obj_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            //save initial position
            try{
                geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", gripper_name, tf2::TimePointZero);
                Robx = transform.transform.translation.x;
                Roby = transform.transform.translation.y;
                Robz = transform.transform.translation.z;

                tf2::Quaternion Rob_quat(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
                tf2::Matrix3x3 Rob_m(Rob_quat);
                double Rob_orientation_x, Rob_orientation_y, Rob_orientation_z;
                Rob_m.getRPY(Rob_orientation_x, Rob_orientation_y, Rob_orientation_z);
                Robang= Rob_orientation_z;


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
                
                initial_position.point.x = Robx;
                initial_position.point.y = Roby;
                initial_position.angle = Robang;

                //Send objective position
                objective.point.x = Xobj - (0.5 * cos(Angobj));   //Check to match, maybe using trigonometry depending of angle
                objective.point.y = Yobj - (0.5 * sin(Angobj));
                objective.point.z = Zobj;
                objective.angle = Angobj;       //
                objective.obj_id = task.obj_id;
                objective.robot_state = robot_state.robot_state;
                publisher_robot_objective->publish(objective);

                //Send objective position to /tf2
                objective_transform.transform.translation.x = objective.point.x;
                objective_transform.transform.translation.y = objective.point.y;
                objective_transform.transform.translation.z = objective.point.z;
                

                arm_objective.home_pos = false;
                arm_objective.gripper = false;
                arm_objective.send_finish = false;
                arm_objective.transport_pos = false;
                arm_objective.obj_id = task.obj_id;
                publisher_arm_objective->publish(arm_objective); 
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
                //save initial position
                try{
                    geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", gripper_name, tf2::TimePointZero);
                    Robx = transform.transform.translation.x;
                    Roby = transform.transform.translation.y;
                    Robz = transform.transform.translation.z;

                    tf2::Quaternion Rob_quat(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
                    tf2::Matrix3x3 Rob_m(Rob_quat);
                    double Rob_orientation_x, Rob_orientation_y, Rob_orientation_z;
                    Rob_m.getRPY(Rob_orientation_x, Rob_orientation_y, Rob_orientation_z);
                    Robang= Rob_orientation_z;


                    } catch (tf2::LookupException& ex) {
                        RCLCPP_ERROR(this->get_logger(), "Lookup exception: %s", ex.what());
                        //return;waiting_team = true; //Signal to wait another robot exception: %s", ex.what());
                        //return;
                    }
                    
                    initial_position.point.x = Robx;
                    initial_position.point.y = Roby;
                    initial_position.angle = Robang;

                    //Send objective position
                    objective.point.x = Xobj + (0.8 * cos(Angobj));   //Check to match, maybe using trigonometry depending of angle
                    objective.point.y = Yobj + (0.8 * sin(Angobj));
                    objective.point.z = Zobj;
                    objective.angle =  (Angobj + M_PI) - static_cast<int>((Angobj + M_PI) / (2*M_PI)) * 2*M_PI;      //
                    objective.obj_id = task.obj_id;
                    objective.robot_state = robot_state.robot_state;
                    publisher_robot_objective->publish(objective);
                    
                    //Send objective position to /tf2
                    objective_transform.transform.translation.x = objective.point.x;
                    objective_transform.transform.translation.y = objective.point.y;
                    objective_transform.transform.translation.z = objective.point.z;

                    RCLCPP_INFO(this->get_logger(), "Objective point x= %f, y= %f", objective.point.x, objective.point.y);
                    

                    arm_objective.home_pos = false;
                    arm_objective.gripper = false;
                    arm_objective.send_finish = false;
                    arm_objective.transport_pos = false;
                    arm_objective.obj_id = task.obj_id;
                    arm_objective.take_pos = true;
                    publisher_arm_objective->publish(arm_objective); 
                
            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                ss_topic_follower << "/robot_0" << task.leader_robot_id << "/follower_robot";
                            std::string topic_follower = ss_topic_follower.str();
                            publisher_follower_robot = this->create_publisher<interfaces::msg::FollowerRobot>(topic_follower,10);
                            interfaces::msg::FollowerRobot follower_robot_msg;
                            follower_robot_msg.follower_robot_id = robot_id;
                            publisher_follower_robot->publish(follower_robot_msg);

                            RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 1 Approach to object %d", robot_id, task.obj_id);

                            //save initial position
                            try{
                            geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("marker_id_00", gripper_name, tf2::TimePointZero);
                            Robx = transform.transform.translation.x;
                            Roby = transform.transform.translation.y;
                            Robz = transform.transform.translation.z;

                            tf2::Quaternion Rob_quat(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
                            tf2::Matrix3x3 Rob_m(Rob_quat);
                            double Rob_orientation_x, Rob_orientation_y, Rob_orientation_z;
                            Rob_m.getRPY(Rob_orientation_x, Rob_orientation_y, Rob_orientation_z);
                            Robang= Rob_orientation_z;


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
                            
                            initial_position.point.x = Robx;
                            initial_position.point.y = Roby;
                            initial_position.angle = Robang;

                            //Send objective position
                            objective.point.x = Xobj - (0.8 * cos(Angobj));   //Check to match, maybe using trigonometry depending of angle
                            objective.point.y = Yobj - (0.8 * sin(Angobj));
                            objective.point.z = Zobj;
                            objective.angle = Angobj;       //
                            objective.obj_id = task.obj_id;
                            objective.robot_state = robot_state.robot_state;
                            publisher_robot_objective->publish(objective);

                            //Send objective position to /tf2
                            objective_transform.transform.translation.x = objective.point.x;
                            objective_transform.transform.translation.y = objective.point.y;
                            objective_transform.transform.translation.z = objective.point.z;

                            RCLCPP_INFO(this->get_logger(), "Objective point x= %f, y= %f", objective.point.x, objective.point.y);
                            

                            arm_objective.home_pos = false;
                            arm_objective.gripper = false;
                            arm_objective.send_finish = false;
                            arm_objective.transport_pos = false;
                            arm_objective.obj_id = task.obj_id;
                            arm_objective.take_pos = true;
                            publisher_arm_objective->publish(arm_objective);
            }

        }
    }
    
    // PHASE TWO: Last approach to object ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_two_last_approach() {
    
        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 2 Last approach to object %d", robot_id, task.obj_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            objective.point.x = Xobj;   //Check to match, maybe using trigonometry depending of angle
            objective.point.y = Yobj - 0.1;
            objective.point.z = Zobj;
            objective.angle = Angobj;       //
            objective.obj_id = task.obj_id;
            objective.robot_state = robot_state.robot_state;
            publisher_robot_objective->publish(objective);

            //Send objective position to /tf2
            objective_transform.transform.translation.x = objective.point.x;
            objective_transform.transform.translation.y = objective.point.y;
            objective_transform.transform.translation.z = objective.point.z;
            Z_saved= objective.point.z;
            
            arm_objective.home_pos = false;
            arm_objective.gripper = false;
            arm_objective.send_finish = false;
            arm_objective.transport_pos = false;
            arm_objective.obj_id = task.obj_id;
            publisher_arm_objective->publish(arm_objective); 
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
                arm_objective.home_pos = false;
                arm_objective.gripper = false;
                arm_objective.send_finish = false;
                arm_objective.transport_pos = false;
                arm_objective.obj_id = task.obj_id;
                arm_objective.take_pos = true;
                publisher_arm_objective->publish(arm_objective);

                //Send objective position
                objective.point.x = Xobj + (0.19 * cos(Angobj));   //Check to match, maybe using trigonometry depending of angle
                objective.point.y = Yobj + (0.19 * sin(Angobj));
                objective.point.z = Zobj;
                objective.angle =  (Angobj + M_PI) - static_cast<int>((Angobj + M_PI) / (2*M_PI)) * 2*M_PI;      //
                objective.obj_id = task.obj_id;
                objective.robot_state = robot_state.robot_state;
                publisher_robot_objective->publish(objective);
                
                //Send objective position to /tf2
                objective_transform.transform.translation.x = objective.point.x;
                objective_transform.transform.translation.y = objective.point.y;
                objective_transform.transform.translation.z = objective.point.z;
            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                arm_objective.home_pos = false;
                arm_objective.gripper = false;
                arm_objective.send_finish = false;
                arm_objective.transport_pos = false;
                arm_objective.obj_id = task.obj_id;
                arm_objective.take_pos = true;
                publisher_arm_objective->publish(arm_objective);

                //Send objective position
                objective.point.x = Xobj - (0.27 * cos(Angobj));   //Check to match, maybe using trigonometry depending of angle
                objective.point.y = Yobj - (0.27 * sin(Angobj));
                objective.point.z = Zobj;
                objective.angle = Angobj;       //
                objective.obj_id = task.obj_id;
                objective.robot_state = robot_state.robot_state;
                publisher_robot_objective->publish(objective);

                //Send objective position to /tf2
                objective_transform.transform.translation.x = objective.point.x;
                objective_transform.transform.translation.y = objective.point.y;
                objective_transform.transform.translation.z = objective.point.z;
            }

        }
    }

    // PHASE THREE: Taking object ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_three_take_object() {

        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 3 Taking object %d", robot_id, task.obj_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            objective_transform.transform.translation.x = objective.point.x;
            objective_transform.transform.translation.y = objective.point.y;
            objective_transform.transform.translation.z = objective.point.z;

            arm_objective.home_pos = false;
            arm_objective.gripper = true;
            arm_objective.send_finish = false;
            arm_objective.transport_pos = false;
            arm_objective.obj_id = task.obj_id;
            publisher_arm_objective->publish(arm_objective); 

            //timer to wait robot close gripper
            rclcpp::sleep_for(10s);

            arm_objective.send_finish = true;
            arm_objective.transport_pos = true;
            publisher_arm_objective->publish(arm_objective); 
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed) *************************************************************************************************************
            RCLCPP_INFO(this->get_logger(), "Two robots needed to take object");

            while(team_ready == false){
                RCLCPP_INFO(this->get_logger(), "Robot_ID %d waiting for team robot", robot_id);
                
                waiting_msg.waiting_team = true;
                publisher_waiting_robot->publish(waiting_msg);


                rclcpp::sleep_for(1s);
            }

            waiting_msg.waiting_team = true;
            publisher_waiting_robot->publish(waiting_msg);


            if (robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
                RCLCPP_INFO(this->get_logger(), "Robot_ID %d Team robot ready. Taking object", robot_id);

                arm_objective.home_pos = false;
                arm_objective.gripper = true;
                arm_objective.take_pos = true;
                arm_objective.send_finish = false;
                arm_objective.transport_pos = false;
                arm_objective.obj_id = task.obj_id;
                publisher_arm_objective->publish(arm_objective); 

                //timer to wait robot close gripper
                rclcpp::sleep_for(10s);

                arm_objective.take_pos = false;
                arm_objective.send_finish = true;
                arm_objective.transport_pos = true;
                publisher_arm_objective->publish(arm_objective); 
            }
            else { //Logic for follower robot **************************************************************************************************************************************************
                RCLCPP_INFO(this->get_logger(), "Robot_ID %d Team robot ready. Taking object", robot_id);

                arm_objective.home_pos = false;
                arm_objective.gripper = true;
                arm_objective.take_pos = true;
                arm_objective.send_finish = false;
                arm_objective.transport_pos = false;
                arm_objective.obj_id = task.obj_id;
                publisher_arm_objective->publish(arm_objective); 

                //timer to wait robot close gripper
                rclcpp::sleep_for(10s);

                arm_objective.send_finish = true;
                arm_objective.transport_pos = true;
                publisher_arm_objective->publish(arm_objective); 
            }
         }
    }

    // PHASE FOUR: Transporting object ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_four_transport() {

        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 4 Transporting object %d", robot_id, task.obj_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            objective.point.x = task.goal.x - (0.5 * cos(angle_goal));   //Check to match, maybe using trigonometry depending of angle
            objective.point.y = task.goal.y - (0.5 * sin(angle_goal));
            objective.angle = angle_goal;       // Define if i can select goal angle
            objective.obj_id = task.obj_id;
            objective.robot_state = robot_state.robot_state;
            publisher_robot_objective->publish(objective);

            objective_transform.transform.translation.x = objective.point.x;
            objective_transform.transform.translation.y = objective.point.y;
            

            arm_objective.home_pos = false;
            arm_objective.gripper = true;
            arm_objective.send_finish = false;
            arm_objective.transport_pos = true;
            arm_objective.obj_id = task.obj_id;
            publisher_arm_objective->publish(arm_objective); 
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
               //Send objective position
               objective.point.x = task.goal.x - (0.8 * cos(angle_goal));   //Check to match, maybe using trigonometry depending of angle
               objective.point.y = task.goal.y - (0.8 * sin(angle_goal));
               objective.angle =  (angle_goal + M_PI) - static_cast<int>((angle_goal + M_PI) / (2*M_PI)) * 2*M_PI;      //
               objective.obj_id = task.obj_id;
               objective.robot_state = robot_state.robot_state;
               publisher_robot_objective->publish(objective);
               
               //Send objective position to /tf2
               objective_transform.transform.translation.x = objective.point.x;
               objective_transform.transform.translation.y = objective.point.y;

            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                std_msgs::msg::Bool copy_message;
                copy_message.data = true;
                publisher_copy_control->publish(copy_message);
                RCLCPP_INFO(this->get_logger(), "Copying leader movements");
                publisher_copy_control->publish(copy_message);
            }

        }
    }

    // PHASE FIVE: Placing object ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_five_place_object() {

        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 2 Placing object %d", robot_id, task.obj_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
               
            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                
            }

        }
    }

    // PHASE SIX: Releasing object ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_six_release_object() {

        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 6 Releasing object %d", robot_id, task.obj_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
               
            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                
            }

        }
    }

    // PHASE SEVEN: Returning to start position ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_seven_return_to_start() {

        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 7 Returning to start position", robot_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
               
            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                
            }

        }
    }

    // PHASE EIGHT: Finalizing task ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_eight_finalize_task() {

        RCLCPP_INFO(this->get_logger(), "Robot_ID %d Phase 8 Finalizing task", robot_id);

        
        if (task.obj_size == 1) { //Logic for object size = 1 (One robot needed) *************************************************************************************************************
            
        }

        else if (task.obj_size == 2) { //Logic for object size = 2 (Two robots needed)
            
            if (task.robot_id == task.leader_robot_id) {  //Logic for leader robot **************************************************************************************************************
               
            }
            
            else { //Logic for follower robot **************************************************************************************************************************************************
                
            }

        }
    }

    // PHASE NINE: Resetting state ------------------------------------------------------------------------------------------------------------------------------------------------------
    void phase_nine_reset_state() {
        RCLCPP_INFO(this->get_logger(), "Phase 9: Resetting state");
        robot_state.robot_state = 0;
        publisher_robot_state->publish(robot_state);
    }
};

class Subs_Node_Team : public rclcpp::Node {
    public:
    Subs_Node_Team() : Node("subs_node_team") {
        subscription_team_ready_ = this->create_subscription<std_msgs::msg::Bool>(
            "/team_ready", 10, std::bind(&Subs_Node_Team::team_ready_callback, this, std::placeholders::_1));            
           
        }
    
    private:
        rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription_team_ready_;

        void team_ready_callback(const std_msgs::msg::Bool::SharedPtr msg) {
            if (msg->data) {
                //RCLCPP_INFO(this->get_logger(), "Team is ready");
                team_ready = true;
            } else {
                //RCLCPP_INFO(this->get_logger(), "Team is not ready");
                team_ready = false;
            }
        }

};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<EventDrivenControl>();
    auto subs_node_team = std::make_shared<Subs_Node_Team>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(subs_node_team);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}
