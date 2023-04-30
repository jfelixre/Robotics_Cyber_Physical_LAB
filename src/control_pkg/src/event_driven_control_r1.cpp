#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
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


//Variables estados de robots
int robot1_state=-1;
int robot2_state=-1;


//Variables de posición
float Xobj1=-1;
float Yobj1=-1;
float ANGobj1=-1;
float Xtgt=-1;
float Ytgt=-1;
float ANGtgt=-1;

//Variables para objetivos
int Xgoal=-1;
int Ygoal=-1;
float ANGgoal=-1;
interfaces::msg::RobotObjective RO_msg;

class Event_Driven_Control_R1 : public rclcpp::Node
{
	public:
		Event_Driven_Control_R1() : Node("event_driven_control_r1")
		{

      		state_robot1_publisher = create_publisher<interfaces::msg::RobotState>("robot_1/state",1);
			objective_robot1_publisher = create_publisher<interfaces::msg::RobotObjective>("robot_1/objective",1);
      		


			/////////////////////ROBOT STATE -1/////////////////////////////     CALIBRAR BRAZO
			state_robot1_publisher -> publish(robot1_state);

			RCLCPP_INFO(this->get_logger(), "Iniciando Nodo de Control de Robot 1 y Calibrando Brazo");
			//FALTA CALIBRAR BRAZO
			
			robot1_state = 0;    //Poner en 0 cuando termine la calibracion del brazo.




			///////////////////////ROBOT STATE 0//////////////////////////////  INICIALIZAR BRAZO Y ESPERAR CONEXIÓN DE ROBOT 2
			state_robot1_publisher -> publish(robot1_state);


				RCLCPP_INFO(this->get_logger(), "Inicializando Brazo y Esperando Conexion de Robot 2");

				while (robot2_state<0)
				{
					RCLCPP_INFO(this->get_logger(), "Esperando Conexion de Robot 2");
				}
				
				robot1_state=1;

				RCLCPP_INFO(this->get_logger(), "Iniciando Control de Robot 1");



			///////////////////////ROBOT STATE 1//////////////////////////////  CONTROL FASE 1  Aproximacion al objeto
			state_robot1_publisher -> publish(robot1_state);

			Xgoal=Xobj1-(0.3*cos(ANGobj1));
			Ygoal=Yobj1-(0.3*sin(ANGobj1));
			ANGgoal=ANGobj1;

			RO_msg.point.x = Xgoal;
			RO_msg.point.y = Ygoal;
			RO_msg.angle = ANGgoal;

			objective_robot1_publisher -> publish(RO_msg);


			//cliente control trayectoria




			robot1_state=2;



			///////////////////////ROBOT STATE 2//////////////////////////////  CONTROL FASE 2  Tomar objeto
			state_robot1_publisher -> publish(robot1_state);


			robot1_state=3;


			///////////////////////ROBOT STATE 3//////////////////////////////  CONTROL FASE 3  Retirarse de la base del objeto
			state_robot1_publisher -> publish(robot1_state);


			robot1_state=4;

			///////////////////////ROBOT STATE 4//////////////////////////////  CONTROL FASE 4  Aproximacion objetivo
			state_robot1_publisher -> publish(robot1_state);

			robot1_state=5;


			///////////////////////ROBOT STATE 5//////////////////////////////  CONTROL FASE 5  Soltar objeto
			state_robot1_publisher -> publish(robot1_state);

			robot1_state=6;


			///////////////////////ROBOT STATE 6//////////////////////////////  CONTROL FASE 6  Retirarse de la base objetivo
			state_robot1_publisher -> publish(robot1_state);

			robot1_state=7;

			///////////////////////ROBOT STATE 7//////////////////////////////  CONTROL FASE 7  Regresar a la posicion inicial
			state_robot1_publisher -> publish(robot1_state);

			robot1_state=8;

			///////////////////////ROBOT STATE 8//////////////////////////////  CONTROL FASE 8  Control Terminado
			state_robot1_publisher -> publish(robot1_state);


			/////////////////FIN DE CONTROL///////////////////////////////////////////////////////////


		}


