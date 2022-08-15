#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/robot_vel.hpp"
#include <interfaces/msg/img_data.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/control_data.hpp>
#include <interfaces/msg/limit_switch.hpp>
#include <interfaces/msg/motor_vels_w_arm.hpp>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>

using std::placeholders::_1;
using namespace std::chrono_literals;

std::ofstream myfile;

auto request = std::make_shared<interfaces::srv::RobotVel::Request>();

//Variables para recibir datos
int r1x_msg,r1y_msg,r2x_msg,r2y_msg,obj1x_msg,obj1y_msg,obj2x_msg,obj2y_msg,tgtx_msg,tgty_msg;
float r1ang_msg,r2ang_msg,obj1ang_msg,obj2ang_msg,tgtang_msg;

//Variables estados de robots
int robot1_state=-1;
int robot2_state=-1;


//Variables de posición
int Xrobot1=-1;
int Yrobot1=-1;
float ANGrobot1=-1;
int Xobj1=-1;
int Yobj1=-1;
float ANGobj1=-1;
int Xtgt=-1;
int Ytgt=-1;
float ANGtgt=-1;

int Xinit= -1;
int Yinit= -1;
float ANGinit= -1;

//Variables para objetivos
int Xgoal=-1;
int Ygoal=-1;
float ANGgoal=-1;

float B1goal=0;
float B2goal=0;
float B3goal=0;

//Variables PID
float KPx=2;
float KIx=0;
float KDx=0.1;

float KPy=2;
float KIy=0;
float KDy=0.1;

float KPang=1;
float KIang=0;
float KDang=0.1;

float Px=0;
float Ix=0;
float Dx=0;

float Py=0;
float Iy=0;
float Dy=0;

float Pang=0;
float Iang=0;
float Dang=0;

float sumPIDx=0;
float sumPIDy=0;
float sumPIDang=0;

float velx=0;
float vely=0;
float velang=0;

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
int errorX=0;
int errorX_ant1=0;
int errorX_ant2=0;
float INTerrorX=0;
float DERerrorX=0;

int errorY=0;
int errorY_ant1=0;
int errorY_ant2=0;
float INTerrorY=0;
float DERerrorY=0;

float errorAng=0;
float errorAng_ant1=0;
float errorAng_ant2=0;
float INTerrorAng=0;
float DERerrorAng=0;

int error_count=0;

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


//Variables de tiempo
//auto time = std::chrono::system_clock::now();
//auto end_time = std::chrono::system_clock::now();
//std::chrono::duration<double> dt = end_time - start_time;
auto dt = 50ms;
float dt_sec = 0.05;

float pi= 3.1416;

//Variables LimitSwitch
int LS_B1_min, LS_B1_max, LS_B2_min, LS_B2_max, LS_B3_min, LS_B3_max;

//Variables velocidades de encoders
float vel_M1,vel_M2,vel_M3,vel_M4,vel_B1,vel_B2,vel_B3,vel_G1;

//Variables de posición de brazo
float B1_pos=0;
float B2_pos=0;
float B3_pos=0;
float velG1=1;

int armcal=0;
int arminit=0;
int i_gripper=0;

class Control_PID_R1_node : public rclcpp::Node
{
	public:
		Control_PID_R1_node()
		: Node("control_pid_r1_node"), count_(0)
		{

			client = this->create_client<interfaces::srv::RobotVel>("robot1_vel");

			subs_img_data = this->create_subscription<interfaces::msg::ImgData>(
				"/image_data", 1, std::bind(&Control_PID_R1_node::subs_callback,this, _1));

			timer_ = this->create_wall_timer(
      			dt, std::bind(&Control_PID_R1_node::timer_callback, this));

      		state_robot1_publisher = create_publisher<interfaces::msg::RobotState>("robot1_state",1);
      		
      		state_robot2_subscriber= create_subscription<interfaces::msg::RobotState>(
      			"/robot2_state", 1, std::bind(&Control_PID_R1_node::r2_subs,this,_1));
			
			data_robot_publisher = create_publisher<interfaces::msg::ControlData>("robot1_control_data",1);

			LmSw_subscriber= create_subscription<interfaces::msg::LimitSwitch>(
      			"/robot_1/limit_switch", 1, std::bind(&Control_PID_R1_node::LmSw_subs,this,_1));

			velM_subscriber= create_subscription<interfaces::msg::MotorVelsWArm>(
      			"/robot_1/sim_vel_Motors", 1, std::bind(&Control_PID_R1_node::velM_subs,this,_1));

		}

	private:

