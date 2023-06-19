#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/srv/trajectory_control.hpp>
#include <interfaces/srv/platform_vel.hpp>
#include <interfaces/msg/limit_switch.hpp>
#include <interfaces/msg/motor_vels_w_arm.hpp>
#include <Eigen/Dense>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using std::placeholders::_1;
using namespace std::chrono_literals;

//Variables LimitSwitch
int LS_B1_min, LS_B1_max, LS_B2_min, LS_B2_max, LS_B3_min, LS_B3_max;

//Variables velocidades de encoders
float vel_B1,vel_B2,vel_B3,vel_G1;

//Variables de posición de brazo
float B1_pos=0;
float B2_pos=0;
float B3_pos=0;
float velG1=1;

//Variables para objetivos
float B1goal=0;
float B2goal=0;
float B3goal=0;

//Variables PID
float KPb1=150;
float KIb1=80;
float KDb1=350;

float KPb2=120;
float KIb2=30;
float KDb2=200;

float KPb3=70;
float KIb3=5;
float KDb3=50;

float Pb1=0;
float Ib1=0;
float Db1=0;

float Pb2=0;
float Ib2=0;
float Db2=0;

float Pb3=0;
float Ib3=0;
float Db3=0;

float sumPIDb1=0;
float sumPIDb2=0;
float sumPIDb3=0;

//Variables para control
float errorB1=0;
float errorB1_ant1=0;
float errorB1_ant2=0;
float INTerrorB1=0;
float DERerrorB1=0;

float errorB2=0;
float errorB2_ant1=0;
float errorB2_ant2=0;
float INTerrorB2=0;
float DERerrorB2=0;

float errorB3=0;
float errorB3_ant1=0;
float errorB3_ant2=0;
float INTerrorB3=0;
float DERerrorB3=0;

float dt_sec = 0.05;


class Control_Arm_Pid_R1 : public rclcpp::Node
{
	public:
		Control_Arm_Pid_R1() : Node("node_control_arm_pid_r1")
		{
            timer_pid_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            timer_pid_ = this->create_wall_timer(
                50ms, std::bind(&Control_Arm_Pid_R1::timer_pid_callback, this), timer_pid_cb_group_);

        }

    private:

        rclcpp::TimerBase::SharedPtr timer_pid_;
        rclcpp::CallbackGroup::SharedPtr timer_pid_cb_group_;


        void timer_pid_callback()   //CONTROL PID//
        { 
            //Cálculo de errores
			errorB1 = B1goal - B1_pos;
			errorB2 = B2goal - B2_pos;
			errorB3 = B3goal - B3_pos;

			//RCLCPP_INFO(this->get_logger(), "velB1= %f", vel_B1);
			//RCLCPP_INFO(this->get_logger(), "goal B1= %f", B1goal);
			//RCLCPP_INFO(this->get_logger(), "Error B1= %f", errorB1);
			//RCLCPP_INFO(this->get_logger(), "Pos B1= %f", B1_pos);

			//Calculo de PID B1

			Pb1 = KPb1 * errorB1;

			INTerrorB1= (errorB1 + errorB1_ant1 + errorB1_ant2) * dt_sec;
			Ib1 = KIb1 * INTerrorB1;

			DERerrorB1=(errorB1 - errorB1_ant1)/dt_sec;

			Db1=DERerrorB1*KDb1;

			errorB1_ant2 = errorB1_ant1;
			errorB1_ant1 = errorB1;

			sumPIDb1 = Pb1 + Ib1 + Db1;

			if (sumPIDb1>10) {sumPIDb1=10;}
			if (sumPIDb1<-10) {sumPIDb1=-10;}

			//Calculo de PID B2

			Pb2 = KPb2 * errorB2;

			INTerrorB2= (errorB2 + errorB2_ant1 + errorB2_ant2) * dt_sec;
			Ib2 = KIb2 * INTerrorB2;

			DERerrorB2=(errorB2 - errorB2_ant1)/dt_sec;
			Db2=DERerrorB2*KDb2;

			errorB2_ant2 = errorB2_ant1;
			errorB2_ant1 = errorB2;

			sumPIDb2 = Pb2 + Ib2 + Db2;

			if (sumPIDb2>10) {sumPIDb2=10;}
			if (sumPIDb2<-10) {sumPIDb2=-10;}

			//Calculo de PID B3

			Pb3 = KPb3 * errorB3;

			INTerrorB3= (errorB3 + errorB3_ant1 + errorB3_ant2) * dt_sec;
			Ib3 = KIb3 * INTerrorB3;

			DERerrorB3=(errorB3 - errorB3_ant1)/dt_sec;
			Db3=DERerrorB3*KDb3;

			errorB3_ant2 = errorB3_ant1;
			errorB3_ant1 = errorB3;

			sumPIDb3 = Pb3 + Ib3 + Db3;

			if (sumPIDb3>10) {sumPIDb3=10;}
			if (sumPIDb3<-10) {sumPIDb3=-10;}

        }

};

