#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/srv/trajectory_control.hpp>
#include <interfaces/srv/platform_vel.hpp>
#include <Eigen/Dense>
#include <interfaces/msg/robot_objective.hpp>

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

double ANG_Ob = -1;
double ANG_Tg = -1;

int n_objective = -1;
float distance_objective = 0;
 double phid = 0;

//Variables de tiempo y control
    int tf = 180; // tiempo de simulacion
    double ts = 0.1; // tiempo de muestreo
    //const int N = std::round((tf + ts) / ts); // cantidad de muestras 
    int N = std::round((tf+ts)/ts); //1801;     //N = 1201 para 60 s y 601 para 30s
   // std::vector<std::vector<double>> he(2, std::vector<double>(1));
   // std::vector<std::vector<double>> J(2, std::vector<double>(2));
   // std::vector<std::vector<double>> K(2, std::vector<double>(2));
   // std::vector<std::vector<double>> qpRef(2, std::vector<double>(1));


    // TRAYECTORIA DESEADA
    //double hxd[N] = {};
    //double hyd[N] = {};

    std::vector<double> hxd(N);
    std::vector<double> hyd(N);
//    double phid[N] = {};

   

    //double hxdp[N] = {};
    //double hydp[N] = {};

    std::vector<double>  hxdp(N);
    std::vector<double>  hydp(N);

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

    rclcpp::Client<interfaces::srv::PlatformVel>::SharedPtr client_vel;


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
            tf = request->time;

            N = std::round((tf+ts)/ts);
            hxd.resize(N,0);
            hyd.resize(N,0);
            hxdp.resize(N,0);
            hydp.resize(N,0);
            uxRef.resize(N,1.0);
            uyRef.resize(N,1.0);
            wRef.resize(N,1.0);
            hxe.resize(N,0);
            hye.resize(N,0);
            hwe.resize(N,0);
            gain.resize(N,0);

            hxa = 0;
            hya = 0;
            phia = 0;
            k=0;

            std::cout << "service start" << std::endl;
            control_active = true;
            rclcpp::sleep_for(std::chrono::seconds(tf));
            response -> success = control_active;
            std::cout << "service finish" << std::endl;




        }




};

class Node_Subs_Path_R1 : public rclcpp::Node
{
	public:
		Node_Subs_Path_R1() : Node("node_subs_path_r1")
		{

            subs_path = this->create_subscription<geometry_msgs::msg::Polygon>(
               "/robot_1/path", 1, std::bind(&Node_Subs_Path_R1::subs_path_callback,this,_1));

        }

    private:

    rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path;

     void subs_path_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg)
        {   

           // std::cout << "subs path" << k << std::endl;

            //memset(hxd, 0, sizeof(hxd));
            //memset(hyd, 0, sizeof(hyd));
            hxd.resize(N,0);
            hyd.resize(N,0);
            
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


           
            /*
            for (int i=0; i<=N; i++){
                 std::cout << hxd[i] << ";" << hyd[i] << std::endl;
            }
            */

            RCLCPP_INFO(this->get_logger(), "path received");
            

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

            


        }

};

class Node_Subs_Positions_R1 : public rclcpp::Node
{
	public:
		Node_Subs_Positions_R1() : Node("node_subs_positions_r1")
		{

            subs_pos = this->create_subscription<interfaces::msg::Positions>(
               "/positions", 1, std::bind(&Node_Subs_Positions_R1::subs_pos_callback,this,_1));

            subs_objective = this->create_subscription<interfaces::msg::RobotObjective>(
                "robot_1/objective", 1, std::bind(&Node_Subs_Positions_R1::subs_obj_callback,this,_1));

        }

    private:

    rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subs_pos;
    rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;


    void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
            n_objective = obj_msg->objective;
            distance_objective = obj_msg->distance;
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Update objective");

            switch (n_objective)
            {
            case -1:
                phid= 0;
                break;

            case 0:
                phid= 0;
                break;

            case 1:
                //phid= ANG_Ob + M_PI_2;
                phid= M_PI_2;
                break;

            case 2:
                phid= ANG_Tg + M_PI_2;
                break;               
            
            default:
                break;
            }



    }
    
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
            X_Robot = X_Robot + (0.125 * cos(ANG_Robot)) - 0.02;
            Y_Robot = Y_Robot + (0.125 * sin(ANG_Robot));
            
            tf2::Quaternion Object_quat(pos_msg->pos_object1.orientation.x, pos_msg->pos_object1.orientation.y, pos_msg->pos_object1.orientation.z, pos_msg->pos_object1.orientation.w);
            tf2::Matrix3x3 Object_m(Object_quat);
            double Object_orientation_x, Object_orientation_y, Object_orientation_z;
            Object_m.getRPY(Object_orientation_x, Object_orientation_y, Object_orientation_z);
			ANG_Ob= Object_orientation_z;

            tf2::Quaternion Target_quat(pos_msg->pos_target.orientation.x, pos_msg->pos_target.orientation.y, pos_msg->pos_target.orientation.z, pos_msg->pos_target.orientation.w);
            tf2::Matrix3x3 Target_m(Target_quat);
            double Target_orientation_x, Target_orientation_y, Target_orientation_z;
            Target_m.getRPY(Target_orientation_x, Target_orientation_y, Target_orientation_z);
			ANG_Tg= Target_orientation_z;

          //  std::cout << ANG_Robot << std::endl;
        }

};


