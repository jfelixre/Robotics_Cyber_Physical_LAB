#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"
#include <interfaces/msg/img_data.hpp>
#include <interfaces/srv/control_pid_start.hpp>
#include <interfaces/msg/plat_state.hpp>
#include <interfaces/msg/control_data.hpp>

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

auto request = std::make_shared<interfaces::srv::PlatformVel::Request>();

//Variables para recibir datos
int r1x_msg,r1y_msg,r2x_msg,r2y_msg,obj1x_msg,obj1y_msg,obj2x_msg,obj2y_msg,tgtx_msg,tgty_msg;
float r1ang_msg,r2ang_msg,obj1ang_msg,obj2ang_msg,tgtang_msg;

//Variables estados de robots
int platform1_state=-1;
int platform2_state=-1;


//Variables de posición
int Xrobot2=-1;
int Yrobot2=-1;
float ANGrobot2=-1;
int Xobj2=-1;
int Yobj2=-1;
float ANGobj2=-1;
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


//Variables PID
float KPx=2;
float KIx=0;
float KDx=0.1;

float KPy=2;
float KIy=0;
float KDy=0.1;

float KPang=1;
float KIang=0;
float KDang=0.01;

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


//Variables para control
int errorX=0;
int errorX_ant1=0;
int errorX_ant2=0;
float INTerrorX=0;

int errorY=0;
int errorY_ant1=0;
int errorY_ant2=0;
float INTerrorY=0;

float errorAng=0;
float errorAng_ant1=0;
float errorAng_ant2=0;
float INTerrorAng=0;

int error_count=0;


//Variables de tiempo
//auto start_time = std::chrono::system_clock::now();
//auto end_time = std::chrono::system_clock::now();
//std::chrono::duration<double> dt = end_time - start_time;
auto dt = 50ms;
float dt_sec = 0.05;

float pi= 3.1416;



class Control_PID_P2_node : public rclcpp::Node
{
	public:
		Control_PID_P2_node()
		: Node("control_pid_p2_node"), count_(0)
		{

			client = this->create_client<interfaces::srv::PlatformVel>("platform2_vel");

			subs_img_data = this->create_subscription<interfaces::msg::ImgData>(
				"/image_data", 1, std::bind(&Control_PID_P2_node::subs_callback,this, _1));

			timer_ = this->create_wall_timer(
      dt, std::bind(&Control_PID_P2_node::timer_callback, this));

      state_platform2_publisher = create_publisher<interfaces::msg::PlatState>("platform2_state",1);
      state_platform1_subscriber= create_subscription<interfaces::msg::PlatState>(
      	"/platform1_state", 1, std::bind(&Control_PID_P2_node::p1_subs,this,_1));

      data_platform_publisher = create_publisher<interfaces::msg::ControlData>("platform2_control_data",1);
			
			
		}

	private:

		void subs_callback(const interfaces::msg::ImgData::SharedPtr data_msg) const
		{
			RCLCPP_INFO(this->get_logger(), "Data received");
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
			Xrobot2=r2x_msg;
			Yrobot2=r2y_msg;
			ANGrobot2=r2ang_msg;

			Xobj2=obj2x_msg;
			Yobj2=obj2y_msg;
			ANGobj2=obj2ang_msg;

			Xtgt=tgtx_msg;
			Ytgt=tgty_msg;
			ANGtgt=tgtang_msg;

		}

		void p1_subs(const interfaces::msg::PlatState::SharedPtr msg) const
		{
			platform1_state=msg->platform_state;
		}