class Node_Estimate_Position : public rclcpp::Node
{
	public:
		Node_Estimate_Position() : Node("node_estimate_position")
		{
            LmSw_subscriber= create_subscription<interfaces::msg::LimitSwitch>(
      			"/robot_1/limit_switch", 1, std::bind(&Node_Estimate_Position::LmSw_subs,this,_1));

			encoders_subscriber= create_subscription<interfaces::msg::MotorVelsWArm>(
      			"/robot_1/encoders", 1, std::bind(&Node_Estimate_Position::encoders_subs,this,_1));

          //  timer_ep_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            //timer_estimate_pos_ = this->create_wall_timer(
             //    100ms, std::bind(&Node_Estimate_Position::timer_ep_callback, this), timer_ep_cb_group_);
                       
        }

    private:

        rclcpp::Subscription<interfaces::msg::LimitSwitch>::SharedPtr LmSw_subscriber;
        rclcpp::Subscription<interfaces::msg::MotorVelsWArm>::SharedPtr encoders_subscriber;
        //rclcpp::TimerBase::SharedPtr timer_estimate_pos_;
        //rclcpp::CallbackGroup::SharedPtr timer_ep_cb_group_;

    /*
        //Encoder envia los mensajes con la posicion, no con velocidades, Se necesita corregir para implementacion
        //Checar Robot plugin
        void timer_ep_callback()
        { 
            //
            //Cálculo de posiciones
            //B1_pos=B1_pos+(vel_B1*dt_sec);
            B1_pos=vel_B1+0.1;
            //B2_pos=B2_pos+(vel_B2*dt_sec);
            B2_pos=(vel_B2+0.1)*-1;
            //B3_pos=B3_pos+(vel_B3*dt_sec);
            B3_pos=vel_B3+0.1;

        }
    */
        void LmSw_subs(const interfaces::msg::LimitSwitch::SharedPtr msg) const
            {
                LS_B1_min=msg->b1_min;
                LS_B1_max=msg->b1_max;
                LS_B2_min=msg->b2_min;
                LS_B2_max=msg->b2_max;
                LS_B3_min=msg->b3_min;
                LS_B3_max=msg->b3_max;

                if (LS_B1_min==1) { 
                    B1_pos=-1.4;
                }

                if (LS_B1_max==1) { 
                    B1_pos=1.4;
                }

                if (LS_B2_min==1) { 
                    B2_pos=-1.45;
                }

                if (LS_B2_max==1) { 
                    B2_pos=1.45;
                }

                if (LS_B3_min==1) { 
                    B3_pos=-1.5;
                }

                if (LS_B3_max==1) { 
                    B3_pos=1.5;
                }

            }

            void encoders_subs(const interfaces::msg::MotorVelsWArm::SharedPtr msg) const
            {
                vel_B1=msg->vel_b1;
                vel_B2=msg->vel_b2;
                vel_B3=msg->vel_b3;
                vel_G1=msg->vel_g1;

                vel_B2=vel_B2*-1;


                //Cálculo de posiciones
                //B1_pos=B1_pos+(vel_B1*dt_sec);
                B1_pos=vel_B1+0.1;
                //B2_pos=B2_pos+(vel_B2*dt_sec);
                B2_pos=(vel_B2+0.1)*-1;
                //B3_pos=B3_pos+(vel_B3*dt_sec);
                B3_pos=vel_B3+0.1;
            }


};


int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);

	auto node_control_arm_pid_r1 = std::make_shared<Control_Arm_Pid_R1>();
    auto node_estimate_position = std::make_shared<Node_Estimate_Position>();
	//auto node_subs_positions = std::make_shared<Node_Subs_Positions>();
   // auto node_control_timer = std::make_shared<Node_Control_Timer>();
   // auto node_client_vel = std::make_shared<Node_Client_Vel>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node_control_arm_pid_r1);
   // executor.add_node(node_subs_path);
	//executor.add_node(node_subs_positions);
   // executor.add_node(node_control_timer);
    //executor.add_node(node_client_vel);
    executor.spin();

 	rclcpp::shutdown();
  
}