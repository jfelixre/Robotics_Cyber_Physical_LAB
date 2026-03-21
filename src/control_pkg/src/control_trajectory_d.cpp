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
#include <tf2_ros/transform_broadcaster.h>
#include "tf2_ros/buffer.h"
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/data_error.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

// --- LIBRERÍA PARA MARCADORES EN RVIZ ---
#include <visualization_msgs/msg/marker_array.hpp> 

#include <memory>
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm> 
#include <iomanip> 

using std::placeholders::_1;
using namespace std::chrono_literals;

// --- VARIABLES GLOBALES ---
int robot_id = 0;
int object_id = 0;
float angle_objective = 0;
geometry_msgs::msg::Point point_objective;
geometry_msgs::msg::Point gripper_position;
float angle_robot = 0;
int robot_state=0;

// Variables de tiempo y control
double tf = 10.0; 
double ts = 0.1;  
int N = 100;      

std::vector<double> hxd;
std::vector<double> hyd;
std::vector<double> uxRef;
std::vector<double> uyRef;
std::vector<double> wRef;
std::vector<double> hxe;
std::vector<double> hye;
std::vector<double> hwe;

int k = 0;  
double hxa = 0; 
double hya = 0; 
double phia = 0; 
double phid = 0; 

bool control_active = false;

// Variables PID
double sum_x_err = 0.0;
double sum_y_err = 0.0;
double sum_w_err = 0.0;

interfaces::msg::DataError data_error;
interfaces::msg::DataError data_error_total;

// --- NODO 1: Configuración Inicial ---
class Control_Trajectory_Node : public rclcpp::Node
{
    public:
        Control_Trajectory_Node() : Node("control_trajectory_node")
        {
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/trajectory_control";
            
            subs_trajectory_control = this->create_subscription<interfaces::msg::TrajectoryControl>(
                ss_topic_name.str(), 1, std::bind(&Control_Trajectory_Node::trajectory_control_caller, this, _1));
        }

    private:
        rclcpp::Subscription<interfaces::msg::TrajectoryControl>::SharedPtr subs_trajectory_control;
        
        void trajectory_control_caller(const interfaces::msg::TrajectoryControl::SharedPtr trajectory_msg)
        {   
            tf = trajectory_msg->time; 
        }
};

// --- NODO 2: Suscripción al Path (GENERADOR) ---
class Node_Subs_Path : public rclcpp::Node
{
    public:
        Node_Subs_Path() : Node("node_subs_path")
        {
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/path";
            
            subs_path = this->create_subscription<geometry_msgs::msg::Polygon>(
               ss_topic_name.str(), 1, std::bind(&Node_Subs_Path::subs_path_callback,this,_1));
        }

    private:
        rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path;

        void subs_path_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg)
        {   
            if (path_msg->points.empty()) {
                control_active = false;
                k = 0;
                return;
            }

            int n_points = path_msg->points.size();
            double total_distance = 0.0;
            for(int i = 0; i < n_points - 1; i++) {
                double dx = path_msg->points[i+1].x - path_msg->points[i].x;
                double dy = path_msg->points[i+1].y - path_msg->points[i].y;
                total_distance += std::sqrt(dx*dx + dy*dy);
            }

            double desired_velocity = 1.0; // m/s - Increased significantly for much faster movement
            tf = total_distance / desired_velocity;
            if (tf < 1.5) tf = 1.5;

            N = std::round((tf + ts) / ts);

            hxd.assign(N, 0.0); 
            hyd.assign(N, 0.0);
            uxRef.assign(N + 100, 0.0); 
            uyRef.assign(N + 100, 0.0);
            wRef.assign(N + 100, 0.0);
            hxe.assign(N + 100, 0.0); 
            hye.assign(N + 100, 0.0); 
            hwe.assign(N + 100, 0.0);

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

                    double seg_dist = std::sqrt(pow(p2_x - p1_x, 2) + pow(p2_y - p1_y, 2));
                    int steps_seg = std::round((seg_dist / total_distance) * N);
                    if (steps_seg < 1) steps_seg = 1;

                    double dx = (p2_x - p1_x) / steps_seg;
                    double dy = (p2_y - p1_y) / steps_seg;

                    for (int j = 0; j < steps_seg; j++) {
                        if (current_idx >= N) break;
                        hxd[current_idx] = p1_x + (dx * j);
                        hyd[current_idx] = p1_y + (dy * j);
                        current_idx++;
                    }
                }
                while (current_idx < N) {
                    hxd[current_idx] = path_msg->points[n_points-1].x;
                    hyd[current_idx] = path_msg->points[n_points-1].y;
                    current_idx++;
                }
            }
            
            // RESETEO DE VARIABLES DE CONTROL
            k = 0;
            hxa = hxd[0];
            hya = hyd[0];
            phia = angle_robot; 
            
            sum_x_err = 0.0;
            sum_y_err = 0.0;
            sum_w_err = 0.0;

            control_active = true;
            RCLCPP_INFO(this->get_logger(), "Path Recibido. Control PID Iniciado.");
        }
};

