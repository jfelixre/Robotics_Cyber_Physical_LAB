#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/srv/event_control.hpp>
#include <interfaces/msg/arm_joints_positions.hpp>
#include <interfaces/msg/trajectory_control.hpp>

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


//Variables estados de robots
interfaces::msg::RobotState robot2_state;
//int32_t robot1_state=-1;
int32_t robot1_state=-1;


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

//Variables posicion brazo
interfaces::msg::ArmJointsPositions arm_joints_position;
//auto request_ctl = std::make_shared<interfaces::srv::TrajectoryControl::Request>();
interfaces::msg::TrajectoryControl trajectory_msg;



class Event_Driven_Control_R2 : public rclcpp::Node
{
	public:
		Event_Driven_Control_R2() : Node("event_driven_control_r2")
		{

      		state_robot2_publisher = create_publisher<interfaces::msg::RobotState>("robot_2/state", 10);

			objective_robot2_publisher = create_publisher<interfaces::msg::RobotObjective>("robot_2/objective", 1);

			service_event_control = this->create_service<interfaces::srv::EventControl>(
                "event_control_r2", std::bind(&Event_Driven_Control_R2::event_control_handler, this,
                std::placeholders::_1, std::placeholders::_2));

      		publisher_arm_pos = this->create_publisher<interfaces::msg::ArmJointsPositions>("/robot_2/set_arm_joints_position",10);
			
			//client_cb_group = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

			publisher_trajectory_ctrl = this->create_publisher<interfaces::msg::TrajectoryControl>("/robot_2/trajectory_control",10);
			

		}


	private:

		rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr state_robot2_publisher;
		rclcpp::Publisher<interfaces::msg::RobotObjective>::SharedPtr objective_robot2_publisher;
		rclcpp::Service<interfaces::srv::EventControl>::SharedPtr service_event_control;
		rclcpp::Publisher<interfaces::msg::ArmJointsPositions>::SharedPtr publisher_arm_pos;
		rclcpp::Publisher<interfaces::msg::TrajectoryControl>::SharedPtr publisher_trajectory_ctrl;
        //rclcpp::CallbackGroup::SharedPtr client_cb_group;

