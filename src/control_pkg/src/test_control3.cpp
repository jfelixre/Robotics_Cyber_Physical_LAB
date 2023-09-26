#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/srv/trajectory_control.hpp>
#include <interfaces/srv/robot_vel.hpp>
#include <Eigen/Dense>

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



class Control_Trajectory_R1 : public rclcpp::Node
{
	public:
		Control_Trajectory_R1() : Node("control_trajectory_r1")
		{
           service_trajectory_control = this->create_service<interfaces::srv::TrajectoryControl>(
                "trajectory_control_server_R1", std::bind(&Control_Trajectory_R1::trajectory_control_caller, this,
                std::placeholders::_1, std::placeholders::_2));

            client_vel = this->create_client<interfaces::srv::RobotVel>("robot1_vel");
            

        }

    private:

        rclcpp::Service<interfaces::srv::TrajectoryControl>::SharedPtr service_trajectory_control;

        rclcpp::Client<interfaces::srv::RobotVel>::SharedPtr client_vel;
        
        
        void trajectory_control_caller(const std::shared_ptr<interfaces::srv::TrajectoryControl::Request> request,
          std::shared_ptr<interfaces::srv::TrajectoryControl::Response>      response)
		{
            

            // Simulation Time
            const double tf = 40; //Final time
            const double ts = 0.1; //sample time
            const int N = static_cast<int>(tf / ts) + 1; //Samples

            std::vector<double> t(N); // time vector
            for (int i = 0; i < N; i++) {
                t[i] = i * ts;
            }

            // Parametros Robot
            const double a = 0.15; //meters

            // initial conditions
            std::vector<double> x1(N + 1, 0);
            std::vector<double> y1(N + 1, 0);
            std::vector<double> phi(N + 1, 0);

            x1[0] = 0;
            y1[0] = 0;
            phi[0] = 0 * (M_PI / 180.0);

            std::vector<double> hx(N + 1, 0);
            std::vector<double> hy(N + 1, 0);

            //Forward Kinematics
            hx[0] = x1[0] + a * std::cos(phi[0]);
            hy[0] = y1[0] + a * std::sin(phi[0]);

            // Posición deseada
            const double hxd = 0;
            const double hyd = 1;

            // Velocidades de referecias - El algoritmo debe calcular las velocidades necesarias. 
            std::vector<double> uRef(N, 1.0);
            std::vector<double> wRef(N, 1.0);

            // Errores 
            std::vector<double> hxe(N, 0);
            std::vector<double> hye(N, 0);

            //Variable adicional
            std::vector<double> gain(N, 0);

            // Bucle de simulación
            for (int k = 0; k < N; k++)
            {
                //Errores!
                hxe[k] = hxd - hx[k];
                hye[k] = hyd - hy[k];


                Eigen::MatrixXd he(2,1);
                he << hxe[k], 
                    hye[k];

                // Matriz Jacobiana
                Eigen::MatrixXd J(2,2);
                J << std::cos(phi[k]), -a * std::sin(phi[k]),
                    std::sin(phi[k]), a * std::cos(phi[k]);

            //    double J[2][2] = {{std::cos(phi[k]), -a * std::sin(phi[k])},
              //                  {std::sin(phi[k]), a * std::cos(phi[k])}};

                double distance = std::sqrt(std::pow(hxd - hx[k], 2) + std::pow(hyd - hy[k], 2));

                const int kmax = 2;
                const int k1 = 100;
                gain[k] = kmax / (1 + k1 * distance);
                
                
                
                //Parametros de control
                Eigen::MatrixXd K(2,2);

                K<< gain[k], 0,
                  0, gain[k];

                                // Define variables
                //Eigen::MatrixXd J, K, he, qpRef;
               // double uRef, wRef, phi, x1, y1, a, ts, x1p, y1p, hx, hy;

                Eigen::MatrixXd qpRef;
                double x1p,y1p;

                // Ley de control
                //qpRef = (J.transpose() * (J*J.transpose()).inverse() * K * he).eval();

                qpRef = J.inverse() * K * he; 

                // Aplicar control
                uRef[k] = qpRef(0, 0);
                wRef[k] = qpRef(1, 0);

                phi[k+1] = phi[k] + ts*wRef[k];

                // Modelo cinemático
                x1p = uRef[k]*cos(phi[k+1]);
                y1p = uRef[k]*sin(phi[k+1]);

                // Integración numérica
                x1[k+1] = x1[k] + ts*x1p;
                y1[k+1] = y1[k] + ts*y1p;

                // Cinemática directa
                hx[k+1] = x1[k+1] + a*cos(phi[k+1]);
                hy[k+1] = y1[k+1] + a*sin(phi[k+1]);
                    
                    
                
                
                
                
                // Modelo cinematico
                double velx = uRef[k]*cos(phi[k+1])*10000;
                double vely = uRef[k]*sin(phi[k+1])*10000;

                std::cout<< "uRef = " << uRef[k] << "  wRef =" << wRef[k] << std::endl;

                auto request = std::make_shared<interfaces::srv::RobotVel::Request>();

                request->x_vel = velx;
                request->y_vel = vely;
                request->ang_vel = wRef[k];
                request->b1_vel = 0;
                request->b2_vel = 0;
                request->b3_vel = 0;
                request->g1_vel = 0;


                auto result = client_vel->async_send_request(request);

                rclcpp::sleep_for(std::chrono::milliseconds(100));


            }
        }




};


int main(int argc, char * argv[])
{

  	rclcpp::init(argc, argv);

	
	auto node = std::make_shared<Control_Trajectory_R1>();


    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

    executor.spin();

 	rclcpp::shutdown();
  
}