	private:


/*\
		void control_F1()  //Aproximacion a objeto
		{
			Xgoal=Xobj1-(220*cos(ANGobj1)) -10;
			Ygoal=Yobj1-(220*sin(ANGobj1));
			ANGgoal=ANGobj1;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);


			B1goal=1.05;
			B2goal=1.15;
			B3goal=-0.63;
			velG1=1;

			PIDcicleArm();

			RCLCPP_INFO(this->get_logger(), "Error B1 R1 = %f", errorB1);
			RCLCPP_INFO(this->get_logger(), "Error B2 R1 = %f", errorB2);
			RCLCPP_INFO(this->get_logger(), "Error B3 R1 = %f", errorB3);


			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.04)
			{
				error_count++;
			}

			if (error_count>10){
				robot1_state=2;
				error_count=0;
			}
			
		}

		void control_F2()  //Tomar objeto
		{
			Xgoal=Xobj1-(190*cos(ANGobj1))-10;
			Ygoal=Yobj1-(190*sin(ANGobj1));
			ANGgoal=ANGobj1;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			RCLCPP_INFO(this->get_logger(), "Error B1 R1 = %f", errorB1);
			RCLCPP_INFO(this->get_logger(), "Error B2 R1 = %f", errorB2);
			RCLCPP_INFO(this->get_logger(), "Error B3 R1 = %f", errorB3);

			B1goal=1.05;
			B2goal=1.15;
			B3goal=-0.63;
			//velG1=-1;

			

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.04)
			{
				error_count++;
			}

			if (error_count>10){

					velG1=-1;
					i_gripper++;

					if (i_gripper>=1000)
					{
						B1goal=0.785;
						B2goal=0.785;
						B3goal=0;

						//RCLCPP_INFO(this->get_logger(), "Subiendo brazo");
					}

					else
					{
						//RCLCPP_INFO(this->get_logger(), "Cerrando pinza");
					}


				state_robot1_publisher
			}


			PIDcicle();
			PIDcicleArm();


			if (abs(errorB1)<0.09 and abs(errorB2)<0.09 and abs(errorB3)<0.09 and i_gripper>=1000)
			{
				robot1_state=3;
				error_count=0;
				i_gripper=0;
			}
		}

		void control_F3()  //Retirarse de la base del objeto
		{
			Xgoal=Xobj1-(210*cos(ANGobj1));
			Ygoal=Yobj1-(210*sin(ANGobj1));
			ANGgoal=ANGobj1;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			B1goal=0.785;
			B2goal=0.785;
			B3goal=0;	

			PIDcicleArm();

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.04)
			{
				error_count++;
			}

			if (error_count>10){
				robot1_state=4;
				error_count=0;
			}

		}

		void control_F4()  //Aproximación a objetivo
		{
			Xgoal=Xtgt-(210*cos(ANGtgt));
			Ygoal=Ytgt-(210*sin(ANGtgt));
			ANGgoal=ANGtgt;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);


			B1goal=0.9;
			B2goal=0.9;
			B3goal=-0.9;

			PIDcicleArm();


			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.2)
			{
				error_count++;
			}

			if (error_count>10){
				robot1_state=5;
				error_count=0;
			}
		}

		void control_F5()  //Soltar objeto
		{
			Xgoal=Xtgt-(150*cos(ANGtgt));
			Ygoal=Ytgt-(150*sin(ANGtgt));robot2_state=msg->robot_state;, "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.2)
			{
				error_count++;
			}

			if (error_count>10){
				robot1_state=6;
				error_count=0;
			}
		}

		void control_F6()  //Retirarse de la base objetivo
		{
			Xgoal=Xtgt-(250*cos(ANGtgt));
			Ygoal=Ytgt-(250*sin(ANGtgt));
			ANGgoal=ANGtgt;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.2)
			{
				error_count++;
			}

			if (error_count>10){
				robot1_state=7;
				error_count=0;
			}
		}

		void control_F7()  //Regresar a posición inicial
		{
			Xgoal=Xinit;
			Ygoal=Yinit;
			ANGgoal=-pi/2;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.2)
			{
				error_count++;
			}

			if (error_count>10){
				robot1_state=8;
				error_count=0;
			}
		}

		
*/

		rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr state_robot1_publisher;
		rclcpp::Publisher<interfaces::msg::RobotObjective>::SharedPtr objective_robot1_publisher;

};


class Node_Subs_Pose_R1 : public rclcpp::Node
{
	public:
		Node_Subs_Pose_R1() : Node("node_subs_pose_r1")
		{
     		
      		poses_subscriber= create_subscription<interfaces::msg::Positions>(
      			"/positions", 1, std::bind(&Node_Subs_Pose_R1::pose_subs,this,_1));
			
		}

	private:

		void pose_subs(const interfaces::msg::Positions::SharedPtr msg) const
		{
			Xobj1= msg->pos_object1.position.x;
			Yobj1= msg->pos_object1.position.y;
			tf2::Quaternion Obj_quat(msg->pos_object1.orientation.x, msg->pos_object1.orientation.y, msg->pos_object1.orientation.z, msg->pos_object1.orientation.w);
            tf2::Matrix3x3 Obj_m(Obj_quat);
            double Obj_orientation_x, Obj_orientation_y, Obj_orientation_z;
            Obj_m.getRPY(Obj_orientation_x, Obj_orientation_y, Obj_orientation_z);
			ANGobj1= Obj_orientation_z;



			Xtgt=msg->pos_object1.position.x;
			Ytgt=msg->pos_object1.position.y;
			tf2::Quaternion Tgt_quat(msg->pos_target.orientation.x, msg->pos_target.orientation.y, msg->pos_target.orientation.z, msg->pos_target.orientation.w);
            tf2::Matrix3x3 Tgt_m(Tgt_quat);
            double Tgt_orientation_x, Tgt_orientation_y, Tgt_orientation_z;
            Obj_m.getRPY(Obj_orientation_x, Obj_orientation_y, Obj_orientation_z);
			ANGtgt=Obj_orientation_z;
		}



		rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr poses_subscriber;


};

class Node_Subs_State_R2 : public rclcpp::Node
{
	public:
		Node_Subs_State_R2() : Node("node_subs_state_r2")
		{
     		
      		state_robot2_subscriber= create_subscription<interfaces::msg::RobotState>(
      			"/robot_2/state", 1, std::bind(&Node_Subs_State_R2::r2_subs,this,_1));
			
		}

	private:

		void r2_subs(const interfaces::msg::RobotState::SharedPtr msg) const
		{
			robot2_state=msg->robot_state;
		}

		rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr state_robot2_subscriber;

};



int main(int argc, char * argv[])
{

  	rclcpp::init(argc, argv);
  	rclcpp::spin(std::make_shared<Event_Driven_Control_R1>());
	
	auto node = std::make_shared<Event_Driven_Control_R1>();
    auto node_subs_pose_r1 = std::make_shared<Node_Subs_Pose_R1>();
	auto node_subs_state_r2 = std::make_shared<Node_Subs_State_R2>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(node_subs_pose_r1);
	executor.add_node(node_subs_state_r2);
    executor.spin();

 	rclcpp::shutdown();
  
}