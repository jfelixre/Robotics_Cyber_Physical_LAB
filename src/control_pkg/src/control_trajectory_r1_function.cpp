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
#include <math.h>

using std::placeholders::_1;
using namespace std::chrono_literals;
geometry_msgs::msg::Polygon path;

//Variables de posición
double X_Robot=-1;
double Y_Robot=-1;
double ANG_Robot=-1;

//Variables de tiempo y control
    const double tf = 180; // tiempo de simulacion
    const double ts = 0.05; // tiempo de muestreo
    //const int N = std::round((tf + ts) / ts); // cantidad de muestras 
    const int N = 3601;     //N = 1201 para 60 s y 601 para 30s
   // std::vector<std::vector<double>> he(2, std::vector<double>(1));
   // std::vector<std::vector<double>> J(2, std::vector<double>(2));
   // std::vector<std::vector<double>> K(2, std::vector<double>(2));
   // std::vector<std::vector<double>> qpRef(2, std::vector<double>(1));


    // TRAYECTORIA DESEADA
    double hx[N] = {};
    double hy[N] = {};
    double phi[N] = {};

   

    double hxdp[N] = {};
    double hydp[N] = {};

    // ERRORES
    //double hxe[N] = {};
    //double hye[N] = {};

    // Posición deseada
   // const double hxd = 1;
   // const double hyd = 1;
   // const double hwd = 0;

    // Velocidades de referecias - El algoritmo debe calcular las velocidades necesarias. 
    std::vector<double> uxRef(N, 1.0);
    std::vector<double> uyRef(N, 1.0);
    std::vector<double> wRef(N, 1.0);

    // Errores 
    std::vector<double> hxe(N, 0);
    std::vector<double> hye(N, 0);
    std::vector<double> hwe(N, 0);

    //Variable adicional
    std::vector<double> gain(N, 0);


   

   // double t[N];

    int k = 0;  //Step control
    double hxa = 0;
    double hya = 0;
    double phia = 0;


    //double uRef[N] = {};  // Velocidad lineal en metros/segundos [m/s]
    //double wRef[N] = {};  // Velocidad angular en radianes/segundos [rad/s]
    

        std::vector<double> t(N); // time vector


    bool control_active = false;

class Control_Trajectory_R1 : public rclcpp::Node
{
	public:
		Control_Trajectory_R1() : Node("control_trajectory_r1")
		{
           service_trajectory_control = this->create_service<interfaces::srv::TrajectoryControl>(
                "trajectory_control_server_R1", std::bind(&Control_Trajectory_R1::trajectory_control_caller, this,
                std::placeholders::_1, std::placeholders::_2));

            

        }

    private:

        rclcpp::Service<interfaces::srv::TrajectoryControl>::SharedPtr service_trajectory_control;
        
        
        void trajectory_control_caller(const std::shared_ptr<interfaces::srv::TrajectoryControl::Request> request,
          std::shared_ptr<interfaces::srv::TrajectoryControl::Response>      response)
		{
            hxa = 0;
            hya = 0;
            phia = 0;

            std::cout << "service start" << std::endl;
            control_active = true;
            rclcpp::sleep_for(std::chrono::seconds(60));
            response -> success = control_active;
            std::cout << "service finish" << std::endl;




        }




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

