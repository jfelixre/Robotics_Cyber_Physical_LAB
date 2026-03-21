#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/platform_vel.hpp"
#include <std_msgs/msg/float64.h>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/bool.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/leader_robot.hpp>

#include <memory>
#include <cinttypes>
#include <thread>
#include <csignal>

int robot_id=0;
float copy_vel_x, copy_vel_y, copy_vel_ang;
bool copy_vel_flag = false;
int robot_state = 0;
int leader_robot_id = 0;

class Robot_Platform_Vel_Node : public rclcpp::Node
{
	public:
		Robot_Platform_Vel_Node() : Node("robot_platform_vel_node")
		{	
			
			this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

			std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/set_platform_vel";
            std::string topic_name = ss_topic_name.str();


           subs_platform_vel_r1 = this->create_subscription<interfaces::msg::PlatformVel>(
                topic_name, 1, std::bind(&Robot_Platform_Vel_Node::send_vel, this, std::placeholders::_1));


			std::stringstream ss_topic_name_m1;
            ss_topic_name_m1 << "/robot_0" << robot_id << "/m1_joint/controller";
            std::string topic_name_m1 = ss_topic_name_m1.str();

			publisher_M1 = this->create_publisher<std_msgs::msg::Float64>(topic_name_m1,1);

			std::stringstream ss_topic_name_m2;
			ss_topic_name_m2 << "/robot_0" << robot_id << "/m2_joint/controller";
			std::string topic_name_m2 = ss_topic_name_m2.str();

			publisher_M2 = this->create_publisher<std_msgs::msg::Float64>(topic_name_m2,1);

			std::stringstream ss_topic_name_m3;
			ss_topic_name_m3 << "/robot_0" << robot_id << "/m3_joint/controller";
			std::string topic_name_m3 = ss_topic_name_m3.str();

			publisher_M3 = this->create_publisher<std_msgs::msg::Float64>(topic_name_m3,1);

			std::stringstream ss_topic_name_m4;
			ss_topic_name_m4 << "/robot_0" << robot_id << "/m4_joint/controller";
			std::string topic_name_m4 = ss_topic_name_m4.str();

			publisher_M4 = this->create_publisher<std_msgs::msg::Float64>(topic_name_m4,1);
            

        }

        // Destructor para asegurar parada segura
        ~Robot_Platform_Vel_Node() {
            stop_robot();
        }