		void timer_callback()
		{
			if (platform2_state==-1)
			{
				RCLCPP_INFO(this->get_logger(), "Iniciando Nodo de Control de Plataforma 2");
				platform2_state = 0;
			}

			if (platform2_state==0)
			{
				RCLCPP_INFO(this->get_logger(), "Esperando Conexion de Plataforma 1");

				if (platform1_state==0 or platform1_state==1)
				{
					
					RCLCPP_INFO(this->get_logger(), "Iniciando Control de Plataforma 2");
					Xinit=Xrobot2;
					Yinit=Yrobot2;
					ANGinit=ANGrobot2;
				}

				if (platform1_state>=4)
				{
					platform2_state=1;
				}

			}

			if (platform2_state==1)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 1 en Proceso");
				control_F1();			//Aproximación a objeto
			}

			if (platform2_state==2)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 2 en Proceso");
				control_F2();			//Tomar objeto
			}

			if (platform2_state==3)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 3 en Proceso");
				control_F3();			//Retirarse de la base del objeto
			}

			if (platform2_state==4)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 4 en Proceso");
				control_F4();			//Aproximación a objetivo
			}

			if (platform2_state==5)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 5 en Proceso");
				control_F5();			//Soltar objeto
			}

			if (platform2_state==6)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 6 en Proceso");
				control_F6();			//Retirarse de la base objetivo
			}

			if (platform2_state==7)
			{
				RCLCPP_INFO(this->get_logger(), "Control Fase 7 en Proceso");
				control_F7();			//Regresar a posición inicial
			}

			if (platform2_state==8)
			{
				RCLCPP_INFO(this->get_logger(), "Control Plataforma 2 Terminado");
				velx=0;
				vely=0;
				velang=0;
				send_vel();
				sumPIDx=0;
				sumPIDy=0;
				sumPIDang=0;
			}


			conv_world_2_robot();
			send_vel();



			 interfaces::msg::PlatState msg_publ;
			 msg_publ.platform_state=platform2_state;	
			 state_platform2_publisher->publish(msg_publ);

			 save_data();
			 //RCLCPP_INFO(this->get_logger(), "Fase %d", platform1_state);
		}


		void control_F1()  //Aproximacion a objeto
		{
			Xgoal=Xobj2-(190*cos(ANGobj2));
			Ygoal=Yobj2-(190*sin(ANGobj2));
			ANGgoal=ANGobj2;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.04)
			{
				error_count++;
			}

			if (error_count>10){
				platform2_state=2;
				error_count=0;
			}
			
		}

		void control_F2()  //Tomar objeto
		{
			Xgoal=Xobj2-(130*cos(ANGobj2));
			Ygoal=Yobj2-(130*sin(ANGobj2));
			ANGgoal=ANGobj2;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.04)
			{
				error_count++;
			}

			if (error_count>10){
				platform2_state=3;
				error_count=0;
			}
		}

		void control_F3()  //Retirarse de la base del objeto
		{
			Xgoal=Xobj2-(200*cos(ANGobj2));
			Ygoal=Yobj2-(200*sin(ANGobj2));
			ANGgoal=ANGobj2;
			//RCLCPP_INFO(this->get_logger(), "Entro a control_F1");
			PIDcicle();
			RCLCPP_INFO(this->get_logger(), "Error X R1 = %d", errorX);
			RCLCPP_INFO(this->get_logger(), "Error Y R1 = %d", errorY);
			RCLCPP_INFO(this->get_logger(), "Error Ang R1 = %f", errorAng);

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.04)
			{
				error_count++;
			}

			if (error_count>10 && platform1_state>=7){
				platform2_state=4;
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

			if (abs(errorX)<8 && abs(errorY)<8 && abs(errorAng)<0.2)
			{
				error_count++;
			}

			if (error_count>10){
				platform2_state=5;
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
				platform2_state=6;
				error_count=0;
			}
		}

		void control_F6()  //Retirarse de la base objetivo
		{
			Xgoal=Xtgt-(200*cos(ANGtgt));
			Ygoal=Ytgt-(200*sin(ANGtgt));
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
				platform2_state=7;
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
				platform2_state=8;
				error_count=0;
			}
		}

		void PIDcicle()
		{
			//RCLCPP_INFO(this->get_logger(), "Entro a PIDcicle");
			//Cálculo de errores
			errorX = Xgoal - Xrobot2;
			errorY = Ygoal - Yrobot2;
			errorAng = ANGgoal - ANGrobot2;


			//Calculo de PID X

			Px = KPx * errorX;

			INTerrorX= (errorX + errorX_ant1 + errorX_ant2) * dt_sec;
			Ix = KIx * INTerrorX;

			Dx=(errorX - errorX_ant1)/dt_sec;

			errorX_ant2 = errorX_ant1;
			errorX_ant1 = errorX;

			sumPIDx = Px + Ix + Dx;

			if (sumPIDx>200) {sumPIDx=200;}
			if (sumPIDx<-200) {sumPIDx=-200;}



			//Calculo de PID Y

			Py = KPy * errorY;

			INTerrorY= (errorY + errorY_ant1 + errorY_ant2) * dt_sec;
			Iy = KIy * INTerrorY;

			Dy=(errorY - errorY_ant1)/dt_sec;

			errorY_ant2 = errorY_ant1;
			errorY_ant1 = errorY;

			sumPIDy = Py + Iy + Dy;

			if (sumPIDy>200) {sumPIDy=200;}
			if (sumPIDy<-200) {sumPIDy=-200;}


			//Calculo de PID Ang

			Pang = KPang * errorAng;

			INTerrorAng= (errorAng + errorAng_ant1 + errorAng_ant2) * dt_sec;
			Iang = KIang * INTerrorAng;

			Dang=(errorAng - errorAng_ant1)/dt_sec;

			errorAng_ant2 = errorAng_ant1;
			errorAng_ant1 = errorAng;

			sumPIDang = Pang + Iang + Dang;

			if (sumPIDang>3) {sumPIDang=3;}
			if (sumPIDang<-3) {sumPIDang=-3;}

		}

		void conv_world_2_robot()
		{
			//RCLCPP_INFO(this->get_logger(), "Entro a conv");
			float z = sqrt((sumPIDx*sumPIDx)+(sumPIDy*sumPIDy));
			float Beta = atan2(sumPIDy,sumPIDx);
			float Gamma = ANGrobot2 - (pi/2);
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

  		auto result = client->async_send_request(request);
  		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Platform 2 server");

		}


		void save_data()
		{
			interfaces::msg::ControlData ctrl_data_msg;

			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

			ctrl_data_msg.time = time;
			ctrl_data_msg.fase = platform2_state;
			ctrl_data_msg.x_robot = Xrobot2;
			ctrl_data_msg.y_robot = Yrobot2;
			ctrl_data_msg.ang_robot = ANGrobot2;
			ctrl_data_msg.x_obj = Xobj2;
			ctrl_data_msg.y_obj = Yobj2;
			ctrl_data_msg.ang_obj = ANGobj2;
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

			data_platform_publisher->publish(ctrl_data_msg);

			myfile << time;
			myfile << "_";
			myfile << platform2_state;
			myfile << "_";
			myfile << Xrobot2;
			myfile << "_";
			myfile << Yrobot2;
			myfile << "_";
			myfile << ANGrobot2;
			myfile << "_";
			myfile << Xobj2;
			myfile << "_";
			myfile << Yobj2;
			myfile << "_";
			myfile << ANGobj2;
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
		rclcpp::Publisher<interfaces::msg::PlatState>::SharedPtr state_platform2_publisher;
		rclcpp::Subscription<interfaces::msg::PlatState>::SharedPtr state_platform1_subscriber;

		rclcpp::Client<interfaces::srv::PlatformVel>::SharedPtr client;

		rclcpp::Publisher<interfaces::msg::ControlData>::SharedPtr data_platform_publisher;


};




int main(int argc, char * argv[])
{
	myfile.open ("Data_P2.csv");
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<Control_PID_P2_node>());

  rclcpp::shutdown();
  myfile.close();
}