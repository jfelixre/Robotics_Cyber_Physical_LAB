#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

using std::placeholders::_1;
using namespace std::chrono_literals;
geometry_msgs::msg::Polygon path;

//Variables de posición
float X_Robot=-1;
float Y_Robot=-1;
float ANG_Robot=-1;

//Variables de tiempo y control
    const double tf = 60; // tiempo de simulacion
    const double ts = 0.05; // tiempo de muestreo
    const int N = std::round((tf + ts) / ts); // cantidad de muestras
    std::vector<std::vector<double>> he(2, std::vector<double>(1));
    std::vector<std::vector<double>> J(2, std::vector<double>(2));
    std::vector<std::vector<double>> K(2, std::vector<double>(2));
    std::vector<std::vector<double>> qpRef(2, std::vector<double>(1));

    //double x1p[N] = {};
    //double y1p[N] = {};

    //double t[N+1];

    //for (int i = 0; i <= N; ++i) {
    //    t[i] = i * ts;
    //}



class Control_Trajectory_R1 : public rclcpp::Node
{
	public:
		Control_Trajectory_R1() : Node("control_trajectory_r1")
		{
           // service_ = this->create_service<interfaces::srv::AStarService>(
             //   "a_star_server", std::bind(&AStarServer::a_star_caller, this,
               // std::placeholders::_1, std::placeholders::_2));

        }

    private:

};

class Node_Subs_Path : public rclcpp::Node
{
	public:
		Node_Subs_Path() : Node("node_subs_path")
		{

            subs_path = this->create_subscription<geometry_msgs::msg::Polygon>(
               "/robot_1/path", 1, std::bind(&Node_Subs_Path::subs_path_callback,this,_1));

        }

    private:

    rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path;

     void subs_path_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg)
        {
        //    path = path_msg->points;
        }

};

class Node_Subs_Positions : public rclcpp::Node
{
	public:
		Node_Subs_Positions() : Node("node_subs_positions")
		{

            subs_pos = this->create_subscription<interfaces::msg::Positions>(
               "/positions", 1, std::bind(&Node_Subs_Positions::subs_pos_callback,this,_1));

        }

    private:

    rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subs_pos;

     void subs_pos_callback(const interfaces::msg::Positions::SharedPtr pos_msg)
        {
            X_Robot= pos_msg->pos_robot1.position.x;
			Y_Robot= pos_msg->pos_robot1.position.y;
			tf2::Quaternion Robot_quat(pos_msg->pos_robot1.orientation.x, pos_msg->pos_robot1.orientation.y, pos_msg->pos_robot1.orientation.z, pos_msg->pos_robot1.orientation.w);
            tf2::Matrix3x3 Robot_m(Robot_quat);
            double Robot_orientation_x, Robot_orientation_y, Robot_orientation_z;
            Robot_m.getRPY(Robot_orientation_x, Robot_orientation_y, Robot_orientation_z);
			ANG_Robot= Robot_orientation_z;
        }

};


int main(int argc, char * argv[])
{

  	rclcpp::init(argc, argv);
  	//rclcpp::spin(std::make_shared<Control_Trajectory_R1>());
	
	auto node = std::make_shared<Control_Trajectory_R1>();
    auto node_subs_path = std::make_shared<Node_Subs_Path>();
	auto node_subs_positions = std::make_shared<Node_Subs_Positions>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(node_subs_path);
	executor.add_node(node_subs_positions);
    executor.spin();

 	rclcpp::shutdown();
  
}