        void stop_robot() {
            std_msgs::msg::Float64 zero;
            zero.data = 0.0;
            if (publisher_M1 && publisher_M2 && publisher_M3 && publisher_M4) {
                publisher_M1->publish(zero);
                publisher_M2->publish(zero);
                publisher_M3->publish(zero);
                publisher_M4->publish(zero);
                RCLCPP_INFO(this->get_logger(), "Robot detenido: velocidades en 0");
                // Dar tiempo para que se envíen los mensajes
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

        // Hacer públicos los publishers para acceso desde shutdown_handler
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M1;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M2;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M3;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M4;

    private:

        rclcpp::Subscription<interfaces::msg::PlatformVel>::SharedPtr subs_platform_vel_r1;

        void send_vel(const interfaces::msg::PlatformVel::SharedPtr request){

			float max_vel = 12.0; // Much higher from 8.5 for very fast robot movement

			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;

			if(copy_vel_flag){
				//velang= 0;
				//vely=0;
				if (robot_id == leader_robot_id){
					velang = velang * 0;
					vely = vely * 2;
					velx = velx * 2;
					
				}
				else{
					velx = copy_vel_x * -2;
					vely = copy_vel_y * -2;
					velang = copy_vel_ang * 0;
				}
				
			}


			// if (robot_state == 4){
			// 	//velang = velang * 2;
			// 	//vely=0;
			// 	//velx = velx*10;
			// }

			float La = 128.9375;
			float Lb = 109.379;
			float R = 100/2;

			std_msgs::msg::Float64 vel_m1,vel_m2,vel_m3,vel_m4;

			vel_m1.data=(velx - vely - ((La+Lb)*velang))/R ;
			vel_m2.data=(velx + vely + ((La+Lb)*velang))/R *-1;
			vel_m3.data=(velx + vely - ((La+Lb)*velang))/R ;
			vel_m4.data=(velx - vely + ((La+Lb)*velang))/R *-1;

			if (vel_m1.data > max_vel) { vel_m1.data = max_vel;}
			if (vel_m1.data < -max_vel) { vel_m1.data = -max_vel;}
			if (vel_m2.data > max_vel) { vel_m2.data = max_vel;}
			if (vel_m2.data < -max_vel) { vel_m2.data = -max_vel;}
			if (vel_m3.data > max_vel) { vel_m3.data = max_vel;}
			if (vel_m3.data < -max_vel) { vel_m3.data = -max_vel;}
			if (vel_m4.data > max_vel) { vel_m4.data = max_vel;}
			if (vel_m4.data < -max_vel) { vel_m4.data = -max_vel;}

			// if(robot_state == 4){
			// 	if (leader_robot_id == robot_id){
				
			// 		// vel_m1.data= 0;
			// 		// vel_m2.data= 0;
			// 		// //RCLCPP_INFO(this->get_logger(), "M1 and M2 = 0  ********************************************************************");		
			// 	}
			// 	else{
			// 		// vel_m1.data= vel_m4.data;
			// 		// vel_m2.data= vel_m3.data;
			// 		// vel_m3.data= 0;
			// 		// vel_m4.data= 0;
			// 		vel_m3.data= vel_m3.data + 0.5;
			// 		vel_m4.data= vel_m4.data + 0.5;
			// 		//RCLCPP_INFO(this->get_logger(), "M3 and M4 = 0  ********************************************************************");
			// 	}		
			// }


			publisher_M1->publish(vel_m1);
			publisher_M2->publish(vel_m2);
			publisher_M3->publish(vel_m3);
			publisher_M4->publish(vel_m4);



			//RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Robot 1 Platform:  M1=%f M2=%f M3=%f M4=%f",
			//	vel_m1.data, vel_m2.data, vel_m3.data, vel_m4.data);

		}



};


class Node_Copy : public rclcpp::Node
{
	public:
		Node_Copy() : Node("node_copy")
		{	
			//delay
			rclcpp::sleep_for(std::chrono::seconds(5));
			RCLCPP_INFO(this->get_logger(), "Node Copy started");

			


			std::string copy_vel_topic;
			if (robot_id == 1){
				copy_vel_topic = "/robot_02/set_platform_vel";
			}
			else{
				copy_vel_topic = "/robot_01/set_platform_vel";
			}

			subs_platform_vel_copy = this->create_subscription<interfaces::msg::PlatformVel>(
				copy_vel_topic, 1, std::bind(&Node_Copy::copy_vel_callback, this, std::placeholders::_1));

			
			



		}

		private:
		rclcpp::Subscription<interfaces::msg::PlatformVel>::SharedPtr subs_platform_vel_copy;
		

		

		void copy_vel_callback(const interfaces::msg::PlatformVel::SharedPtr msg)
		{
			if (copy_vel_flag) {
				copy_vel_x = msg->x_vel;
				copy_vel_y = msg->y_vel;
				copy_vel_ang = msg->ang_vel;

				RCLCPP_INFO(this->get_logger(), "Velocities copied");
			}
		}



};

class Node_Extra_Subs : public rclcpp::Node
{
	public:
	Node_Extra_Subs() : Node("node_extra_subs")
		{	
			rclcpp::sleep_for(std::chrono::seconds(5));

			std::string subs_copy_control_topic = "/robot_0" + std::to_string(robot_id) + "/copy_control";
			subs_copy = this->create_subscription<std_msgs::msg::Bool>(
				subs_copy_control_topic, 1, std::bind(&Node_Extra_Subs::copy_callback, this, std::placeholders::_1));


				std::string subs_robot_state_topic = "robot_0" + std::to_string(robot_id) + "/robot_state";

				subs_robot_state = this->create_subscription<interfaces::msg::RobotState>(
					subs_robot_state_topic, 1, std::bind(&Node_Extra_Subs::state_callback, this, std::placeholders::_1));
				
	
				std::string subs_leader_robot_id = "/leader_robot_id";
				subs_leader_robot = this->create_subscription<interfaces::msg::LeaderRobot>(
					subs_leader_robot_id, 1, std::bind(&Node_Extra_Subs::leader_callback, this, std::placeholders::_1));

		}

		private:
			rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subs_copy;
			rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subs_robot_state;
			rclcpp::Subscription<interfaces::msg::LeaderRobot>::SharedPtr subs_leader_robot;

			void copy_callback(const std_msgs::msg::Bool::SharedPtr msg)
			{
				copy_vel_flag = msg->data;
				RCLCPP_INFO(this->get_logger(), "Copy Vel Flag: %s", copy_vel_flag ? "true" : "false");
			}

			void state_callback(const interfaces::msg::RobotState::SharedPtr msg){
				robot_state = msg ->robot_state;
				RCLCPP_INFO(this->get_logger(), "Get Robot State: %d", robot_state);
			}
	
			void leader_callback(const interfaces::msg::LeaderRobot::SharedPtr msg){
				leader_robot_id = msg->leader_robot_id;
				RCLCPP_INFO(this->get_logger(), "Get Leader Robot: %d", leader_robot_id);
			}
	
};

std::shared_ptr<Robot_Platform_Vel_Node> global_node_ptr;

void shutdown_handler(int signum) {
    RCLCPP_INFO(rclcpp::get_logger("shutdown"), "Señal recibida (%d), deteniendo robot...", signum);
    if (global_node_ptr) {
        global_node_ptr->stop_robot();
        // Esperar para asegurar publicación
        rclcpp::sleep_for(std::chrono::milliseconds(200));
    }
    rclcpp::shutdown();
    std::exit(signum);
}

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);

	global_node_ptr = std::make_shared<Robot_Platform_Vel_Node>();
	auto node_copy = std::make_shared<Node_Copy>();
	auto node_extra_subs = std::make_shared<Node_Extra_Subs>();

	// Registrar handler de señal para SIGINT y SIGTERM
	std::signal(SIGINT, shutdown_handler);
	std::signal(SIGTERM, shutdown_handler);

	rclcpp::executors::MultiThreadedExecutor executor;
	executor.add_node(global_node_ptr);
	executor.add_node(node_copy);
	executor.add_node(node_extra_subs);

	executor.spin();

	rclcpp::shutdown();
	return 0;
}