#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/platform_vel.hpp"
#include <std_msgs/msg/float64.h>
#include <std_msgs/msg/float64.hpp>


#include <memory>
#include <cinttypes>

int robot_id=0;

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

    private:

        rclcpp::Subscription<interfaces::msg::PlatformVel>::SharedPtr subs_platform_vel_r1;
		rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M1;
		rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M2;
		rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M3;
		rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M4;

        void send_vel(const interfaces::msg::PlatformVel::SharedPtr request){

			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;

			float La = 128.9375;
			float Lb = 109.379;
			float R = 100/2;

			std_msgs::msg::Float64 vel_m1,vel_m2,vel_m3,vel_m4;

			vel_m1.data=(velx - vely - ((La+Lb)*velang))/R ;
			vel_m2.data=(velx + vely + ((La+Lb)*velang))/R *-1;
			vel_m3.data=(velx + vely - ((La+Lb)*velang))/R *-1;
			vel_m4.data=(velx - vely + ((La+Lb)*velang))/R ;

			if (vel_m1.data > 20) { vel_m1.data = 20;}
			if (vel_m1.data < -20) { vel_m1.data = -20;}
			if (vel_m2.data > 20) { vel_m2.data = 20;}
			if (vel_m2.data < -20) { vel_m2.data = -20;}
			if (vel_m3.data > 20) { vel_m3.data = 20;}
			if (vel_m3.data < -20) { vel_m3.data = -20;}
			if (vel_m4.data > 20) { vel_m4.data = 20;}
			if (vel_m4.data < -20) { vel_m4.data = -20;}


			publisher_M1->publish(vel_m1);
			publisher_M2->publish(vel_m2);
			publisher_M3->publish(vel_m3);
			publisher_M4->publish(vel_m4);



			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to Robot 1 Platform:  M1=%f M2=%f M3=%f M4=%f",
				vel_m1.data, vel_m2.data, vel_m3.data, vel_m4.data);

		}



};




int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<Robot_Platform_Vel_Node>();

  rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

	executor.spin();

 	rclcpp::shutdown();

}