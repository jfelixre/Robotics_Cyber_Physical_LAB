#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/srv/trajectory_control.hpp>
#include <interfaces/srv/platform_vel.hpp>
#include <interfaces/msg/limit_switch.hpp>
#include <interfaces/msg/motor_vels_w_arm.hpp>
#include <interfaces/msg/arm_objective.hpp>
#include <interfaces/msg/motor_arm_vels.hpp>
#include <std_msgs/msg/float64.h>
#include <std_msgs/msg/float64.hpp>
#include <tf2_ros/transform_listener.h>
#include "tf2_ros/buffer.h"
#include <tf2_ros/transform_broadcaster.h>
#include <std_msgs/msg/empty.hpp>
#include <std_msgs/msg/empty.h>
#include <interfaces/msg/control_finish.hpp>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>

using std::placeholders::_1;
using namespace std::chrono_literals;

int robot_id = 0;
std_msgs::msg::Float64 msg_b1;
std_msgs::msg::Float64 msg_b2;
std_msgs::msg::Float64 msg_b3;
std_msgs::msg::Float64 msg_p1;
std_msgs::msg::Float64 msg_p2;

bool home_pos = true;
bool gripper = false;  //false = open, true = close
bool send_finish = false;
bool transport_pos = false;
bool take_pos = false;
int obj_id = 0;

// Struct to represent joint angles
struct JointAngles {
  double theta1;
  double theta2;
  double theta3;
};


double L1 = 0.075;
double L2 = 0.07;
//double L3 = 0.174;
double L3 = 0.25;
 int i=0;

class Arm_Position_Node : public rclcpp::Node
{
	public:
		Arm_Position_Node() : Node("arm_position_node")
		{
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

			std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/arm_objective";
            std::string topic_name = ss_topic_name.str();

            arm_position_subs= create_subscription<interfaces::msg::ArmObjective>(
      			topic_name, 1, std::bind(&Arm_Position_Node::arm_pos_callback,this,_1));

            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
            


            timer_pid_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            timer_pid_ = this->create_wall_timer(
                100ms, std::bind(&Arm_Position_Node::timer_pid_callback, this), timer_pid_cb_group_);

            

            std::stringstream ss_topic_name_b1;
            ss_topic_name_b1 << "/robot_0" << robot_id << "/b1_joint/controller";
            std::string topic_name_b1 = ss_topic_name_b1.str();
                     
            publisher_pos_b1 = this->create_publisher<std_msgs::msg::Float64>(topic_name_b1,10);

            std::stringstream ss_topic_name_b2;
            ss_topic_name_b2 << "/robot_0" << robot_id << "/b2_joint/controller";
            std::string topic_name_b2 = ss_topic_name_b2.str();

            publisher_pos_b2 = this->create_publisher<std_msgs::msg::Float64>(topic_name_b2,10);

            std::stringstream ss_topic_name_b3;
            ss_topic_name_b3 << "/robot_0" << robot_id << "/b3_joint/controller";
            std::string topic_name_b3 = ss_topic_name_b3.str();

            publisher_pos_b3 = this->create_publisher<std_msgs::msg::Float64>(topic_name_b3,10);

            std::stringstream ss_topic_name_p1;
            ss_topic_name_p1 << "/robot_0" << robot_id << "/p1_joint/controller";
            std::string topic_name_p1 = ss_topic_name_p1.str();

            publisher_pos_p1 = this->create_publisher<std_msgs::msg::Float64>(topic_name_p1,10);

            std::stringstream ss_topic_name_p2;
            ss_topic_name_p2 << "/robot_0" << robot_id << "/p2_joint/controller";
            std::string topic_name_p2 = ss_topic_name_p2.str();

            publisher_pos_p2 = this->create_publisher<std_msgs::msg::Float64>(topic_name_p2,10);

            std::stringstream ss_topic_name_finish;
            ss_topic_name_finish << "/robot_0" << robot_id << "/control_finish";
            std::string topic_name_finish = ss_topic_name_finish.str();

            publisher_control_finish = this->create_publisher<interfaces::msg::ControlFinish>(topic_name_finish,1);

        }