		void subs_callback(const interfaces::msg::ImgData::SharedPtr data_msg) const
		{
			//RCLCPP_INFO(this->get_logger(), "Data received");
			r1x_msg=data_msg->r1_x;
			r1y_msg=data_msg->r1_y;
			r1ang_msg=data_msg->r1_ang;

			r2x_msg=data_msg->r2_x;
			r2y_msg=data_msg->r2_y;
			r2ang_msg=data_msg->r2_ang;

			obj1x_msg=data_msg->obj1_x;
			obj1y_msg=data_msg->obj1_y;
			obj1ang_msg=data_msg->obj1_ang;

			obj2x_msg=data_msg->obj2_x;
			obj2y_msg=data_msg->obj2_y;
			obj2ang_msg=data_msg->obj2_ang;

			tgtx_msg=data_msg->tgt_x;
			tgty_msg=data_msg->tgt_y;
			tgtang_msg=data_msg->tgt_ang;



			//Falta realizar codigo para evitar datos incorrectos.
			Xrobot1=r1x_msg;
			Yrobot1=r1y_msg;
			ANGrobot1=r1ang_msg;

			Xobj1=obj1x_msg;
			Yobj1=obj1y_msg;
			ANGobj1=obj1ang_msg;

			Xtgt=tgtx_msg;
			Ytgt=tgty_msg;
			ANGtgt=tgtang_msg;

		}

		void r2_subs(const interfaces::msg::RobotState::SharedPtr msg) const
		{
			robot2_state=msg->robot_state;
		}

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

