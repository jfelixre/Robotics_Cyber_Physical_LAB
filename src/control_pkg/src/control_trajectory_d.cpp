#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/msg/trajectory_control.hpp>
#include <interfaces/msg/platform_vel.hpp>
#include <interfaces/msg/data_control.hpp>
#include <Eigen/Dense>
#include <interfaces/msg/robot_objective.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include "tf2_ros/buffer.h"
#include <interfaces/msg/control_finish.hpp>

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
geometry_msgs::msg::Polygon path;


double ANG_Ob = -1;
double ANG_Tg = -1;

int n_objective = -1;
float distance_objective = 0;
 double phid = 0;



 int robot_id = 0;
int object_id = 0;
float angle_objective = 0;
geometry_msgs::msg::Point point_objective;
geometry_msgs::msg::Point robot_position;
geometry_msgs::msg::Point gripper_position;
float angle_robot = 0;
geometry_msgs::msg::Point object_position;
float angle_object = 0;
float type_object = 0;  //1: Single object, 2: Double object




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

    //rclcpp::Client<interfaces::srv::PlatformVel>::SharedPtr client_vel;


class Control_Trajectory_Node : public rclcpp::Node
{
	public:
		Control_Trajectory_Node() : Node("control_trajectory_node")
		{

            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            //RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/trajectory_control";
            std::string topic_name = ss_topic_name.str();


            subs_trajectory_control = this->create_subscription<interfaces::msg::TrajectoryControl>(
                topic_name, 1, std::bind(&Control_Trajectory_Node::trajectory_control_caller, this,
                std::placeholders::_1));

        }

    private:

        rclcpp::Subscription<interfaces::msg::TrajectoryControl>::SharedPtr subs_trajectory_control;
        
        void trajectory_control_caller(const interfaces::msg::TrajectoryControl::SharedPtr trajectory_msg)
		{   
           // //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "subs trajectory");
            tf = trajectory_msg->time;

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

            std::cout << "trajectory control start" << std::endl;
            control_active = true;
            // rclcpp::sleep_for(std::chrono::seconds(tf));
            // std::cout << "service finish" << std::endl;




        }




};

class Node_Subs_Path : public rclcpp::Node
{
	public:
		Node_Subs_Path() : Node("node_subs_path")
		{

            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            //RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/path";
            std::string topic_name = ss_topic_name.str();
        

            subs_path = this->create_subscription<geometry_msgs::msg::Polygon>(
               topic_name, 1, std::bind(&Node_Subs_Path::subs_path_callback,this,_1));

        }

    private:

    rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path;

     void subs_path_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg)
        {   

           // std::cout << "subs path" << k << std::endl;
           //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "subs_path_callback");

            //memset(hxd, 0, sizeof(hxd));
            //memset(hyd, 0, sizeof(hyd));
            hxd.resize(N,0);
            hyd.resize(N,0);
            
            int n_points= path_msg->points.size();

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "n points %d", n_points);
           
            
            //colocar primer punto del mensaje en el espacio actual (k) de la trayectoria deseada
            hxd[k] = path_msg->points[0].x;
            hyd[k] = path_msg->points[0].y;

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "first point");
            
            if (n_points>1){
                //colocar el ultimo punto del mensaje en el ultimo espacio de la trayectoria deseada
                hxd[N] = path_msg->points[n_points-1].x;
                hyd[N] = path_msg->points[n_points-1].y;

                //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "last point");
            

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

                ////RCLCPP_INFO(this->get_logger(), "path received");
                

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
            


        }

};

class Node_Subs_Positions : public rclcpp::Node
{
	public:
		Node_Subs_Positions() : Node("node_subs_positions")
		{   
            

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/objective";
            std::string topic_name = ss_topic_name.str();


            subs_objective = this->create_subscription<interfaces::msg::RobotObjective>(
                topic_name, 1, std::bind(&Node_Subs_Positions::subs_obj_callback,this,_1));

        }

    private:

    rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;


    void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
        ////RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "subs_obj_callback");

        object_id = obj_msg->obj_id;
        angle_objective = obj_msg->angle;
        point_objective = obj_msg->point;

       //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Trajectory control Start");
        control_active = true;
        k=0;

    }
    
};