           // std::cout << "subs path" << k << std::endl;
/*
            memset(hxd, 0, sizeof(hxd));
            memset(hyd, 0, sizeof(hyd));


            
            int n_points= path_msg->points.size();
           
            
            //colocar primer punto del mensaje en el espacio actual (k) de la trayectoria deseada
            hxd[k] = path_msg->points[0].x;
            hyd[k] = path_msg->points[0].y;
            

            //colocar el ultimo punto del mensaje en el ultimo espacio de la trayectoria deseada
            hxd[N] = path_msg->points[n_points-1].x;
            hyd[N] = path_msg->points[n_points-1].y;
           

            //calcular espacios entre puntos
            

            int step_size = floor((N - k)/n_points);
            

            //Colocar los puntos del mensaje en el espacio que corresponden
            int max_size_i;

            for (int i = 1; i<(n_points); i++){
                hxd[k+(step_size*i)] = path_msg->points[i].x;
                hyd[k+(step_size*i)] = path_msg->points[i].y;
                max_size_i = k + (step_size*(i));
            }

            //for (int i=0; i<=N; i++){
            //    std::cout << hxd[i] << ";" << hyd[i] << std::endl;
            //}

            for (int i = 0; i<(n_points-1); i++){
                hxd[k+(step_size*i)] = path_msg->points[i].x;

                double dx = (hxd[k+(step_size*(i+1))] - hxd[k+(step_size*i)])/(step_size+1);
                double dy = (hyd[k+(step_size*(i+1))] - hyd[k+(step_size*i)])/(step_size+1);

                for (int j=1; j<step_size; j++){
                    hxd[(k+(step_size*i))+j] = hxd[(k+(step_size*i))+j-1]+dx;
                    hyd[(k+(step_size*i))+j] = hyd[(k+(step_size*i))+j-1]+dy;
                }
            }
                

            //std::cout<<max_size_i<<std::endl;

                double dx_l = (hxd[N] - hxd[max_size_i])/(N-max_size_i);
                double dy_l = (hyd[N] - hyd[max_size_i])/(N-max_size_i);

                for (int i=(max_size_i+1); i<N; i++){
                    hxd[i] = hxd[i-1]+dx_l;
                    hyd[i] = hyd[i-1]+dy_l;
                }


           

            //for (int i=0; i<=N; i++){
            //     std::cout << hxd[i] << ";" << hyd[i] << std::endl;
            //}

            //std::cout << "fin" << std::endl;
            

            //Derivadas

            hxdp[0]= 0;
            hydp[0]= 0;

            hxdp[N]= 0;
            hydp[N]= 0;

            for (int i=0; i<=N; i++){
                hxdp[i]= (hxd[i+1] - hxd[i]) /ts;
                hydp[i]= (hyd[i+1] - hyd[i]) /ts;
            }

            hxdp[N]= 0;
            hydp[N]= 0;

            //for (int i=0; i<=N; i++){
            //     std::cout << hxdp[i] << "  " << hydp[i] << std::endl;
            //}

            
*/

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

          //   std::cout << "subs pos" << std::endl;
            X_Robot= pos_msg->pos_robot1.position.x;
			Y_Robot= pos_msg->pos_robot1.position.y;
			tf2::Quaternion Robot_quat(pos_msg->pos_robot1.orientation.x, pos_msg->pos_robot1.orientation.y, pos_msg->pos_robot1.orientation.z, pos_msg->pos_robot1.orientation.w);
            tf2::Matrix3x3 Robot_m(Robot_quat);
            double Robot_orientation_x, Robot_orientation_y, Robot_orientation_z;
            Robot_m.getRPY(Robot_orientation_x, Robot_orientation_y, Robot_orientation_z);
			ANG_Robot= Robot_orientation_z;

          //  std::cout << ANG_Robot << std::endl;
        }

};


class Node_Control_Timer : public rclcpp::Node
{
	public:
		Node_Control_Timer() : Node("node_control_timer")
		{
            client_vel = this->create_client<interfaces::srv::RobotVel>("robot1_vel");



            timer_ = this->create_wall_timer(
                 50ms, std::bind(&Node_Control_Timer::timer_callback, this));
          

        }

    private:

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Client<interfaces::srv::RobotVel>::SharedPtr client_vel;