		void velM_subs(const interfaces::msg::MotorVelsWArm::SharedPtr msg) const
		{
			vel_M1=msg->vel_m1;
			vel_M2=msg->vel_m2;
			vel_M3=msg->vel_m3;
			vel_M4=msg->vel_m4;
			vel_B1=msg->vel_b1;
			vel_B2=msg->vel_b2;
			vel_B3=msg->vel_b3;
			vel_G1=msg->vel_g1;

			vel_B2=vel_B2*-1;
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

		void PIDcicle()
		{
			//RCLCPP_INFO(this->get_logger(), "Entro a PIDcicle");
			//Cálculo de errores
			errorX = Xgoal - Xrobot1;
			errorY = Ygoal - Yrobot1;
			errorAng = ANGgoal - ANGrobot1;


			//Calculo de PID X

			Px = KPx * errorX;

			INTerrorX= (errorX + errorX_ant1 + errorX_ant2) * dt_sec;
			Ix = KIx * INTerrorX;

			DERerrorX=(errorX - errorX_ant1)/dt_sec;
			Dx=DERerrorX*KDx;

			errorX_ant2 = errorX_ant1;
			errorX_ant1 = errorX;

			sumPIDx = Px + Ix + Dx;

			if (sumPIDx>200) {sumPIDx=200;}
			if (sumPIDx<-200) {sumPIDx=-200;}



			//Calculo de PID Y

			Py = KPy * errorY;

			INTerrorY= (errorY + errorY_ant1 + errorY_ant2) * dt_sec;
			Iy = KIy * INTerrorY;

			DERerrorY=(errorY - errorY_ant1)/dt_sec;
			Dy=DERerrorY*KDy;

			errorY_ant2 = errorY_ant1;
			errorY_ant1 = errorY;

			sumPIDy = Py + Iy + Dy;

			if (sumPIDy>200) {sumPIDy=200;}
			if (sumPIDy<-200) {sumPIDy=-200;}


			//Calculo de PID Ang

			Pang = KPang * errorAng;

			INTerrorAng= (errorAng + errorAng_ant1 + errorAng_ant2) * dt_sec;
			Iang = KIang * INTerrorAng;

			DERerrorAng=(errorAng - errorAng_ant1)/dt_sec;
			Dang=DERerrorAng*KDang;

			errorAng_ant2 = errorAng_ant1;
			errorAng_ant1 = errorAng;

			sumPIDang = Pang + Iang + Dang;

			if (sumPIDang>3) {sumPIDang=3;}
			if (sumPIDang<-3) {sumPIDang=-3;}

		}

		void PIDcicleArm()
		{
			//Cálculo de posiciones
			//B1_pos=B1_pos+(vel_B1*dt_sec);
			B1_pos=vel_B1+0.4;
			//B2_pos=B2_pos+(vel_B2*dt_sec);
			B2_pos=(vel_B2+0.65)*-1;
			//B3_pos=B3_pos+(vel_B3*dt_sec);
			B3_pos=vel_B3+0.5;

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

		void conv_world_2_robot()
		{
			//RCLCPP_INFO(this->get_logger(), "Entro a conv");
			float z = sqrt((sumPIDx*sumPIDx)+(sumPIDy*sumPIDy));
			float Beta = atan2(sumPIDy,sumPIDx);
			float Gamma = ANGrobot1 - (pi/2);
			float Delta = Beta-Gamma;

			velx = z * cos(Delta);
			vely = z * sin(Delta);
			velang = sumPIDang;
		}

		void send_vel()
		{
			//RCLCPP_INFO(this->get_logger(), "Entro a send_vel");
			request->x_vel = velx * -1;
			request->y_vel = vely;
			request->ang_vel = velang;
			request->b1_vel = sumPIDb1;
			request->b2_vel = sumPIDb2 * -1;
			request->b3_vel = sumPIDb3;
			request->g1_vel = velG1;


  		auto result = client->async_send_request(request);
  		//RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Robot 1 server");

		}

		void save_data()
		{
			interfaces::msg::ControlData ctrl_data_msg;

			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

			ctrl_data_msg.time = time;
			ctrl_data_msg.fase = robot1_state;
			ctrl_data_msg.x_robot = Xrobot1;
			ctrl_data_msg.y_robot = Yrobot1;
			ctrl_data_msg.ang_robot = ANGrobot1;
			ctrl_data_msg.x_obj = Xobj1;
			ctrl_data_msg.y_obj = Yobj1;
			ctrl_data_msg.ang_obj = ANGobj1;
			ctrl_data_msg.x_tgt = Xtgt;
			ctrl_data_msg.y_tgt = Ytgt;
			ctrl_data_msg.ang_tgt = ANGtgt;
			ctrl_data_msg.x_goal = Xgoal;
			ctrl_data_msg.y_goal = Ygoal;
			ctrl_data_msg.ang_goal = ANGgoal;
			ctrl_data_msg.x_error = errorX;
			ctrl_data_msg.y_error = errorY;
			ctrl_data_msg.ang_error = errorAng;
			ctrl_data_msg.vel_x_world = sumPIDx;
			ctrl_data_msg.vel_y_world = sumPIDy;
			ctrl_data_msg.vel_ang_world = sumPIDang;
			ctrl_data_msg.vel_x_robot = velx * -1;
			ctrl_data_msg.vel_y_robot = vely;
			ctrl_data_msg.vel_ang_robot = velang;

			data_robot_publisher->publish(ctrl_data_msg);

			myfile << time;
			myfile << "_";
			myfile << robot1_state;
			myfile << "_";
			myfile << Xrobot1;
			myfile << "_";
			myfile << Yrobot1;
			myfile << "_";
			myfile << ANGrobot1;
			myfile << "_";
			myfile << Xobj1;
			myfile << "_";
			myfile << Yobj1;
			myfile << "_";
			myfile << ANGobj1;
			myfile << "_";
			myfile << Xtgt;
			myfile << "_";
			myfile << Ytgt;
			myfile << "_";
			myfile << ANGtgt;
			myfile << "_";
			myfile << Xgoal;
			myfile << "_";
			myfile << Ygoal;
			myfile << "_";
			myfile << ANGgoal;
			myfile << "_";
			myfile << errorX;
			myfile << "_";
			myfile << errorY;
			myfile << "_";
			myfile << errorAng;
			myfile << "_";
			myfile << sumPIDx;
			myfile << "_";
			myfile << sumPIDy;
			myfile << "_";
			myfile << sumPIDang;
			myfile << "_";
			myfile << velx * -1;
			myfile << "_";
			myfile << vely;
			myfile << "_";
			myfile << velang;
			myfile << "\n";


		}




		rclcpp::Subscription<interfaces::msg::ImgData>::SharedPtr subs_img_data;
		rclcpp::TimerBase::SharedPtr timer_;
		size_t count_;
		rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr state_robot1_publisher;
		rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr state_robot2_subscriber;

		rclcpp::Client<interfaces::srv::RobotVel>::SharedPtr client;

		rclcpp::Publisher<interfaces::msg::ControlData>::SharedPtr data_robot_publisher;

		rclcpp::Subscription<interfaces::msg::LimitSwitch>::SharedPtr LmSw_subscriber;

		rclcpp::Subscription<interfaces::msg::MotorVelsWArm>::SharedPtr velM_subscriber;

};




int main(int argc, char * argv[])
{
	myfile.open ("Data_P1.csv");
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<Control_PID_R1_node>());

  rclcpp::shutdown();
  myfile.close();
}