class Node_Control_Timer_R1 : public rclcpp::Node
{
	public:
		Node_Control_Timer_R1() : Node("node_control_timer_r1")
		{

           client_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
           timer_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
           client_vel = this->create_client<interfaces::srv::PlatformVel>("robot_1/set_platform_vel", rmw_qos_profile_services_default, client_cb_group_);



            timer_ = this->create_wall_timer(
                 100ms, std::bind(&Node_Control_Timer_R1::timer_callback, this), timer_cb_group_);
          

        }

    private:

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Client<interfaces::srv::PlatformVel>::SharedPtr client_vel;

    rclcpp::CallbackGroup::SharedPtr client_cb_group_;
    rclcpp::CallbackGroup::SharedPtr timer_cb_group_;

    void timer_callback()   //////CONTROL/////////
    { 

        //std::cout << " Callback de tiempo" << std::endl;

        
       if (control_active == true){

        //std::cout << "inicio " << std::endl;



           
            double vxd = (hxd[k] - hxa)/ts;    
            double vyd = (hyd[k] - hya)/ts;
            double vwd = (phid - phia)/ts;

            // Parametros Robot


            //Errores!


                 hxe[k] = hxd[k] - X_Robot;
                 hye[k] = hyd[k] - Y_Robot;

                 double ErrAng = phid - ANG_Robot;

                hwe[k] = ErrAng;

                double grados = (ANG_Robot * 180)/M_PI;
                double cos_val = cos(ANG_Robot);
                double sin_val = sin(ANG_Robot);
            
            
           // std::cout << " Grados= " << grados << " cos = " << cos_val << " sin =" << sin_val << std::endl;
            

            std::cout << "x deseada = " << hxd[k]  << "  X_Rob = " << X_Robot << std::endl;
            std::cout << "y deseada = " << hyd[k]  << "  Y_Rob = " << Y_Robot << std::endl;
            std::cout << "phi deseada = " << phid  << "  ANG_Rob = " << ANG_Robot << std::endl;
            std::cout << "error x = " <<  hxe[k] << "  error y = " <<hye[k] << " error w = " << hwe[k] << " Grados= " << grados << std::endl;
            

            //Ganancias
            double Kx = 350;
            double Ky = 350;
            double Kw = 10;


            Eigen::MatrixXd he(3,1);
            he << vxd + Kx * tanh(hxe[k]), 
                  vyd + Ky * tanh(hye[k]),
                  vwd + Kw * tanh(hwe[k]);

            //std::cout << "he =" << he << std::endl;



            
            int sign_cos = 1;
            int sign_sin = 1;

            double cos_ang_robot = cos(ANG_Robot) * sign_cos;
            double sin_ang_robot = sin(ANG_Robot) * sign_sin;




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


            auto request = std::make_shared<interfaces::srv::PlatformVel::Request>();


            request->x_vel = uxRef[k] * 35;
            request->y_vel = uyRef[k] * 35;
            request->ang_vel = wRef[k];
            
            std::cout << " Xvel = " << uxRef[k] << "  Yvel = " << uyRef[k] <<  "  ANG_vel = " << wRef[k] << std::endl;

            auto result = client_vel->async_send_request(request);


            std::future_status status = result.wait_for(3s);  // timeout to guarantee a graceful finish
            if (status == std::future_status::ready) {
               // RCLCPP_INFO(this->get_logger(), "Received response");
            }

            std::cout << "k = " << k << std::endl;
            k++;

            hxa = hxd[k];
            hya = hyd[k];
            phia = phid;

           // std::cout << "Despues de asignar valorees anteriores" << std::endl;

            if (k==N){

                request->x_vel = 0.0;
                request->y_vel = 0.0;
                request->ang_vel = 0.0;

                auto result = client_vel->async_send_request(request);
                std::cout << "k=N" << k << N << std::endl;


            }

            if (k>N){
                k=0;
                request->x_vel = 0.0;
                request->y_vel = 0.0;
                request->ang_vel = 0.0;

                auto result = client_vel->async_send_request(request);
                control_active = false;
            }

          //  std::cout << "Despues de los ifss" << std::endl;

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
    k=0;



   // hx[0] = 0.9; // # Posicion inicial en el eje x en metros [m]
    //hy[0] = -0.7;  //# Posicion inicial en el eje y en metros [m]
    //phi[0] = 180*(M_PI/180); //# Orientacion inicial en radianes [rad]
	
	auto node = std::make_shared<Control_Trajectory_R1>();
    auto node_subs_path_r1 = std::make_shared<Node_Subs_Path_R1>();
	auto node_subs_positions_r1 = std::make_shared<Node_Subs_Positions_R1>();
    auto node_control_timer_r1 = std::make_shared<Node_Control_Timer_R1>();
   // auto node_client_vel = std::make_shared<Node_Client_Vel>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(node_subs_path_r1);
	executor.add_node(node_subs_positions_r1);
    executor.add_node(node_control_timer_r1);
    //executor.add_node(node_client_vel);
    executor.spin();

 	rclcpp::shutdown();
  
}