#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"
#include <interfaces/msg/motor_platform_vels.hpp>

#include <memory>
#include <cinttypes>

// rclcpp::Node::SharedPtr Robot1PlatformVelServer_node = nullptr;
// rclcpp::Publisher<interfaces::msg::MotorPlatformVels>::SharedPtr publisher_M = nullptr;

class Robot_1_Platform_Vel_Server : public rclcpp::Node
{
	public:
		Robot_1_Platform_Vel_Server() : Node("robot_1_platform_vel_server")
		{
           service_platform_vel_r1 = this->create_service<interfaces::srv::PlatformVel>(
                "robot_1/set_platform_vel", std::bind(&Robot_1_Platform_Vel_Server::send_vel, this,
                std::placeholders::_1, std::placeholders::_2));

			publisher_M = this->create_publisher<interfaces::msg::MotorPlatformVels>("/robot_1/platform_vel_motors",1);
            

        }

    private:

        rclcpp::Service<interfaces::srv::PlatformVel>::SharedPtr service_platform_vel_r1;
		rclcpp::Publisher<interfaces::msg::MotorPlatformVels>::SharedPtr publisher_M;

		void send_vel(const std::shared_ptr<interfaces::srv::PlatformVel::Request> request,
          std::shared_ptr<interfaces::srv::PlatformVel::Response>      response)
		{
			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;
				
			int sent=0;

			float La = 128.9375;
			float Lb = 109.379;
			float R = 100/2;

			interfaces::msg::MotorPlatformVels vel_M;


			vel_M.vel_m1=(velx - vely - ((La+Lb)*velang))/R ;
			vel_M.vel_m2=(velx + vely + ((La+Lb)*velang))/R ;
			vel_M.vel_m3=(velx + vely - ((La+Lb)*velang))/R ;
			vel_M.vel_m4=(velx - vely + ((La+Lb)*velang))/R ;

			if (vel_M.vel_m1 > 20) { vel_M.vel_m1 = 20;}
			if (vel_M.vel_m1 < -20) { vel_M.vel_m1 = -20;}
			if (vel_M.vel_m2 > 20) { vel_M.vel_m2 = 20;}
			if (vel_M.vel_m2 < -20) { vel_M.vel_m2 = -20;}
			if (vel_M.vel_m3 > 20) { vel_M.vel_m3 = 20;}
			if (vel_M.vel_m3 < -20) { vel_M.vel_m3 = -20;}
			if (vel_M.vel_m4 > 20) { vel_M.vel_m4 = 20;}
			if (vel_M.vel_m4 < -20) { vel_M.vel_m4 = -20;}


			publisher_M->publish(vel_M);


			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Robot 1 Platform:  M1=%f M2=%f M3=%f M4=%f",
				vel_M.vel_m1, vel_M.vel_m2, vel_M.vel_m3, vel_M.vel_m4);

			sent=sent+1;
			response-> sent;
		}



};

/* void send_vel(const std::shared_ptr<interfaces::srv::PlatformVel::Request> request,
          std::shared_ptr<interfaces::srv::PlatformVel::Response>      response)
		{
		  
			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;
				
			int sent=0;

			float La = 128.9375;
			float Lb = 109.379;
			float R = 100/2;

			interfaces::msg::MotorPlatformVels vel_M;


			vel_M.vel_m1=(velx - vely - ((La+Lb)*velang))/R ;
			vel_M.vel_m2=(velx + vely + ((La+Lb)*velang))/R ;
			vel_M.vel_m3=(velx + vely - ((La+Lb)*velang))/R ;
			vel_M.vel_m4=(velx - vely + ((La+Lb)*velang))/R ;

			if (vel_M.vel_m1 > 20) { vel_M.vel_m1 = 20;}
			if (vel_M.vel_m1 < -20) { vel_M.vel_m1 = -20;}
			if (vel_M.vel_m2 > 20) { vel_M.vel_m2 = 20;}
			if (vel_M.vel_m2 < -20) { vel_M.vel_m2 = -20;}
			if (vel_M.vel_m3 > 20) { vel_M.vel_m3 = 20;}
			if (vel_M.vel_m3 < -20) { vel_M.vel_m3 = -20;}
			if (vel_M.vel_m4 > 20) { vel_M.vel_m4 = 20;}
			if (vel_M.vel_m4 < -20) { vel_M.vel_m4 = -20;}


			publisher_M->publish(vel_M);


			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Robot 1 Platform:  M1=%f M2=%f M3=%f M4=%f",
				vel_M.vel_m1, vel_M.vel_m2, vel_M.vel_m3, vel_M.vel_m4);

			sent=sent+1;
			response-> sent;
		} */


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<Robot_1_Platform_Vel_Server>();

  rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

	executor.spin();

 	rclcpp::shutdown();

 
//   Robot1PlatformVelServer_node = rclcpp::Node::make_shared("robot_1_platform_vel_server");


//   rclcpp::Service<interfaces::srv::PlatformVel>::SharedPtr service =
//     Robot1PlatformVelServer_node->create_service<interfaces::srv::PlatformVel>("robot_1/set_platform_vel", &send_vel);

//   publisher_M = Robot1PlatformVelServer_node->create_publisher<interfaces::msg::MotorPlatformVels>("/robot_1/platform_vel_motors",1);

//   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to send velocities to Robot 1 Platform.");

//   rclcpp::spin(Robot1PlatformVelServer_node);
//   rclcpp::shutdown();
}