    private:

        rclcpp::TimerBase::SharedPtr timer_pid_;
        rclcpp::CallbackGroup::SharedPtr timer_pid_cb_group_;
        rclcpp::Subscription<interfaces::msg::ArmObjective>::SharedPtr arm_position_subs;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_b1;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_b2;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_b3;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_p1;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_p2;
        std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
        std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        rclcpp::Publisher<interfaces::msg::ControlFinish>::SharedPtr publisher_control_finish;



        void timer_pid_callback()   //CONTROL PID MEJORADO
        { 
            // 1. MODOS FIJOS (HOME, TRANSPORT, TAKE)
            if (home_pos==true){
                msg_b1.data = 2.0; msg_b2.data = -2.0; msg_b3.data = 2.0;
                msg_p1.data = -0.5; msg_p2.data = msg_p1.data;
                publisher_pos_b1->publish(msg_b1); publisher_pos_b2->publish(msg_b2);
                publisher_pos_b3->publish(msg_b3); publisher_pos_p1->publish(msg_p1); publisher_pos_p2->publish(msg_p2);
            }
            else if (transport_pos==true){
                msg_b1.data = 0.82; msg_b2.data = 0.42; msg_b3.data = 0.26;
                msg_p1.data = 0.5; msg_p2.data = msg_p1.data;
                publisher_pos_b1->publish(msg_b1); publisher_pos_b2->publish(msg_b2);
                publisher_pos_b3->publish(msg_b3); publisher_pos_p1->publish(msg_p1); publisher_pos_p2->publish(msg_p2);

                if(send_finish==true){
                    i++;
                    if (i>=50){
                        interfaces::msg::ControlFinish msg_finish; msg_finish.finish_confirm = true;
                        publisher_control_finish->publish(msg_finish); i=0;
                    }
                }
            }
            else if (take_pos==true){
                msg_b1.data = 1.5; msg_b2.data = 1.5; msg_b3.data = -1.5;
                publisher_pos_b1->publish(msg_b1); publisher_pos_b2->publish(msg_b2); publisher_pos_b3->publish(msg_b3);

                if(send_finish==true){
                    i++;
                    if (i>=50){
                        interfaces::msg::ControlFinish msg_finish; msg_finish.finish_confirm = true;
                        publisher_control_finish->publish(msg_finish); i=0;
                    }
                }
            }
            // 2. MODO IK (SEGUIMIENTO DE OBJETIVO)
            else {
                std::string objective_frame = "objective_" + std::to_string(robot_id);
                std::string arm_frame = "robot_0" + std::to_string(robot_id) + "/base_arm";

                // --- MEJORA: COMPROBAR SI EXISTE TF ANTES DE LEER ---
                if (tf_buffer_->canTransform(arm_frame, objective_frame, tf2::TimePointZero)) {
                    try{
                        auto transformStamped = tf_buffer_->lookupTransform(arm_frame, objective_frame, tf2::TimePointZero);
                        double target_x = transformStamped.transform.translation.x;
                        double target_y = transformStamped.transform.translation.y;
                        
                        JointAngles joint_angles = inverseKinematics(target_x, target_y);

                        msg_b1.data = joint_angles.theta1 * -1;
                        msg_b2.data = joint_angles.theta2 - -1; // ¿Es esto correcto? ¿O querías * -1?
                        msg_b3.data = joint_angles.theta3 * -1;

                        if (!std::isnan(joint_angles.theta1) && !std::isnan(joint_angles.theta2) && !std::isnan(joint_angles.theta3)){
                            // Limites simples
                            if(abs(joint_angles.theta1)<=1.5 && abs(joint_angles.theta2)<=1.5 && abs(joint_angles.theta3)<= 1.5){
                                publisher_pos_b1->publish(msg_b1);
                                publisher_pos_b2->publish(msg_b2);
                                publisher_pos_b3->publish(msg_b3);
                            }
                        }
                    }
                    catch (tf2::TransformException &ex){
                        // Silenciar error crítico, solo warn
                        RCLCPP_WARN(this->get_logger(), "TF Error: %s", ex.what());
                    }
                } else {
                    // Si no hay TF, no hacemos nada (evita spam de errores)
                    // Esto ocurrirá mientras el EventDrivenControl no envíe datos válidos
                }

                if(send_finish==true){
                    i++;
                    if (i>=100){
                        interfaces::msg::ControlFinish msg_finish; msg_finish.finish_confirm = true;
                        publisher_control_finish->publish(msg_finish); i=0;
                    }
                }
            }

            // 3. CONTROL DE GRIPPER
            if (gripper==true){
                msg_p1.data = 0; msg_p2.data = 0;
                publisher_pos_p1->publish(msg_p1); publisher_pos_p2->publish(msg_p2);

                // Publicar Attach solo una vez o continuamente (tu lógica actual lo hace continuamente)
                std::string topic_grab = "/robot_0" + std::to_string(robot_id) + "/cube_" + std::to_string(obj_id) + "/attach";
                auto publisher_gripper = this->create_publisher<std_msgs::msg::Empty>(topic_grab,10);
                publisher_gripper->publish(std_msgs::msg::Empty());

                std::string topipc_objf = "/FC" + std::to_string(obj_id) + "/detach";
                auto publisher_obj_j= this->create_publisher<std_msgs::msg::Empty>(topipc_objf,10);
                publisher_obj_j->publish(std_msgs::msg::Empty());
            }
            else{
                msg_p1.data = -0.5; msg_p2.data = -0.5;
                publisher_pos_p1->publish(msg_p1); publisher_pos_p2->publish(msg_p2);

                std::string topic_grab = "/robot_0" + std::to_string(robot_id) + "/cube_" + std::to_string(obj_id) + "/detach";
                auto publisher_gripper = this->create_publisher<std_msgs::msg::Empty>(topic_grab,10);
                publisher_gripper->publish(std_msgs::msg::Empty());

                std::string topipc_objf = "/FC" + std::to_string(obj_id) + "/attach";
                auto publisher_obj_j= this->create_publisher<std_msgs::msg::Empty>(topipc_objf,10);
                publisher_obj_j->publish(std_msgs::msg::Empty());
            }
        }