class Node_Control_Timer : public rclcpp::Node
{
	public:
		Node_Control_Timer() : Node("node_control_timer")
		{

            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

           //client_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
           //timer_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
           //client_vel = this->create_client<interfaces::srv::PlatformVel>("robot_1/set_platform_vel", rmw_qos_profile_services_default, client_cb_group_);
           std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/set_platform_vel";
            std::string topic_name = ss_topic_name.str();


            publisher_vel = this->create_publisher<interfaces::msg::PlatformVel>(topic_name,1);

            std::stringstream ss_topic_name_finish;
            ss_topic_name_finish << "/robot_0" << robot_id << "/control_finish";
            std::string topic_name_finish = ss_topic_name_finish.str();

            publisher_control_finish = this->create_publisher<interfaces::msg::ControlFinish>(topic_name_finish,1);

            timer_ = this->create_wall_timer(
                 100ms, std::bind(&Node_Control_Timer::timer_callback, this));

            

 
          

        }

    private:

    rclcpp::TimerBase::SharedPtr timer_;
    //rclcpp::Client<interfaces::srv::PlatformVel>::SharedPtr client_vel;

    //rclcpp::CallbackGroup::SharedPtr client_cb_group_;
    rclcpp::CallbackGroup::SharedPtr timer_cb_group_;
    //rclcpp::Publisher<interfaces::msg::DataControl>::SharedPtr data_control_robot1_publisher;
    rclcpp::Publisher<interfaces::msg::PlatformVel>::SharedPtr publisher_vel;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Publisher<interfaces::msg::ControlFinish>::SharedPtr publisher_control_finish;


    void timer_callback()   //////CONTROL/////////
    { 

       // std::cout << " Callback de tiempo" << std::endl;
        //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "timer_callback");

        
       if (control_active == true){

        //Save robot and object position
        for (int marker=1; marker<30; marker++){
                std::stringstream ss_marker;
                if (marker<10){
                    ss_marker << "marker_id_0" << marker;
                }
                else{
                ss_marker << "marker_id_" << marker;
                }

                std::string marker_name = ss_marker.str();

                ////RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Object ID %d", object_id);

                
                    if(robot_id==marker){                   
                        
                        std::stringstream ss_gripper;
                        ss_gripper << "robot_0" << robot_id << "/gr_ref_link";
                        std::string gripper_name = ss_gripper.str();


                        try{
                            geometry_msgs::msg::TransformStamped transformStamped_gripper = tf_buffer_->lookupTransform("marker_id_00", gripper_name, tf2::TimePointZero);
                            gripper_position.x = transformStamped_gripper.transform.translation.x;
                            gripper_position.y = transformStamped_gripper.transform.translation.y;
                        }
                        catch (tf2::TransformException &ex){
                            ////RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }


                        std::stringstream ss_base;
                        ss_base << "robot_0" << robot_id << "/base_link";
                        std::string base_name = ss_base.str();

                        try{
                            geometry_msgs::msg::TransformStamped transformStamped = tf_buffer_->lookupTransform("marker_id_00", base_name, tf2::TimePointZero);
                            robot_position.x = transformStamped.transform.translation.x;
                            robot_position.y = transformStamped.transform.translation.y;
                            tf2::Quaternion quat(transformStamped.transform.rotation.x, transformStamped.transform.rotation.y, transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
                            tf2::Matrix3x3 m(quat);
                            double roll, pitch, yaw;
                            m.getRPY(roll,pitch,yaw);
                            angle_robot = yaw;
                        }
                        catch (tf2::TransformException &ex){
                            ////RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }

                    }

                    else if(object_id==marker){
                        ////RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Object detected ID %d", object_id);
                        try{
                            geometry_msgs::msg::TransformStamped transformStamped = tf_buffer_->lookupTransform("marker_id_00", marker_name, tf2::TimePointZero);
                            object_position.x = transformStamped.transform.translation.x;
                            object_position.y = transformStamped.transform.translation.y;
                            tf2::Quaternion quat(transformStamped.transform.rotation.x, transformStamped.transform.rotation.y, transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
                            tf2::Matrix3x3 m(quat);
                            double roll, pitch, yaw;
                            m.getRPY(roll,pitch,yaw);
                            angle_object = yaw;
                            if(marker>10 && marker<20){
                                type_object = 1;
                            }
                            else if(marker>20){
                                type_object = 2;
                            }
                        }
                        catch (tf2::TransformException &ex){
                            ////RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }
                    }

        }

        //std::cout << "inicio " << std::endl;
        //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "inicio");

            phid= angle_objective;  //Desired angle

           
            double vxd = (hxd[k] - hxa)/ts;    
            double vyd = (hyd[k] - hya)/ts;
            double vwd = (phid - phia)/ts;

            // Parametros Robot


            //Errores!


                 hxe[k] = hxd[k] - gripper_position.x;
                 hye[k] = hyd[k] - gripper_position.y;

                 double ErrAng = phid - angle_robot;

                hwe[k] = ErrAng;

            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "hxe = %f", hxe[k]);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "hye = %f", hye[k]);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "hwe = %f", hwe[k]);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "k = %d", k);

            int min_error = 0.00005;   //Error to skip control to next step

            if(abs(hxe[k])<min_error && abs(hye[k])<min_error && abs(hwe[k])<min_error){
                    k=N;
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "End of control FOR ERROR <0.01");
                    
            }   

                

                //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Errores");

                //double grados = (ANG_Robot * 180)/M_PI;
                //double cos_val = cos(ANG_Robot);
                //double sin_val = sin(ANG_Robot);
            
            
           // std::cout << " Grados= " << grados << " cos = " << cos_val << " sin =" << sin_val << std::endl;
            

            //std::cout << "x deseada = " << hxd[k]  << "  X_Rob = " << gripper_position.x << std::endl;
            //std::cout << "y deseada = " << hyd[k]  << "  Y_Rob = " << gripper_position.y << std::endl;
            //std::cout << "phi deseada = " << phid  << "  ANG_Rob = " << ANG_Robot << std::endl;
            //std::cout << "error x = " <<  hxe[k] << "  error y = " <<hye[k] << " error w = " << hwe[k] << " Grados= " << grados << std::endl;
            

            //Ganancias
            double Kx = 50;
            double Ky = 50;
            double Kw = 10;


            Eigen::MatrixXd he(3,1);
            he << vxd + Kx * tanh(hxe[k]), 
                  vyd + Ky * tanh(hye[k]),
                  vwd + Kw * tanh(hwe[k]);

            //std::cout << "he =" << he << std::endl;

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Matrix he");


            
            int sign_cos = 1;
            int sign_sin = 1;

            double cos_ang_robot = cos(angle_robot) * sign_cos;
            double sin_ang_robot = sin(angle_robot) * sign_sin;




            // Matriz Jacobiana
            Eigen::MatrixXd J(3,3);
            J << cos_ang_robot, -sin_ang_robot, 0,
                 sin_ang_robot,  cos_ang_robot, 0,
                 0,                 0,            1;


            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Jacobiana");

            // Define variables
            Eigen::MatrixXd qpRef;
            

            // Ley de control
            qpRef = J.inverse() * he; 

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "inverse Jacobiana");



            // Aplicar control
            uxRef[k] = qpRef(0, 0);
            uyRef[k] = qpRef(1, 0);
            wRef[k] = qpRef(2, 0);
            
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "apply control");