		void event_control_handler(const std::shared_ptr<interfaces::srv::EventControl::Request> request,
			std::shared_ptr<interfaces::srv::EventControl::Response>      response)
			{
				
				/////////////////////ROBOT STATE -1/////////////////////////////     CALIBRAR BRAZO
				state_robot2_publisher -> publish(robot2_state);

				RCLCPP_INFO(this->get_logger(), "Iniciando Nodo de Control de Robot 2 y Calibrando Brazo");
					
				//Home calibración brazo
				arm_joints_position.pos_b1 = 2;
				arm_joints_position.pos_b2 = -2;
				arm_joints_position.pos_b3 = 2;
				arm_joints_position.pos_g1 = 1;

				publisher_arm_pos->publish(arm_joints_position);
				
				
				robot2_state.robot_state= 0;    //Poner en 0 cuando termine la calibracion del brazo.



	
				///////////////////////ROBOT STATE 0//////////////////////////////  INICIALIZAR BRAZO Y ESPERAR CONEXIÓN DE ROBOT 2
				state_robot2_publisher -> publish(robot2_state);


					RCLCPP_INFO(this->get_logger(), "Inicializando Brazo y Esperando Conexion de Robot 2");
					
					RCLCPP_INFO(this->get_logger(), "Esperando Conexion de Robot 1");

					while (robot1_state<0)
					{
						
					}

					RCLCPP_INFO(this->get_logger(), "Iniciando Control de Robot 2");

					RCLCPP_INFO(this->get_logger(), "Esperando completar tarea de Robot 1");

					while (robot1_state<=3)
					{
						RCLCPP_INFO(this->get_logger(), "Esperando completar tarea de Robot 1");
					}
					
					robot2_state.robot_state=1;






				///////////////////////ROBOT STATE 1//////////////////////////////  CONTROL FASE 1  Aproximacion al objeto
				state_robot2_publisher -> publish(robot2_state);

				std::cout << "FASE 1" << std::endl;


				/*
				Xgoal=Xobj1-(0.3*cos(ANGobj1));
				Ygoal=Yobj1-(0.3*sin(ANGobj1));
				ANGgoal=ANGobj1;

				RO_msg.point.x = Xgoal;
				RO_msg.point.y = Ygoal;
				RO_msg.angle = ANGgoal;
				*/

				RO_msg.objective = 1;  //Objeto 1
				RO_msg.distance = 25; //Distancia al objeto

				objective_robot2_publisher -> publish(RO_msg);

				
				//control trayectoria
				// request_ctl->time = 140;

				// while (!client_trajectory_ctrl->wait_for_service(1s)){
               // if (!rclcpp::ok()){
               //     RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
               // }
               // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          	   // }
				
				// auto result = client_trajectory_ctrl->async_send_request(request_ctl);
				
				// result.wait();

				// bool result_control = result.get()->success;

				// std::cout << " Result control" << std::endl;

				trajectory_msg.time = 140;

				publisher_trajectory_ctrl->publish(trajectory_msg);
				rclcpp::sleep_for(std::chrono::seconds(trajectory_msg.time));

				robot2_state.robot_state=2;



				///////////////////////ROBOT STATE 2//////////////////////////////  CONTROL FASE 2  Tomar objeto
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 2" << std::endl;

				//Posicion del brazo
				arm_joints_position.pos_b1 = 0.9;
				arm_joints_position.pos_b2 = 0.5;
				arm_joints_position.pos_b3 = -0.4;
				arm_joints_position.pos_g1 = 1;

				publisher_arm_pos->publish(arm_joints_position);
				rclcpp::sleep_for(std::chrono::seconds(45));


				//Control last distance

				RO_msg.objective = 1;  //Objeto 1
				RO_msg.distance = 16; //Distancia al objeto

				objective_robot2_publisher -> publish(RO_msg);

				
				//cliente control trayectoria
				// request_ctl->time = 30;

				// while (!client_trajectory_ctrl->wait_for_service(1s)){
                // if (!rclcpp::ok()){
                //     RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
                // }
                // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          		// }
				
				// auto result2 = client_trajectory_ctrl->async_send_request(request_ctl);
				
				// result2.wait();

				// result_control = result.get()->success;

				trajectory_msg.time = 30;

				publisher_trajectory_ctrl->publish(trajectory_msg);
				rclcpp::sleep_for(std::chrono::seconds(trajectory_msg.time));
				
				//Cerrar brazo
				arm_joints_position.pos_b1 = 0.9;
				arm_joints_position.pos_b2 = 0.5;
				arm_joints_position.pos_b3 = -0.4;
				arm_joints_position.pos_g1 = -1;

				publisher_arm_pos->publish(arm_joints_position);
				rclcpp::sleep_for(std::chrono::seconds(30));

				arm_joints_position.pos_b1 = 0.9;
				arm_joints_position.pos_b2 = -0.1;
				arm_joints_position.pos_b3 = 0.1;
				arm_joints_position.pos_g1 = -1;

				publisher_arm_pos->publish(arm_joints_position);
				rclcpp::sleep_for(std::chrono::seconds(40));


				robot2_state.robot_state=3;


				///////////////////////ROBOT STATE 3//////////////////////////////  CONTROL FASE 3  Retirarse de la base del objeto
				
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 3" << std::endl;


				objective_robot2_publisher -> publish(RO_msg);

				arm_joints_position.pos_b1 = 1.2;
				arm_joints_position.pos_b2 = -1.2;
				arm_joints_position.pos_b3 = 1.2;
				arm_joints_position.pos_g1 = -1;

				publisher_arm_pos->publish(arm_joints_position);
				rclcpp::sleep_for(std::chrono::seconds(1));

				RO_msg.objective = 3;  //Objeto 1 
				RO_msg.distance = 25; //Distancia al objeto



				
				


				//cliente control trayectoria
				//request_ctl->time = 1;

				while (!client_trajectory_ctrl->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          		}
				
				auto result3 = client_trajectory_ctrl->async_send_request(request_ctl);
				
				result3.wait();

				result_control = result.get()->success;

				robot2_state.robot_state=4;

				///////////////////////ROBOT STATE 4//////////////////////////////  CONTROL FASE 4  Aproximacion objetivo
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 4" << std::endl;

				RO_msg.objective = 2;  //Objeto 2
				RO_msg.distance = 25; //Distancia al objeto



				objective_robot2_publisher -> publish(RO_msg);

				


				//cliente control trayectoria
				request_ctl->time = 100;

				while (!client_trajectory_ctrl->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          		}
				
				auto result4 = client_trajectory_ctrl->async_send_request(request_ctl);
				
				result4.wait();

				result_control = result.get()->success;

		
				robot2_state.robot_state=5;


				///////////////////////ROBOT STATE 5//////////////////////////////  CONTROL FASE 5  Soltar objeto
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 5" << std::endl;

				//Posicion del brazo
				arm_joints_position.pos_b1 = 2;
				arm_joints_position.pos_b2 = -2;
				arm_joints_position.pos_b3 = 1.2;
				arm_joints_position.pos_g1 = -1;

				publisher_arm_pos->publish(arm_joints_position);

				
				//Control last distance
				RO_msg.objective = 2;  //Objeto 2
				RO_msg.distance = 10; //Distancia al objeto

				objective_robot2_publisher -> publish(RO_msg);

				//cliente control trayectoria
				request_ctl->time = 30;

				while (!client_trajectory_ctrl->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          		}
				
				auto result5 = client_trajectory_ctrl->async_send_request(request_ctl);
				
				result5.wait();

				result_control = result.get()->success;


				arm_joints_position.pos_b1 = 2;
				arm_joints_position.pos_b2 = -2;
				arm_joints_position.pos_b3 = 2;
				arm_joints_position.pos_g1 = -1;

				publisher_arm_pos->publish(arm_joints_position);

				rclcpp::sleep_for(std::chrono::seconds(2));

				arm_joints_position.pos_b1 = 2;
				arm_joints_position.pos_b2 = -2;
				arm_joints_position.pos_b3 = 2;
				arm_joints_position.pos_g1 = 1;

				publisher_arm_pos->publish(arm_joints_position);

				rclcpp::sleep_for(std::chrono::seconds(30));

				

				robot2_state.robot_state=6;


				///////////////////////ROBOT STATE 6//////////////////////////////  CONTROL FASE 6  Retirarse de la base objetivo
				
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 6" << std::endl;

				RO_msg.objective = 2;  //Objeto 1
				RO_msg.distance = 35; //Distancia al objeto



				objective_robot2_publisher -> publish(RO_msg);

				


				//cliente control trayectoria
				request_ctl->time = 30;

				while (!client_trajectory_ctrl->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          		}
				
				auto result6 = client_trajectory_ctrl->async_send_request(request_ctl);
				
				result6.wait();

				result_control = result.get()->success;

				std::cout << " Result control" << std::endl;

				//Posicion del brazo
				arm_joints_position.pos_b1 = 2;
				arm_joints_position.pos_b2 = -2;
				arm_joints_position.pos_b3 = 2;
				arm_joints_position.pos_g1 = 1;

				publisher_arm_pos->publish(arm_joints_position);


				robot2_state.robot_state=7;

				///////////////////////ROBOT STATE 7//////////////////////////////  CONTROL FASE 7  Regresar a la posicion inicial
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 7" << std::endl;

				RO_msg.objective = 0;  //Objeto 1
				RO_msg.distance = 0; //Distancia al objeto



				objective_robot2_publisher -> publish(RO_msg);

				


				//cliente control trayectoria
				request_ctl->time = 100;

				while (!client_trajectory_ctrl->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service Control Trajectory.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service Control Trajectory not available, waiting again...");
          		}
				
				auto result7 = client_trajectory_ctrl->async_send_request(request_ctl);
				
				result7.wait();

				result_control = result.get()->success;

				arm_joints_position.pos_b1 = 2;
				arm_joints_position.pos_b2 = -2;
				arm_joints_position.pos_b3 = -2;
				arm_joints_position.pos_g1 = 1;

				publisher_arm_pos->publish(arm_joints_position);


				robot2_state.robot_state=8;

				///////////////////////ROBOT STATE 8//////////////////////////////  CONTROL FASE 8  Control Terminado
				state_robot2_publisher -> publish(robot2_state);
				std::cout << "FASE 8" << std::endl;


				/////////////////FIN DE CONTROL///////////////////////////////////////////////////////////
	


			}
};


