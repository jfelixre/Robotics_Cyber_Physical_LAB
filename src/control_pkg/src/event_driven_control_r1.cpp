#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>


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
int Xobj1=-1;
int Yobj1=-1;
float ANGobj1=-1;
int Xtgt=-1;
int Ytgt=-1;
float ANGtgt=-1;

//Variables para objetivos
int Xgoal=-1;
int Ygoal=-1;
float ANGgoal=-1;

class Event_Driven_Control_R1 : public rclcpp::Node
{
	public:
		Event_Driven_Control_R1() : Node("event_driven_control_r1")
		{

      		state_robot1_publisher = create_publisher<interfaces::msg::RobotState>("robot1_state",1);
      		
      		state_robot2_subscriber= create_subscription<interfaces::msg::RobotState>(
      			"/robot2_state", 1, std::bind(&Event_Driven_Control_R1::r2_subs,this,_1));
			
		}

	private:

		void r2_subs(const interfaces::msg::RobotState::SharedPtr msg) const
		{
			robot2_state=msg->robot_state;
		}

		



		void timer_callback()
		{
			if (robot1_state==-1)
			{
				RCLCPP_INFO(this->get_logger(), "Iniciando Nodo de Control de Robot 1 y Calibrando Brazo");

				sumPIDb1=-5;
				sumPIDb2=5;
				sumPIDb3=5;
				//send_vel();

				if(LS_B1_min==1 and LS_B2_max==1 and LS_B3_max==1){
					armcal=1;
				}

				if (armcal==1)
				{
					robot1_state = 0;
					sumPIDb1=0;
					sumPIDb2=0;
					sumPIDb3=0;
					//send_vel();
				}

			}

			if (robot1_state==0)
			{
				RCLCPP_INFO(this->get_logger(), "Inicializando Brazo y Esperando Conexion de Robot 2");

				B1goal=0;
				B2goal=0;
				B3goal=0;

				PIDcicleArm();

				RCLCPP_INFO(this->get_logger(), "Error B1 R1 = %f", errorB1);
				RCLCPP_INFO(this->get_logger(), "Error B2 R1 = %f", errorB2);
				RCLCPP_INFO(this->get_logger(), "Error B3 R1 = %f", errorB3);

				

				if (abs(errorB1)<0.1 and abs(errorB2)<0.1 and abs(errorB3)<0.1)
				{
					arminit=1;
				}

				if (arminit==1)
				{
					if (robot2_state==0 or robot2_state==1)
					{
						robot1_state=1;
						RCLCPP_INFO(this->get_logger(), "Iniciando Control de Robot 1");
						Xinit=Xrobot1;
						Yinit=Yrobot1;
						ANGinit=ANGrobot1;
					}
				}

				//send_vel();
				
			}

			if (robot1_state==1)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 1 en Proceso");
				control_F1();			//Aproximación a objeto
			}

			if (robot1_state==2)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 2 en Proceso");
				control_F2();			//Tomar objeto
			}

			if (robot1_state==3)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 3 en Proceso");
				control_F3();			//Retirarse de la base del objeto
			}

			if (robot1_state==4)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 4 en Proceso");
				control_F4();			//Aproximación a objetivo
			}

			if (robot1_state==5)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 5 en Proceso");
				control_F5();			//Soltar objeto
			}

			if (robot1_state==6)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 6 en Proceso");
				control_F6();			//Retirarse de la base objetivo
			}

			if (robot1_state==7)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 7 en Proceso");
				control_F7();			//Regresar a posición inicial
			}

			if (robot1_state==8)
			{
				RCLCPP_INFO(this->get_logger(), "Control Plataforma 1 Terminado");
				velx=0;
				vely=0;
				velang=0;
				
				sumPIDx=0;
				sumPIDy=0;
				sumPIDang=0;

				send_vel();
			}


			conv_world_2_robot();
			send_vel();



			 interfaces::msg::RobotState msg_publ;
			 msg_publ.robot_state=robot1_state;	
			 state_robot1_publisher->publish(msg_publ);

			 save_data();
			 //RCLCPP_INFO(this->get_logger(), "Fase %d", robot1_state);
		}


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
			Ygoal=Ytgt-(150*sin(ANGtgt));
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

		




		rclcpp::Subscription<interfaces::msg::ImgData>::SharedPtr subs_img_data;
		rclcpp::TimerBase::SharedPtr timer_;
		size_t count_;
		rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr state_robot1_publisher;
		rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr state_robot2_subscriber;

};


class Event_Driven_Control_R1 : public rclcpp::Node
{
	public:
		Event_Driven_Control_R1() : Node("event_driven_control_r1")
		{

      		state_robot1_publisher = create_publisher<interfaces::msg::RobotState>("robot1_state",1);
      		
      		state_robot2_subscriber= create_subscription<interfaces::msg::RobotState>(
      			"/robot2_state", 1, std::bind(&Event_Driven_Control_R1::r2_subs,this,_1));
			
		}

	private:

		void r2_subs(const interfaces::msg::RobotState::SharedPtr msg) const
		{
			robot2_state=msg->robot_state;
		}

};

class Node_Subs_State_R2 : public rclcpp::Node
{
	public:
		Node_Subs_State_R2() : Node("node_subs_state_r2")
		{
     		
      		state_robot2_subscriber= create_subscription<interfaces::msg::RobotState>(
      			"/robot2_state", 1, std::bind(&Node_Subs_State_R2::r2_subs,this,_1));
			
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