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
#include <interfaces/msg/robot_objective.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include "tf2_ros/buffer.h"
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/data_error.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm> 

using std::placeholders::_1;
using namespace std::chrono_literals;

// --- VARIABLES GLOBALES ---
int robot_id = 0;
int object_id = 0;
float angle_objective = 0;
geometry_msgs::msg::Point point_objective;
geometry_msgs::msg::Point robot_position;
geometry_msgs::msg::Point gripper_position;
float angle_robot = 0;
int robot_state=0;

// Configuración de Tiempo
double tf = 10.0; 
double ts = 0.1;  
int N = 100;      

// Trayectorias (Posición y Velocidad Deseada)
std::vector<double> hxd, hyd;     // Posición deseada
std::vector<double> vxd_ff, vyd_ff; // Velocidad Feedforward (Precalculada)

// Datos para Plotting/Debug
std::vector<double> hxe, hye, hwe; // Errores

int k = 0;  
bool control_active = false;

// Objetos de mensaje
interfaces::msg::DataError data_error;
interfaces::msg::DataError data_error_total;

// --- NODO 1: Configuración ---
class PID_Control_Config : public rclcpp::Node
{
    public:
        PID_Control_Config() : Node("pid_control_config_node")
        {
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();

            std::stringstream ss; ss << "/robot_0" << robot_id << "/trajectory_control";
            subs_trajectory_control = this->create_subscription<interfaces::msg::TrajectoryControl>(
                ss.str(), 1, std::bind(&PID_Control_Config::trajectory_control_caller, this, _1));
        }
    private:
        rclcpp::Subscription<interfaces::msg::TrajectoryControl>::SharedPtr subs_trajectory_control;
        void trajectory_control_caller(const interfaces::msg::TrajectoryControl::SharedPtr msg) { tf = msg->time; }
};

// --- NODO 2: Generador de Trayectoria (Feedforward Calculator) ---
class PID_Path_Subs : public rclcpp::Node
{
    public:
        PID_Path_Subs() : Node("pid_path_subs_node")
        {
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();

            std::stringstream ss; ss << "/robot_0" << robot_id << "/path";
            subs_path = this->create_subscription<geometry_msgs::msg::Polygon>(
               ss.str(), 1, std::bind(&PID_Path_Subs::subs_path_callback,this,_1));
        }

    private:
        rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path;

        void subs_path_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg)
        {   
            if (path_msg->points.empty()) {
                control_active = false; k = 0; return;
            }

            int n_points = path_msg->points.size();

            // 1. Calcular Distancia Total
            double total_dist = 0.0;
            for(int i=0; i<n_points-1; i++) {
                double dx = path_msg->points[i+1].x - path_msg->points[i].x;
                double dy = path_msg->points[i+1].y - path_msg->points[i].y;
                total_dist += std::sqrt(dx*dx + dy*dy);
            }

            // 2. Configurar Velocidad Deseada (1.0 m/s)
            double desired_vel = 1.0; 
            tf = total_dist / desired_vel;
            if (tf < 1.0) tf = 1.0;

            N = std::round((tf + ts) / ts);

            // Redimensionar vectores
            hxd.assign(N, 0.0); hyd.assign(N, 0.0);
            vxd_ff.assign(N, 0.0); vyd_ff.assign(N, 0.0); // Velocidades Feedforward
            
            // Resize vectores de error para seguridad
            hxe.assign(N+100, 0.0); hye.assign(N+100, 0.0); hwe.assign(N+100, 0.0);

            // 3. Interpolación Lineal (Generar hxd, hyd)
            if (n_points == 1) {
                std::fill(hxd.begin(), hxd.end(), path_msg->points[0].x);
                std::fill(hyd.begin(), hyd.end(), path_msg->points[0].y);
            } 
            else {
                int current_idx = 0;
                int segments = n_points - 1;
                
                for (int i = 0; i < segments; i++) {
                    double p1_x = path_msg->points[i].x;
                    double p1_y = path_msg->points[i].y;
                    double p2_x = path_msg->points[i+1].x;
                    double p2_y = path_msg->points[i+1].y;

                    double seg_len = std::sqrt(pow(p2_x - p1_x, 2) + pow(p2_y - p1_y, 2));
                    int steps = std::round((seg_len / total_dist) * N);
                    if (steps < 1) steps = 1;

                    double dx = (p2_x - p1_x) / steps;
                    double dy = (p2_y - p1_y) / steps;

                    for (int j = 0; j < steps; j++) {
                        if (current_idx >= N) break;
                        hxd[current_idx] = p1_x + (dx * j);
                        hyd[current_idx] = p1_y + (dy * j);
                        current_idx++;
                    }
                }
                while (current_idx < N) { // Rellenar final
                    hxd[current_idx] = path_msg->points[n_points-1].x;
                    hyd[current_idx] = path_msg->points[n_points-1].y;
                    current_idx++;
                }
            }

            // 4. Calcular Velocidades Feedforward (Derivada discreta)
            for(int i=0; i<N-1; i++){
                vxd_ff[i] = (hxd[i+1] - hxd[i]) / ts;
                vyd_ff[i] = (hyd[i+1] - hyd[i]) / ts;
            }
            vxd_ff[N-1] = 0.0; vyd_ff[N-1] = 0.0;

            RCLCPP_INFO(this->get_logger(), "PID Path Plan: Dist=%.2fm, TF=%.2fs, Vel=%.1fm/s", total_dist, tf, desired_vel);
            
            k = 0;
            control_active = true;
        }
};