    void timer_callback()   //////CONTROL/////////
    { 

        
       if (control_active == true){

        


        //################### POSICION DESEADA ####################
        double hxd = 0.01*t[k]; //0*t
        double hyd = cos(t[k]/16)*0.5; //0*t
        double phid = -2.8;

        //################### VELOCIDAD DESEADA ####################



        double vxd = (hxd - hxa)/ts;    // double vxd = 1 ;  //#0 * t
        double vyd = (hyd - hya)/ts;   // double vyd = 1 ; // #0 * t
        double vwd = (phid - phia)/ts;   // double vwd = 0 ;

        hx[0]=0*t[k];
        hy[0]=0*t[k];
        phi[0]= 0*t[k];


            // Parametros Robot
            const double a = 0.15; //meters

            //Errores!
               // hxe[k] = hxd - hx[k];
               // hye[k] = hyd - hy[k];

                //double ErrAng = phid - phi[k];

                 hxe[k] = hxd - X_Robot;
                 hye[k] = hyd - Y_Robot;

                 double ErrAng = phid - ANG_Robot;
                /*
                    if (ErrAng >= (1.1 * M_PI)){
                        ErrAng -= 2 * M_PI;
                    }

                    if (ErrAng <= (-1.1 * M_PI)){
                        ErrAng += 2 * M_PI;
                    }
                */

                hwe[k] = ErrAng;

                double grados = (ANG_Robot * 180)/M_PI;
                double cos_val = cos(ANG_Robot);
                double sin_val = sin(ANG_Robot);
            
            
            std::cout << " Grados= " << grados << " cos = " << cos_val << " sin =" << sin_val;
            


        //    std::cout << "error x = " <<  hxe[k] << "  error y = " <<hye[k] << " error w = " << hwe[k] << " Grados= " << grados;
            

            //Ganancias
            double Kx = 300;
            double Ky = 300;
            double Kw = 10;


            Eigen::MatrixXd he(3,1);
            he << vxd + Kx * tanh(hxe[k]), 
                  vyd + Ky * tanh(hye[k]),
                  vwd + Kw * tanh(hwe[k]);



            
            int sign_cos = 1;
            int sign_sin = 1;
/*

           if ((ANG_Robot >= 0 && ANG_Robot <= M_PI_2) ){
                sign_sin = -1;
                std::cout << "cos negativo" << std::endl;
           }

           if ((ANG_Robot > M_PI_2 && ANG_Robot <= M_PI) ){
                sign_sin = -1;
                sign_cos = -1;
           }

         **/  
/*
            if (ANG_Robot < 0){
                sign_sin = -1;
                std::cout << "sin negativo" << std::endl;
           }

*/
            double cos_ang_robot = cos(ANG_Robot) * sign_cos;
            double sin_ang_robot = sin(ANG_Robot) * sign_sin;


/*
            if ((ANG_Robot <= -M_PI_2 && ANG_Robot >= -M_PI) ){
               cos_ang_robot = sin(ANG_Robot) * -1;
               sin_ang_robot = cos(ANG_Robot) * -1;
           }

           */

            // Matriz Jacobiana
            Eigen::MatrixXd J(3,3);
            J << cos_ang_robot, -sin_ang_robot, 0,
                 sin_ang_robot,  cos_ang_robot, 0,
                 0,                 0,            1;

        

            // Define variables
            Eigen::MatrixXd qpRef;
            

            // Ley de control
            qpRef = J.inverse() * he; 

            // Aplicar control
            uxRef[k] = qpRef(0, 0);
            uyRef[k] = qpRef(1, 0);
            wRef[k] = qpRef(2, 0);

            //#Velocidades Lineales Actuales
            double xp = uxRef[k] * cos(phi[k]) - uyRef[k] * sin(phi[k]);
            double yp = uxRef[k] * sin(phi[k]) + uyRef[k] * cos(phi[k]);

            //#Posicion actual
            hx[k+1]=hx[k]+xp*ts;
            hy[k+1]=hy[k]+yp*ts;
            phi[k+1]=phi[k]+wRef[k]*ts;

           // std::cout << " vel x = " << uxRef[k] << "  vel y = " << uyRef[k] <<  " vel w = " << wRef[k] << "  t = " << t[k] <<  "   hx[k] = " << hx[k] <<    std::endl;
            std::cout << " vel x = " << uxRef[k] << "  vel y = " << uyRef[k] <<  " vel w = " << wRef[k];


            float z = sqrt((uxRef[k]*uxRef[k])+(uyRef[k]*uyRef[k]));
			float Beta = atan2(uyRef[k],uxRef[k]);
			float Gamma = ANG_Robot - (M_PI/2);
			float Delta = Beta-Gamma;

			float velx = z * cos(Delta) * -1;
			float vely = z * sin(Delta);

            std::cout << " vel x = " << velx << "  vel y = " << vely <<  std::endl;

            auto request = std::make_shared<interfaces::srv::RobotVel::Request>();


            request->x_vel = uxRef[k] * 10;
            request->y_vel = uyRef[k] * 10;
            request->ang_vel = wRef[k];
            request->b1_vel = 0;
			request->b2_vel = 0;
			request->b3_vel = 0;
			request->g1_vel = 0;


            auto result = client_vel->async_send_request(request);


            k++;

            hxa = hxd;
            hya = hyd;
            phia = phid;

            if (k>N){
                k=0;
                control_active = false;
            }

       }
        
    }
    

 
};

int main(int argc, char * argv[])
{

  	rclcpp::init(argc, argv);
  	//rclcpp::spin(std::make_shared<Control_Trajectory_R1>());

    for (int i = 0; i < N; i++) {
        t[i] = i * ts;
    }



   // hx[0] = 0.9; // # Posicion inicial en el eje x en metros [m]
    //hy[0] = -0.7;  //# Posicion inicial en el eje y en metros [m]
    //phi[0] = 180*(M_PI/180); //# Orientacion inicial en radianes [rad]
	
	auto node = std::make_shared<Control_Trajectory_R1>();
    auto node_subs_path = std::make_shared<Node_Subs_Path>();
	auto node_subs_positions = std::make_shared<Node_Subs_Positions>();
    auto node_control_timer = std::make_shared<Node_Control_Timer>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(node_subs_path);
	executor.add_node(node_subs_positions);
    executor.add_node(node_control_timer);
    executor.spin();

 	rclcpp::shutdown();
  
}