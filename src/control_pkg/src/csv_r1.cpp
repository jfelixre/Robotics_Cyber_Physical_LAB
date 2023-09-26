#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/data_control.hpp>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

//std::ofstream myfile;

using std::placeholders::_1;
using namespace std::chrono_literals;
int state = -1;
std::ofstream myfile;


class Csv_R1 : public rclcpp::Node
{
	public:
		Csv_R1() : Node("csv_r1")
		{
           subs_data = this->create_subscription<interfaces::msg::DataControl>(
               "/robot_1/data_control", 1, std::bind(&Csv_R1::subs_data_callback,this,_1));
            

        }

    private:

        rclcpp::Subscription<interfaces::msg::DataControl>::SharedPtr subs_data;

        void subs_data_callback(const interfaces::msg::DataControl::SharedPtr data_msg)
        {
            myfile << data_msg->time;
            myfile << "_";
            myfile << state;
            myfile << "_";
            myfile << data_msg->k;
            myfile << "_";
            myfile << data_msg->x_goal;
            myfile << "_";
            myfile << data_msg->x_robot;
            myfile << "_";
            myfile << data_msg->x_error;
            myfile << "_";
            myfile << data_msg->y_goal;
            myfile << "_";
            myfile << data_msg->y_robot;
            myfile << "_";
            myfile << data_msg->y_error;
            myfile << "_";
            myfile << data_msg->ang_goal;
            myfile << "_";
            myfile << data_msg->ang_robot;
            myfile << "_";
            myfile << data_msg->ang_error;
            myfile << "_";
            myfile << data_msg->vel_x;
            myfile << "_";
            myfile << data_msg->vel_y;
            myfile << "_";
            myfile << data_msg->vel_ang;
            myfile << "\n";

        }


};

class Csv_Phase_R1 : public rclcpp::Node
{
	public:
		Csv_Phase_R1() : Node("csv_phase_r1")
		{
            subs_state = this->create_subscription<interfaces::msg::RobotState>(
               "/robot_1/state", 1, std::bind(&Csv_Phase_R1::subs_state_callback,this,_1));
          

        }

    private:

        rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subs_state;

        void subs_state_callback(const interfaces::msg::RobotState::SharedPtr state_msg)
        {
            state = state_msg->robot_state;
        }
        
};








int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);
    myfile.open ("csv/Control_Trajectory_R1.csv");

    myfile << "Time";
    myfile << "_";
    myfile << "Phase";
    myfile << "_";
    myfile << "K";
    myfile << "_";
    myfile << "X-Des";
    myfile << "_";
    myfile << "X-Rob";
    myfile << "_";
    myfile << "X-Err";
    myfile << "_";
    myfile << "Y-Des";
    myfile << "_";
    myfile << "Y-Rob";
    myfile << "_";
    myfile << "Y-Err";
    myfile << "_";
    myfile << "Ang-Des";
    myfile << "_";
    myfile << "Ang-Rob";
    myfile << "_";
    myfile << "Ang-Err";
    myfile << "_";
    myfile << "X-vel";
    myfile << "_";
    myfile << "Y-vel";
    myfile << "_";
    myfile << "Ang-vel";
    myfile << "\n";
	
	auto node = std::make_shared<Csv_R1>();
    auto csv_phase_r1 = std::make_shared<Csv_Phase_R1>();


    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(csv_phase_r1);
    executor.spin();

 	rclcpp::shutdown();
    myfile.close();
  
}