// --- NODO 3: Objetivos ---
class PID_Subs_Objective : public rclcpp::Node
{
    public:
        PID_Subs_Objective() : Node("pid_subs_objective_node")
        {   
            std::stringstream ss; ss << "/robot_0" << robot_id << "/objective";
            subs_objective = this->create_subscription<interfaces::msg::RobotObjective>(
                ss.str(), 1, std::bind(&PID_Subs_Objective::subs_obj_callback,this,_1));
        }
    private:
        rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;
        void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
            object_id = obj_msg->obj_id;
            angle_objective = obj_msg->angle;
            point_objective = obj_msg->point;
            robot_state = obj_msg->robot_state;
        }
};

// --- NODO 4: Controlador PID + Feedforward (Omnidireccional) ---
class PID_Controller_Loop : public rclcpp::Node
{
    public:
        PID_Controller_Loop() : Node("pid_controller_loop_node")
        {
            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

            std::stringstream ss; ss << "/robot_0" << robot_id;
            publisher_vel = this->create_publisher<interfaces::msg::PlatformVel>(ss.str() + "/set_platform_vel", 1);
            publisher_control_finish = this->create_publisher<interfaces::msg::ControlFinish>(ss.str() + "/control_finish", 1);
            publisher_data_error = this->create_publisher<interfaces::msg::DataError>(ss.str() + "/data_error", 1);
            publisher_data_error_total = this->create_publisher<interfaces::msg::DataError>(ss.str() + "/total_data_error", 1);
            publisher_plot_data = this->create_publisher<std_msgs::msg::Float32MultiArray>(ss.str() + "/plot_data", 1);

            timer_ = this->create_wall_timer(100ms, std::bind(&PID_Controller_Loop::timer_callback, this));
        }

    private:
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<interfaces::msg::PlatformVel>::SharedPtr publisher_vel;
        std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
        std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        rclcpp::Publisher<interfaces::msg::ControlFinish>::SharedPtr publisher_control_finish;
        rclcpp::Publisher<interfaces::msg::DataError>::SharedPtr publisher_data_error;
        rclcpp::Publisher<interfaces::msg::DataError>::SharedPtr publisher_data_error_total;
        rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_plot_data;