            //auto request = std::make_shared<interfaces::srv::PlatformVel::Request>();
            interfaces::msg::PlatformVel msg_platform_vel;
            
            uxRef[k] = uxRef[k] * 35;
            uyRef[k] = uyRef[k] * 35;

            msg_platform_vel.x_vel = uxRef[k];
            msg_platform_vel.y_vel = uyRef[k];
            msg_platform_vel.ang_vel = wRef[k];

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "create variable for control ");
            
            //std::cout << " Xvel = " << uxRef[k] << "  Yvel = " << uyRef[k] <<  "  ANG_vel = " << wRef[k] << std::endl;

            //auto result = client_vel->async_send_request(request);


            // std::future_status status = result.wait_for(100ms);  // timeout to guarantee a graceful finish
            // if (status == std::future_status::ready) {
            //    // //RCLCPP_INFO(this->get_logger(), "Received response");
            // }

            publisher_vel->publish(msg_platform_vel);

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "publicar control");

            // std::cout << "Antes de publicar" << std::endl;
            //std::cout << "k = " << k << std::endl;
            //save_data(k, hxd[k], gripper_position.x, hxe[k], hyd[k], gripper_position.y, hye[k], phid, ANG_Robot, hwe[k], uxRef[k], uyRef[k], wRef[k]);
            // //SEND DATA 
            
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            
            std::cout << time << "_" << k << "_" << hxd[k] << "_" << gripper_position.x << "_" << hxe[k] << "_" << hyd[k] << "_" << gripper_position.y << "_" << hye[k] << "_" << 
                phid << "_" << angle_robot << "_" << hwe[k] << "_" << uxRef[k] << "_" << uyRef[k] << "_" << wRef[k] << std::endl;
            //////////////////////////
          //  std::cout << "Despues de publicar" << std::endl;

            k++;

            

            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "k = %d", k);
            //float left_time = tf - (k*ts);
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Time left = %f", left_time);

            hxa = hxd[k];
            hya = hyd[k];
            phia = phid;
            
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Guardar anteriores");
        //   std::cout << "Despues de asignar valorees anteriores" << std::endl;

          

            if (k==N){
                
            //    std::cout << "k=N" << k << N << std::endl;
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "k==N");

                msg_platform_vel.x_vel = 0.0;
                msg_platform_vel.y_vel = 0.0;
                msg_platform_vel.ang_vel = 0.0;

                // auto result = client_vel->async_send_request(request);
                // std::future_status status = result.wait_for(100ms);  // timeout to guarantee a graceful finish
                // if (status == std::future_status::ready) {
                // // //RCLCPP_INFO(this->get_logger(), "Received response");
                // }
                 publisher_vel->publish(msg_platform_vel);
              //  std::cout << "k=N" << k << N << std::endl;
                interfaces::msg::ControlFinish msg_control_finish;
                msg_control_finish.finish_confirm = true;
                publisher_control_finish->publish(msg_control_finish);
                control_active = false;
                k=0;


                //save_data(k, hxd[k], gripper_position.x, hxe[k], hyd[k], gripper_position.y, hye[k], phid, ANG_Robot, hwe[k], uxRef[k], uyRef[k], wRef[k]);


            }

            else if (k>N){
               // k=0;
              // std::cout << "k>N" << k << N << std::endl;
              //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "K>N");
                msg_platform_vel.x_vel = 0.0;
                msg_platform_vel.y_vel = 0.0;
                msg_platform_vel.ang_vel = 0.0;

                // auto result = client_vel->async_send_request(request);
                // std::future_status status = result.wait_for(100ms);  // timeout to guarantee a graceful finish
                // if (status == std::future_status::ready) {
                // //RCLCPP_INFO(this->get_logger(), "Received response");
                // }
                publisher_vel->publish(msg_platform_vel);
                interfaces::msg::ControlFinish msg_control_finish;
                msg_control_finish.finish_confirm = true;
                publisher_control_finish->publish(msg_control_finish);

                control_active = false;
                k=0;

               //save_data(k, hxd[k], gripper_position.x, hxe[k], hyd[k], gripper_position.y, hye[k], phid, ANG_Robot, hwe[k], uxRef[k], uyRef[k], wRef[k]);
            }

            //std::cout << "Despues de los ifss" << std::endl;
 
          //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Fin de control");

       }

       else{
           //std::cout << "Control no activo" << std::endl;
           interfaces::msg::PlatformVel msg_platform_vel;
            msg_platform_vel.x_vel = 0.0;
            msg_platform_vel.y_vel = 0.0;
            msg_platform_vel.ang_vel = 0.0;
            publisher_vel->publish(msg_platform_vel);
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Control no activo");
            k=0;
       }
        //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Fin de todo");
    }

     /* void save_data(int k, double x_des, double x_rob, double x_err, double y_des, double y_rob, double y_err, double ang_des, double ang_rob, double ang_err, double x_vel, double y_vel, double ang_vel)
		{
    		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

            std::cout << "Guardar datos" << std
			myfile << time;
            myfile << "_";
            myfile << k;
            myfile << "_";
            myfile << x_des;
            myfile << "_";
            myfile << x_rob;
            myfile << "_";
            myfile << x_err;
            myfile << "_";
            myfile << y_des;
            myfile << "_";
            myfile << y_rob;
            myfile << "_";
            myfile << y_err;
            myfile << "_";
            myfile << ang_des;
            myfile << "_";
            myfile << ang_rob;
            myfile << "_";
            myfile << ang_err;
            myfile << "_";
            myfile << x_vel;
            myfile << "_";
            myfile << y_vel;
            myfile << "_";
            myfile << ang_vel;
            myfile << "\n";


		} */
    

 
};



int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);
    /* myfile.open ("csv/Control_Trajectory_Node.csv");

    myfile << "Time";
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
    */
   
  	//rclcpp::spin(std::make_shared<Control_Trajectory_Node>());

    for (int i = 0; i < N; i++) {
        t[i] = i * ts;
    }
    k=0;



   // hx[0] = 0.9; // # Posicion inicial en el eje x en metros [m]
    //hy[0] = -0.7;  //# Posicion inicial en el eje y en metros [m]
    //phi[0] = 180*(M_PI/180); //# Orientacion inicial en radianes [rad]
	
	auto node = std::make_shared<Control_Trajectory_Node>();
    auto node_subs_path = std::make_shared<Node_Subs_Path>();
	auto node_subs_positions = std::make_shared<Node_Subs_Positions>();
    auto node_control_timer = std::make_shared<Node_Control_Timer>();
   // auto node_client_vel = std::make_shared<Node_Client_Vel>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.add_node(node_subs_path);
	executor.add_node(node_subs_positions);
    executor.add_node(node_control_timer);
    //executor.add_node(node_client_vel);
    executor.spin();

 	rclcpp::shutdown();
   // myfile.close();
  
}