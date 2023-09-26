#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/platform_vel.hpp"
#include <interfaces/msg/motor_platform_vels.hpp>

#include <memory>
#include <cinttypes>


class Robot_2_Platform_Vel_Server : public rclcpp::Node
{
	public:
		Robot_2_Platform_Vel_Server() : Node("robot_2_platform_vel_server")
		{
           subs_platform_vel_r2 = this->create_subscription<interfaces::msg::PlatformVel>(
                "robot_2/set_platform_vel", 1, std::bind(&Robot_2_Platform_Vel_Server::send_vel, this, std::placeholders::_1));

			publisher_M = this->create_publisher<interfaces::msg::MotorPlatformVels>("/robot_2/platform_vel_motors",1);
            

        }

    private:

        rclcpp::Subscription<interfaces::msg::PlatformVel>::SharedPtr subs_platform_vel_r2;
		rclcpp::Publisher<interfaces::msg::MotorPlatformVels>::SharedPtr publisher_M;

        void send_vel(const interfaces::msg::PlatformVel::SharedPtr request){

			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;

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


			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Robot 2 Platform:  M1=%f M2=%f M3=%f M4=%f",
				vel_M.vel_m1, vel_M.vel_m2, vel_M.vel_m3, vel_M.vel_m4);

		}



};




int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<Robot_2_Platform_Vel_Server>();

  rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

	executor.spin();

 	rclcpp::shutdown();

}