        void arm_pos_callback(const interfaces::msg::ArmObjective::SharedPtr msg)
        {
            home_pos = msg->home_pos;
            send_finish = msg->send_finish;
            gripper = msg->gripper;
            transport_pos = msg->transport_pos;
            take_pos = msg->take_pos;
            obj_id = msg->obj_id;
        }

        JointAngles inverseKinematics(double x, double y)
        {
            JointAngles joint_angles;
            
            // Calculate theta1 (shoulder angle)
            double theta1 = atan2(y, x);
            if (theta1 > 1.5){
                theta1 = 1.5;
            }

            if (theta1 < -1.5){
                theta1 = -1.5;
            }

            double D = sqrt(pow(x, 2) + pow(y, 2));
            double L4 = D-L1;

            // Calculate theta 2
            double theta2 = acos((pow(L4, 2) + pow(L2, 2) - pow(L3, 2)) / (2 * L4 * L2));

            double Beta = acos((pow(L3, 2) + pow(L2, 2) - pow(L4, 2)) / (2 * L3 * L2));

            // Calculate theta 3
            double theta3 = M_PI - Beta;


            joint_angles.theta1 = theta1;
            joint_angles.theta2 = theta2;
            joint_angles.theta3 = theta3;

            return joint_angles;
        }

};



int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);

	auto arm_position_node = std::make_shared<Arm_Position_Node>();
    

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(arm_position_node);
    executor.spin();

 	rclcpp::shutdown();
  
}