    void timer_callback()
    { 
       if (control_active == true) {

            // 1. Obtener Posición (Gripper)
            try {
                std::string gr_frame = "robot_0" + std::to_string(robot_id) + "/gr_ref_link";
                auto tf_grip = tf_buffer_->lookupTransform("marker_id_00", gr_frame, tf2::TimePointZero);
                gripper_position.x = tf_grip.transform.translation.x; 
                gripper_position.y = tf_grip.transform.translation.y;

                std::string base_frame = "robot_0" + std::to_string(robot_id) + "/base_link";
                auto tf_base = tf_buffer_->lookupTransform("marker_id_00", base_frame, tf2::TimePointZero);
                tf2::Quaternion q(tf_base.transform.rotation.x, tf_base.transform.rotation.y, tf_base.transform.rotation.z, tf_base.transform.rotation.w);
                tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r, p, y);
                angle_robot = y;
            } catch (tf2::TransformException &ex) { return; }

            // 2. Calcular Errores Globales
            int idx = std::min(k, N-1);
            
            double ex = hxd[idx] - gripper_position.x;
            double ey = hyd[idx] - gripper_position.y;
            double et = angle_objective - angle_robot;
            
            // Normalizar error angular (-PI a PI)
            while (et > M_PI) et -= 2*M_PI;
            while (et < -M_PI) et += 2*M_PI;

            // Guardar errores para debug
            hxe[k] = ex; hye[k] = ey; hwe[k] = et;

            // 3. LEY DE CONTROL: Feedforward + Proporcional (Global)
            // Kp es la "agresividad" para corregir error.
            double Kp = 2.5; 
            double Kw = 1.5;

            // Velocidad Global Deseada = (Velocidad del Path) + (Corrección de Error)
            double vx_global = vxd_ff[idx] + Kp * ex;
            double vy_global = vyd_ff[idx] + Kp * ey;
            double w_global  = Kw * et;

            // 
            // 4. Transformación Global -> Local (Robot Frame)
            // Esto es lo que permite que el robot se mueva de lado
            double cos_th = cos(angle_robot);
            double sin_th = sin(angle_robot);

            // Matriz de Rotación 2D (R^T)
            double vx_robot =  vx_global * cos_th + vy_global * sin_th;
            double vy_robot = -vx_global * sin_th + vy_global * cos_th;
            double w_robot  = w_global;

            // 5. Saturación y Envío
            double max_lin = 1.2; // m/s
            double max_ang = 2.0; // rad/s

            vx_robot = std::max(-max_lin, std::min(vx_robot, max_lin));
            vy_robot = std::max(-max_lin, std::min(vy_robot, max_lin));
            w_robot  = std::max(-max_ang, std::min(w_robot, max_ang));

            interfaces::msg::PlatformVel msg_vel;
            
            // Ganancia de Potencia (Ajustar si el robot es físico o simulación pesada)
            double power_gain = 120.0; 
            
            msg_vel.x_vel = vx_robot * power_gain;
            msg_vel.y_vel = vy_robot * power_gain; 
            msg_vel.ang_vel = w_robot * power_gain; 

            publisher_vel->publish(msg_vel);

            // 6. Publicar Datos Debug
            data_error.x_error = ex; data_error.y_error = ey; data_error.ang_error = et; data_error.k = k;
            publisher_data_error->publish(data_error);
            
            data_error_total.x_error = hxd[N-1] - gripper_position.x;
            data_error_total.y_error = hyd[N-1] - gripper_position.y;
            data_error_total.ang_error = et;
            publisher_data_error_total->publish(data_error_total);

            std_msgs::msg::Float32MultiArray msg_plot;
            msg_plot.data = {(float)robot_state, (float)ex, (float)ey, (float)et, 
                             (float)vx_robot, (float)vy_robot, (float)w_robot,
                             (float)hxd[N-1], (float)hyd[N-1], (float)angle_objective};
            publisher_plot_data->publish(msg_plot);

            // 7. Actualizar y Chequear Fin
            k++;
            
            // Criterio de parada: Llegar al final del tiempo (N) o Error muy bajo
            bool time_done = (k >= N);
            bool error_done = (k > 10 && abs(data_error_total.x_error) < 0.04 && abs(data_error_total.y_error) < 0.04);

            if (time_done || error_done) {
                RCLCPP_INFO(this->get_logger(), "Meta Alcanzada (PID). Deteniendo.");
                msg_vel.x_vel=0; msg_vel.y_vel=0; msg_vel.ang_vel=0;
                publisher_vel->publish(msg_vel);

                interfaces::msg::ControlFinish finish; finish.finish_confirm = true;
                publisher_control_finish->publish(finish);
                
                control_active = false;
                k = 0;
            }

       } else {
           interfaces::msg::PlatformVel stop;
           stop.x_vel=0; stop.y_vel=0; stop.ang_vel=0;
           publisher_vel->publish(stop);
       }
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    // Nombres de clases actualizados para evitar conflicto
    auto node_config = std::make_shared<PID_Control_Config>();
    auto node_path = std::make_shared<PID_Path_Subs>();
    auto node_obj = std::make_shared<PID_Subs_Objective>();
    auto node_loop = std::make_shared<PID_Controller_Loop>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node_config);
    executor.add_node(node_path);
    executor.add_node(node_obj);
    executor.add_node(node_loop);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}