// --- NODO 3: Suscripción a Objetivos ---
class Node_Subs_Positions : public rclcpp::Node
{
    public:
        Node_Subs_Positions() : Node("node_subs_positions")
        {   
            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/objective";
            subs_objective = this->create_subscription<interfaces::msg::RobotObjective>(
                ss_topic_name.str(), 1, std::bind(&Node_Subs_Positions::subs_obj_callback,this,_1));
        }
    private:
        rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;
        void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
            object_id = obj_msg->obj_id;
            angle_objective = obj_msg->angle;
            point_objective = obj_msg->point;
            robot_state = obj_msg->robot_state;

            // IMPORTANTE: Al recibir nuevo objetivo, pausamos control hasta tener nuevo path
            // Esto evita el ciclado infinito si el path tarda en llegar.
            control_active = false;
            k = 0;
            
            // Limpiar integrales
            sum_x_err = 0.0;
            sum_y_err = 0.0;
            sum_w_err = 0.0;
        }
};

// --- NODO 4: Loop de Control ---
class Node_Control_Timer : public rclcpp::Node
{
    public:
        Node_Control_Timer() : Node("node_control_timer")
        {
            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

            std::stringstream ss_base; 
            ss_base << "/robot_0" << robot_id;

            publisher_vel = this->create_publisher<interfaces::msg::PlatformVel>(ss_base.str() + "/set_platform_vel", 1);
            publisher_control_finish = this->create_publisher<interfaces::msg::ControlFinish>(ss_base.str() + "/control_finish", 1);
            publisher_data_error = this->create_publisher<interfaces::msg::DataError>(ss_base.str() + "/data_error", 1);
            publisher_data_error_total = this->create_publisher<interfaces::msg::DataError>(ss_base.str() + "/total_data_error", 1);
            publisher_plot_data = this->create_publisher<std_msgs::msg::Float32MultiArray>(ss_base.str() + "/plot_data", 1);
            
            publisher_markers_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(ss_base.str() + "/control_markers", 1);

            timer_ = this->create_wall_timer(
                 100ms, std::bind(&Node_Control_Timer::timer_callback, this));
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
        rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr publisher_markers_;

    void timer_callback()
    { 
       if (control_active == true) {

            // 1. OBTENER POSICIÓN ACTUAL (TF) CON DIAGNÓSTICO
            try {
                std::string gripper_frame = "robot_0" + std::to_string(robot_id) + "/gr_ref_link";
                // Usamos TimePointZero para obtener la ULTIMA disponible, ignorando diferencias de reloj pequeñas
                auto tf_grip = tf_buffer_->lookupTransform("marker_id_00", gripper_frame, tf2::TimePointZero);
                
                gripper_position.x = tf_grip.transform.translation.x; 
                gripper_position.y = tf_grip.transform.translation.y;

                std::string base_frame = "robot_0" + std::to_string(robot_id) + "/base_link";
                auto tf_base = tf_buffer_->lookupTransform("marker_id_00", base_frame, tf2::TimePointZero);
                
                tf2::Quaternion q(tf_base.transform.rotation.x, tf_base.transform.rotation.y, tf_base.transform.rotation.z, tf_base.transform.rotation.w);
                tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r, p, y);
                angle_robot = y;
            } 
            catch (tf2::TransformException &ex) { 
                // AQUI ESTABA EL PROBLEMA SILENCIOSO
                // Imprimimos el error para saber que está pasando
                RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "TF Error (No puedo ver al robot): %s", ex.what());
                
                // Enviamos velocidad 0 para seguridad mientras no tengamos TF
                interfaces::msg::PlatformVel stop_msg;
                stop_msg.x_vel=0.0; stop_msg.y_vel=0.0; stop_msg.ang_vel=0.0;
                publisher_vel->publish(stop_msg);
                return; 
            }

            phid = angle_objective;

            // ... (Lógica de Path Finding y búsqueda de K se mantiene igual) ...
            // COPIAR: Corrección de Densidad (min_dist_to_path)
            double min_dist_to_path = 9999.0;
            int closest_k = k;
            int search_limit = std::min(k + 100, N);
            for (int i = k; i < search_limit; i++) {
                double d = std::hypot(hxd[i] - gripper_position.x, hyd[i] - gripper_position.y);
                if (d < min_dist_to_path) { min_dist_to_path = d; closest_k = i; }
            }
            if (closest_k > k) k = closest_k;

            // COPIAR: Carrot Chasing (Lookahead)
            double vxd, vyd, vwd, ErrAng;
            double min_lookahead_dist = 0.25; // Reduced for tighter following and better diagonal movement 
            double dist_to_final = std::hypot(hxd[N-1] - gripper_position.x, hyd[N-1] - gripper_position.y);
            if (dist_to_final < min_lookahead_dist) min_lookahead_dist = dist_to_final; 

            int target_idx = k;
            for (int i = k; i < N; i++) {
                double dist = std::hypot(hxd[i] - gripper_position.x, hyd[i] - gripper_position.y);
                if (dist >= min_lookahead_dist) { target_idx = i; break; }
                if (i == N - 1) target_idx = N - 1; 
            }

            // ... (Visualización y Datos de Error se mantienen igual) ...
            if (target_idx < N) {
                int prev_idx = (target_idx - 1 < 0) ? 0 : target_idx - 1;
                vxd = (hxd[target_idx] - hxd[prev_idx]) / ts;    
                vyd = (hyd[target_idx] - hyd[prev_idx]) / ts;
                vwd = (phid - phia) / ts; 
                hxe[k] = hxd[target_idx] - gripper_position.x;
                hye[k] = hyd[target_idx] - gripper_position.y;
            } else {
                vxd = 0; vyd = 0; vwd = 0; 
                hxe[k] = hxd[N-1] - gripper_position.x;
                hye[k] = hyd[N-1] - gripper_position.y;
            }
            
            ErrAng = phid - angle_robot;
            while (ErrAng > M_PI) ErrAng -= 2*M_PI;
            while (ErrAng < -M_PI) ErrAng += 2*M_PI;
            hwe[k] = ErrAng;

            publish_control_markers(gripper_position.x, gripper_position.y, hxd[target_idx], hyd[target_idx], hxe[k], hye[k], hwe[k]);
            
            data_error_total.x_error = hxd[N-1] - gripper_position.x;
            data_error_total.y_error = hyd[N-1] - gripper_position.y;
            data_error_total.ang_error = ErrAng;
            
            // =========================================================
            // 4. LEY DE CONTROL: "FUERZA BRUTA INTELIGENTE"
            // =========================================================

            // Errores actuales directos
            double err_x = hxe[k];
            double err_y = hye[k];
            double err_w = hwe[k];

            // Integrales
            if (dist_to_final < 0.40) { 
                sum_x_err += err_x * ts; sum_y_err += err_y * ts;
            } else { sum_x_err = 0.0; sum_y_err = 0.0; }
            sum_x_err = std::clamp(sum_x_err, -0.15, 0.15);
            sum_y_err = std::clamp(sum_y_err, -0.15, 0.15);

            // Ganancias - Increased significantly for much better response
            double Kx = 30.0; double Ky = 30.0; double Ki = 2.0;
            
            // Supresión de giro si el error es pequeño
            double Kw = (abs(err_w) > 0.05) ? 2.5 : 0.0; // Increased gain and reduced threshold 

            Eigen::MatrixXd he(3,1);
            he << vxd + Kx * tanh(err_x) + Ki * sum_x_err, 
                  vyd + Ky * tanh(err_y) + Ki * sum_y_err, 
                  vwd + Kw * tanh(err_w);

            double cos_th = cos(angle_robot); double sin_th = sin(angle_robot);
            Eigen::MatrixXd J(3,3);
            J << cos_th, -sin_th, 0, sin_th,  cos_th, 0, 0, 0, 1;
            Eigen::MatrixXd qpRef = J.inverse() * he; 

            if (uxRef.size() <= (size_t)k) { uxRef.resize(k+100, 0); uyRef.resize(k+100, 0); wRef.resize(k+100, 0); }
            uxRef[k] = qpRef(0, 0); uyRef[k] = qpRef(1, 0); wRef[k] = qpRef(2, 0);
            
            interfaces::msg::PlatformVel msg_vel;
            
            // --- BOOST DIRECTO BASADO EN ERROR (IGNORANDO PID DÉBIL) ---
            double min_lin_vel = 0.40; // Velocidad mínima para moverse - Much higher for diagonal movement
            double min_ang_vel = 0.30; // Velocidad mínima para girar - Increased significantly
            double gain_lin = 50.0; // Much higher for faster response
            double gain_ang = 8.0; // Higher for better turning

            // Calculamos velocidades base
            double vx_cmd = uxRef[k] * gain_lin;
            double vy_cmd = uyRef[k] * gain_lin;

            // BOOST X: Si hay error en X significativo, aplicamos min_vel
            if (abs(err_x) > 0.005) { // Reduced threshold for more aggressive response
                double sign_x = (err_x > 0) ? 1.0 : -1.0;
                // Si el PID pide poco, forzamos el mínimo en la dirección del error
                if (abs(vx_cmd) < min_lin_vel) vx_cmd = min_lin_vel * sign_x;
            }

            // BOOST Y: Lo mismo para Y
            if (abs(err_y) > 0.005) { // Reduced threshold for more aggressive response
                double sign_y = (err_y > 0) ? 1.0 : -1.0;
                if (abs(vy_cmd) < min_lin_vel) vy_cmd = min_lin_vel * sign_y;
            }

            msg_vel.x_vel = vx_cmd;
            msg_vel.y_vel = vy_cmd;

            // CONTROL DE GIRO
            // Solo giramos si el error es real (> 3 grados) - Reduced for more responsive turning
            if (abs(err_w) > 0.05) {
                double w_cmd = wRef[k] * gain_ang;
                double w_sign = (ErrAng > 0) ? 1.0 : -1.0; // Girar hacia el error
                
                // Corrección de dirección corta (si error > PI)
                if (abs(ErrAng) > M_PI) w_sign *= -1.0;

                // Aplicar mínimo
                if (abs(w_cmd) < min_ang_vel) w_cmd = min_ang_vel * w_sign;
                
                msg_vel.ang_vel = w_cmd;
            } else {
                msg_vel.ang_vel = 0.0; // Bloqueo total de oscilación
            }

            // Clamps de seguridad - Much higher limits for fast operation
            msg_vel.x_vel = std::clamp((double)msg_vel.x_vel, -120.0, 120.0);
            msg_vel.y_vel = std::clamp((double)msg_vel.y_vel, -120.0, 120.0);
            msg_vel.ang_vel = std::clamp((double)msg_vel.ang_vel, -1.0, 1.0);

            publisher_vel->publish(msg_vel);

            k++; // Incrementamos contador de tiempo
            
            // Actualización de referencias
            if (target_idx < N) { hxa = hxd[target_idx]; hya = hyd[target_idx]; } 
            else { hxa = hxd[N-1]; hya = hyd[N-1]; }
            phia = phid;

            // --- CHECK FINISH DIAGNÓSTICO ---
            // Tolerancia fina (1 cm)
            bool pos_ok = abs(data_error_total.x_error) < 0.015 && abs(data_error_total.y_error) < 0.015;
            bool ang_ok = abs(data_error_total.ang_error) < 0.08;
            
            // Timeout de seguridad
            bool hard_timeout = (k >= N + 400);

            if ((pos_ok && ang_ok) || hard_timeout) {
                
                // DIAGNÓSTICO: Esto te dirá la verdad en la terminal
                if (pos_ok && ang_ok) {
                    RCLCPP_INFO(this->get_logger(), "OBJETIVO ALCANZADO (Precision: %.3fm)", 
                        std::hypot(data_error_total.x_error, data_error_total.y_error));
                } else {
                    RCLCPP_WARN(this->get_logger(), "TIMEOUT: Me rendí a %.3fm del objetivo", 
                        std::hypot(data_error_total.x_error, data_error_total.y_error));
                }

                // Frenar y confirmar
                interfaces::msg::PlatformVel stop_msg;
                stop_msg.x_vel=0.0; stop_msg.y_vel=0.0; stop_msg.ang_vel=0.0;
                publisher_vel->publish(stop_msg);

                interfaces::msg::ControlFinish finish_msg;
                finish_msg.finish_confirm = true;
                publisher_control_finish->publish(finish_msg);

                control_active = false;
                k = 0;
            }
       } else {
           interfaces::msg::PlatformVel stop_msg;
           stop_msg.x_vel=0.0; stop_msg.y_vel=0.0; stop_msg.ang_vel=0.0;
           publisher_vel->publish(stop_msg);
       }
    }

    void publish_control_markers(double curr_x, double curr_y, double targ_x, double targ_y, double ex, double ey, double eth) {
        visualization_msgs::msg::MarkerArray markers;
        // Flecha
        visualization_msgs::msg::Marker arrow;
        arrow.header.frame_id = "marker_id_00"; arrow.header.stamp = this->now();
        arrow.ns = "error_vector"; arrow.id = 0; arrow.type = visualization_msgs::msg::Marker::ARROW;
        arrow.action = visualization_msgs::msg::Marker::ADD;
        geometry_msgs::msg::Point p1, p2; p1.x = curr_x; p1.y = curr_y; p1.z = 0.1; p2.x = targ_x; p2.y = targ_y; p2.z = 0.1;
        arrow.points.push_back(p1); arrow.points.push_back(p2);
        arrow.scale.x = 0.02; arrow.scale.y = 0.04; arrow.scale.z = 0.05; 
        arrow.color.r = 1.0; arrow.color.a = 1.0; 
        markers.markers.push_back(arrow);

        // Texto
        visualization_msgs::msg::Marker text;
        text.header.frame_id = "marker_id_00"; text.header.stamp = this->now();
        text.ns = "error_text"; text.id = 1; text.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
        text.action = visualization_msgs::msg::Marker::ADD;
        text.pose.position.x = curr_x + 0.3; text.pose.position.y = curr_y - 0.3; text.pose.position.z = 0.5; 
        text.scale.z = 0.05; 
        text.color.r = 1.0; text.color.g = 1.0; text.color.b = 0.0; text.color.a = 1.0; 
        std::stringstream ss;
        ss << "ex:" << std::fixed << std::setprecision(2) << ex << "m\n";
        ss << "ey:" << std::fixed << std::setprecision(2) << ey << "m\n";
        text.text = ss.str();
        markers.markers.push_back(text);

        // Carrot
        visualization_msgs::msg::Marker sphere;
        sphere.header.frame_id = "marker_id_00"; sphere.header.stamp = this->now();
        sphere.ns = "target_point"; sphere.id = 2; sphere.type = visualization_msgs::msg::Marker::SPHERE;
        sphere.action = visualization_msgs::msg::Marker::ADD;
        sphere.pose.position.x = targ_x; sphere.pose.position.y = targ_y; sphere.pose.position.z = 0.1;
        sphere.scale.x = 0.05; sphere.scale.y = 0.05; sphere.scale.z = 0.05;
        sphere.color.r = 0.0; sphere.color.g = 1.0; sphere.color.b = 0.0; sphere.color.a = 0.8; 
        markers.markers.push_back(sphere);

        publisher_markers_->publish(markers);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<Control_Trajectory_Node>();
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
    return 0;
}