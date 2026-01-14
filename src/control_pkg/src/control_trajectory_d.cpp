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

// --- NUEVO: Variables para el término Integral (PID) ---
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

            double desired_velocity = 0.35; // m/s
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
            
            // IMPORTANTE: Resetear integrales para el nuevo camino
            sum_x_err = 0.0;
            sum_y_err = 0.0;
            sum_w_err = 0.0;

            control_active = true;
            RCLCPP_INFO(this->get_logger(), "Path recibido. Control PID Iniciado.");
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
            
            // Publisher para RVIZ
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

            // 1. OBTENER POSICIÓN ACTUAL (TF)
            try {
                std::string gripper_frame = "robot_0" + std::to_string(robot_id) + "/gr_ref_link";
                auto tf_grip = tf_buffer_->lookupTransform("marker_id_00", gripper_frame, tf2::TimePointZero);
                
                // Sin offset manual, confiamos en el TF
                gripper_position.x = tf_grip.transform.translation.x; 
                gripper_position.y = tf_grip.transform.translation.y;

                std::string base_frame = "robot_0" + std::to_string(robot_id) + "/base_link";
                auto tf_base = tf_buffer_->lookupTransform("marker_id_00", base_frame, tf2::TimePointZero);
                
                tf2::Quaternion q(tf_base.transform.rotation.x, tf_base.transform.rotation.y, tf_base.transform.rotation.z, tf_base.transform.rotation.w);
                tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r, p, y);
                angle_robot = y;
            } catch (tf2::TransformException &ex) { return; }

            phid = angle_objective;

            // ============================================================
            // CORRECCIÓN DE DENSIDAD: SINCRONIZAR 'k' CON LA REALIDAD
            // ============================================================
            
            // Buscamos el punto del camino más cercano a la posición real del robot.
            // Esto evita que 'k' se quede atrasado si el robot va rápido.
            
            double min_dist_to_path = 9999.0;
            int closest_k = k;
            
            // Buscamos solo en una ventana hacia adelante (ej. 100 puntos) para no saltar
            // a otra parte del camino si se cruza (como en una pista de carreras)
            int search_window = 100; 
            int search_limit = std::min(k + search_window, N);

            for (int i = k; i < search_limit; i++) {
                double d = std::hypot(hxd[i] - gripper_position.x, hyd[i] - gripper_position.y);
                if (d < min_dist_to_path) {
                    min_dist_to_path = d;
                    closest_k = i;
                }
            }
            
            // Actualizamos k. PERO nunca permitimos que k retroceda (hysteresis)
            if (closest_k > k) {
                k = closest_k;
            }

            // 2. BUSQUEDA ADAPTATIVA DEL OBJETIVO (CARROT CHASING)
            double vxd, vyd, vwd, ErrAng;
            
            double min_lookahead_dist = 0.15; // 15 cm
            double dist_to_final = std::hypot(hxd[N-1] - gripper_position.x, hyd[N-1] - gripper_position.y);
            
            if (dist_to_final < min_lookahead_dist) {
                min_lookahead_dist = dist_to_final; 
            }

            int target_idx = k;
            for (int i = k; i < N; i++) {
                double dist = std::hypot(hxd[i] - gripper_position.x, hyd[i] - gripper_position.y);
                if (dist >= min_lookahead_dist) {
                    target_idx = i;
                    break;
                }
                if (i == N - 1) target_idx = N - 1; 
            }

            // Variables para visualización RVIZ
            double target_x_vis = 0;
            double target_y_vis = 0;

            if (target_idx < N) {
                int prev_idx = (target_idx - 1 < 0) ? 0 : target_idx - 1;
                vxd = (hxd[target_idx] - hxd[prev_idx]) / ts;    
                vyd = (hyd[target_idx] - hyd[prev_idx]) / ts;
                vwd = (phid - phia) / ts; 

                hxe[k] = hxd[target_idx] - gripper_position.x;
                hye[k] = hyd[target_idx] - gripper_position.y;
                
                target_x_vis = hxd[target_idx];
                target_y_vis = hyd[target_idx];
            } else {
                vxd = 0; vyd = 0; vwd = 0; 
                hxe[k] = hxd[N-1] - gripper_position.x;
                hye[k] = hyd[N-1] - gripper_position.y;
                target_x_vis = hxd[N-1];
                target_y_vis = hyd[N-1];
            }

            ErrAng = phid - angle_robot;
            while (ErrAng > M_PI) ErrAng -= 2*M_PI;
            while (ErrAng < -M_PI) ErrAng += 2*M_PI;
            hwe[k] = ErrAng;

            // 3. PUBLICAR DATOS Y VISUALIZACIÓN
            publish_control_markers(gripper_position.x, gripper_position.y, target_x_vis, target_y_vis, hxe[k], hye[k], hwe[k]);

            data_error.x_error = hxe[k]; data_error.y_error = hye[k]; data_error.ang_error = hwe[k]; data_error.k = k;
            publisher_data_error->publish(data_error);

            // Error TOTAL al final del camino (para checar si terminamos)
            data_error_total.x_error = hxd[N-1] - gripper_position.x;
            data_error_total.y_error = hyd[N-1] - gripper_position.y;
            data_error_total.ang_error = ErrAng;
            data_error_total.k = k;
            publisher_data_error_total->publish(data_error_total);

            std_msgs::msg::Float32MultiArray msg_plot;
            msg_plot.data = { (float)robot_state, (float)hxe[k], (float)hye[k], (float)hwe[k],
                (float)data_error_total.x_error, (float)data_error_total.y_error, (float)data_error_total.ang_error,
                (float)hxd[N-1], (float)hyd[N-1], (float)phid };
            publisher_plot_data->publish(msg_plot);

            // =========================================================
            // 4. LEY DE CONTROL (SINTONIZACIÓN SUAVE / SOFT LANDING)
            // =========================================================
            
            // CAMBIO 1: ZONA DE ACTIVACIÓN MÁS PEQUEÑA
            // Antes: 1.5m. Ahora: 0.40m (40 cm).
            // No queremos que acumule error desde lejos, solo al final.
            if (dist_to_final < 0.40) { 
                sum_x_err += hxe[k] * ts;
                sum_y_err += hye[k] * ts;
                // sum_w_err += hwe[k] * ts; 
            } else {
                // Importante: Si nos alejamos, limpiar el integral para no guardar "basura"
                sum_x_err = 0.0;
                sum_y_err = 0.0;
            }
            
            // CAMBIO 2: SATURACIÓN MÁS ESTRICTA
            // Antes: 1.5. Ahora: 0.3 (Max 30 cm/s de corrección integral)
            // Esto evita que el integral dé "patadas" fuertes.
            double max_I = 0.3; 
            sum_x_err = std::clamp(sum_x_err, -max_I, max_I);
            sum_y_err = std::clamp(sum_y_err, -max_I, max_I);

            // CAMBIO 3: GANANCIAS BALANCEADAS
            // Kx/Ky: Bajamos de 60 a 25 para que no se acerque tan violento.
            double Kx = 25.0; 
            double Ky = 25.0;
            double Kw = 3.0; // El angular suele estar bien bajo
            
            // Ki: Bajamos de 35 a 5.0. 
            // 5.0 es suficiente para empujar suavemente los últimos cm sin causar rebote.
            double Ki = 20.0; 

            Eigen::MatrixXd he(3,1);
            he << vxd + Kx * tanh(hxe[k]) + Ki * sum_x_err, 
                  vyd + Ky * tanh(hye[k]) + Ki * sum_y_err, 
                  vwd + Kw * tanh(hwe[k]);

            double cos_th = cos(angle_robot); double sin_th = sin(angle_robot);
            Eigen::MatrixXd J(3,3);
            J << cos_th, -sin_th, 0, sin_th,  cos_th, 0, 0, 0, 1;

            Eigen::MatrixXd qpRef = J.inverse() * he; 

            if (uxRef.size() <= (size_t)k) { uxRef.resize(k+100, 0); uyRef.resize(k+100, 0); wRef.resize(k+100, 0); }
            uxRef[k] = qpRef(0, 0); uyRef[k] = qpRef(1, 0); wRef[k] = qpRef(2, 0);
            
            interfaces::msg::PlatformVel msg_vel;
            double gain_vel = 35.0; 
            msg_vel.x_vel = uxRef[k] * gain_vel;
            msg_vel.y_vel = uyRef[k] * gain_vel;
            if (abs(ErrAng) > M_PI) msg_vel.ang_vel = wRef[k] * -0.3; else msg_vel.ang_vel = wRef[k];

            publisher_vel->publish(msg_vel);

            k++;
            
            // Actualizar anterior
            if (target_idx < N) {
                hxa = hxd[target_idx];
                hya = hyd[target_idx];
            } else {
                hxa = hxd[N-1];
                hya = hyd[N-1];
            }
            phia = phid;

            // 5. CHECK FINISH (LÓGICA MEJORADA)
            
            // Tolerancias: Si el error baja de 1cm, cortamos INMEDIATAMENTE.
            bool pos_ok = abs(data_error_total.x_error) < 0.01 && abs(data_error_total.y_error) < 0.01;
            bool ang_ok = abs(data_error_total.ang_error) < 0.08;
            
            // Timeout de Seguridad (Solo si algo falla físicamente y pasan 30 segundos extra)
            bool hard_timeout = (k >= N + 300);

            if ((pos_ok && ang_ok) || hard_timeout) {
                
                if (pos_ok) RCLCPP_INFO(this->get_logger(), "¡OBJETIVO ALCANZADO! (Por precisión)");
                else RCLCPP_INFO(this->get_logger(), "TIMEOUT (Por seguridad)");

                msg_vel.x_vel = 0.0; msg_vel.y_vel = 0.0; msg_vel.ang_vel = 0.0;
                publisher_vel->publish(msg_vel);

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
        arrow.header.frame_id = "marker_id_00";
        arrow.header.stamp = this->now();
        arrow.ns = "error_vector";
        arrow.id = 0;
        arrow.type = visualization_msgs::msg::Marker::ARROW;
        arrow.action = visualization_msgs::msg::Marker::ADD;
        geometry_msgs::msg::Point p1, p2;
        p1.x = curr_x; p1.y = curr_y; p1.z = 0.1;
        p2.x = targ_x; p2.y = targ_y; p2.z = 0.1;
        arrow.points.push_back(p1); arrow.points.push_back(p2);
        arrow.scale.x = 0.02; arrow.scale.y = 0.04; arrow.scale.z = 0.05; 
        arrow.color.r = 1.0; arrow.color.a = 1.0; 
        markers.markers.push_back(arrow);

        // Texto
        visualization_msgs::msg::Marker text;
        text.header.frame_id = "marker_id_00";
        text.header.stamp = this->now();
        text.ns = "error_text";
        text.id = 1;
        text.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
        text.action = visualization_msgs::msg::Marker::ADD;
        text.pose.position.x = curr_x + 0.3;
        text.pose.position.y = curr_y - 0.3;
        text.pose.position.z = 0.5; 
        text.scale.z = 0.05; 
        text.color.r = 1.0; text.color.g = 1.0; text.color.b = 0.0; text.color.a = 1.0; 
        std::stringstream ss;
        ss << "ex:" << std::fixed << std::setprecision(2) << ex << "m\n";
        ss << "ey:" << std::fixed << std::setprecision(2) << ey << "m\n";
        ss << "eTh:" << std::fixed << std::setprecision(2) << eth << "rad";
        text.text = ss.str();
        markers.markers.push_back(text);

        // Carrot Point
        visualization_msgs::msg::Marker sphere;
        sphere.header.frame_id = "marker_id_00";
        sphere.header.stamp = this->now();
        sphere.ns = "target_point";
        sphere.id = 2;
        sphere.type = visualization_msgs::msg::Marker::SPHERE;
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