class Node_Subs_Pose_R2 : public rclcpp::Node
{
	public:
		Node_Subs_Pose_R2() : Node("node_subs_pose_r2")
		{
     		
      		poses_subscriber= create_subscription<interfaces::msg::Positions>(
      			"/positions", 1, std::bind(&Node_Subs_Pose_R2::pose_subs,this,_1));
			
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

class Node_Subs_State_R1 : public rclcpp::Node
{
	public:
		Node_Subs_State_R1() : Node("node_subs_state_r1")
		{
     		
      		state_robot1_subscriber= create_subscription<interfaces::msg::RobotState>(
      			"/robot_1/state", 1, std::bind(&Node_Subs_State_R1::r1_subs,this,_1));

				std::cout<<"Listo"<<std::endl;
			
		}

	private:

		void r1_subs(const interfaces::msg::RobotState::SharedPtr msg) const
		{
			robot1_state=msg->robot_state;
			std::cout<<"R1_State_received"<<std::endl;
		}

		rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr state_robot1_subscriber;

};



int main(int argc, char * argv[])
{
  	rclcpp::init(argc, argv);
  	//rclcpp::spin(std::make_shared<Event_Driven_Control_R2>());

	
	robot2_state.robot_state=-1;
	auto node = std::make_shared<Event_Driven_Control_R2>();
    auto node_subs_pose_r2 = std::make_shared<Node_Subs_Pose_R2>();
	auto node_subs_state_r1 = std::make_shared<Node_Subs_State_R1>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(node_subs_pose_r2);
	executor.add_node(node_subs_state_r1);
    executor.spin();

 	rclcpp::shutdown();
  
}