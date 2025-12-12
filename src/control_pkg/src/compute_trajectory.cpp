#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>   // Para sleep_for
#include <cstdlib>  // Para rand()

// OpenCV solo para cálculos matemáticos
#include <opencv2/core.hpp> 
#include <opencv2/imgproc.hpp> 

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <geometry_msgs/msg/pose.hpp>
#include <interfaces/msg/positions.hpp>
#include <interfaces/srv/path_finding.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <geometry_msgs/msg/point32.h>
#include <interfaces/msg/robot_objective.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/buffer.h>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>

// RViz Messages
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <nav_msgs/msg/path.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

#include <rmw/qos_profiles.h>
#include <rclcpp/qos.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;
#define PI 3.14159265358979323846

class Compute_Trajectory : public rclcpp::Node
{
public:
    Compute_Trajectory() : Node("compute_trajectory")
    {
        // 1. Parámetros Generales
        this->declare_parameter<int>("robot_id", 0);
        robot_id = this->get_parameter("robot_id").as_int();
        leader_robot_id = robot_id;

        // NUEVO: Parámetro para saber qué algoritmo estamos usando (solo para logs)
        this->declare_parameter<std::string>("planner_name", "Unknown_Planner");
        planner_name_ = this->get_parameter("planner_name").as_string();

        RCLCPP_INFO(this->get_logger(), "Iniciando Trajectory Controller para Robot %d usando [%s]", robot_id, planner_name_.c_str());

        // Parámetros de Dimensiones
        this->declare_parameter<float>("robot_size_x", 0.7);
        this->declare_parameter<float>("robot_size_y", 0.6);
        this->declare_parameter<float>("object_size_small_x", 0.4);
        this->declare_parameter<float>("object_size_small_y", 0.4);
        this->declare_parameter<float>("object_size_big_x", 0.6);
        this->declare_parameter<float>("object_size_big_y", 0.2);

        float r_x = this->get_parameter("robot_size_x").as_double();
        float r_y = this->get_parameter("robot_size_y").as_double();
        float os_x = this->get_parameter("object_size_small_x").as_double();
        float os_y = this->get_parameter("object_size_small_y").as_double();
        float ob_x = this->get_parameter("object_size_big_x").as_double();
        float ob_y = this->get_parameter("object_size_big_y").as_double();

        robot_size_x_cells = static_cast<int>(r_x / x_grid);
        robot_size_y_cells = static_cast<int>(r_y / y_grid);
        object_size_small_x_cells = static_cast<int>(os_x / x_grid);
        object_size_small_y_cells = static_cast<int>(os_y / y_grid);
        object_size_big_x_cells = static_cast<int>(ob_x / x_grid);
        object_size_big_y_cells = static_cast<int>(ob_y / y_grid);

        // 2. TF Listener
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

        // 3. Publicadores
        std::stringstream ss_topic_name;
        ss_topic_name << "/robot_0" << robot_id << "/path";
        publisher_path = this->create_publisher<geometry_msgs::msg::Polygon>(ss_topic_name.str(), 10);

        // Publicadores para RViz (Debug)
        publisher_grid_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("debug_grid", 10);
        publisher_markers_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("debug_markers", 10);
        publisher_nav_path_ = this->create_publisher<nav_msgs::msg::Path>("debug_path", 10);

        // 4. Suscripciones y Clientes
        client_cb_group = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

        std::stringstream ss_topic_obj;
        ss_topic_obj << "/robot_0" << robot_id << "/objective";
        subs_objective = this->create_subscription<interfaces::msg::RobotObjective>(
            ss_topic_obj.str(), 1, std::bind(&Compute_Trajectory::subs_obj_callback, this, _1));

        std::stringstream ss_topic_task;
        ss_topic_task << "/robot_0" << robot_id << "/task_assigned";
        subs_task_assigned = this->create_subscription<interfaces::msg::TaskDescription>(
            ss_topic_task.str(), 1, std::bind(&Compute_Trajectory::task_assigned_callback, this, _1));

        // Cliente del Servicio (El nombre es fijo, el algoritmo cambia en el launch)
        std::stringstream ss_service_name;
        ss_service_name << "/robot_0" << robot_id << "/path_finding_server";
        client = this->create_client<interfaces::srv::PathFinding>(
            ss_service_name.str(), rmw_qos_profile_services_default, client_cb_group);

        // Diagnóstico Inicial
        if (!client->wait_for_service(std::chrono::seconds(1))) {
            RCLCPP_WARN(this->get_logger(), "Servicio '%s' no disponible al inicio.", ss_service_name.str().c_str());
        }

        subscribe_to_others();
        
        timer_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        timer_ = this->create_wall_timer(
             500ms, std::bind(&Compute_Trajectory::timer_callback, this), timer_cb_group_);
    }

private:
    // Configuración
    float x_grid = 0.05;
    float y_grid = 0.05;
    float x_world = 2.0 * 4.0 * tan(1.39626 / 2.0);  
    float y_world = (2.0 * 4.0 * tan(1.39626 / 2.0)) * (9.0 / 16.0);
    int n_x_spaces = 160;
    int n_y_spaces = 90;

    int robot_id;
    int leader_robot_id;
    int robot_state = 0;
    std::string planner_name_; // Variable para guardar el nombre (A*, Greedy, etc)
    
    int robot_size_x_cells, robot_size_y_cells;
    int object_size_small_x_cells, object_size_small_y_cells;
    int object_size_big_x_cells, object_size_big_y_cells;

    // Estado
    geometry_msgs::msg::Point point_objective;
    float angle_objective = 0;
    int object_id = 0;
    
    geometry_msgs::msg::Point robot_position;
    geometry_msgs::msg::Point gripper_position;
    float angle_robot = 0;
    
    geometry_msgs::msg::Point object_position;
    float angle_object = 0;
    float type_object = 0;

    std::vector<geometry_msgs::msg::Point> obstacle_position;
    std::vector<float> angle_obstacle;
    std::vector<float> type_obstacle;
    int n_obstacles = 0;

    std::vector<cv::Mat> other_robot_path_mats;
    std::vector<rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr> other_path_subs;
    geometry_msgs::msg::Polygon path_leader;

    std::mutex path_mutex_; 
    geometry_msgs::msg::Polygon path_ant;
    bool request_pending_ = false; 

    // ROS
    rclcpp::Publisher<geometry_msgs::msg::Polygon>::SharedPtr publisher_path;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr publisher_grid_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr publisher_markers_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_nav_path_;

    rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;
    rclcpp::Subscription<interfaces::msg::TaskDescription>::SharedPtr subs_task_assigned;
    rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path_leader;
    rclcpp::Client<interfaces::srv::PathFinding>::SharedPtr client;
    rclcpp::TimerBase::SharedPtr timer_;
    
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    
    rclcpp::CallbackGroup::SharedPtr client_cb_group;
    rclcpp::CallbackGroup::SharedPtr timer_cb_group_;

    void subscribe_to_others() {
        other_robot_path_mats.resize(10); 
        for (int i = 1; i <= 9; ++i) {
            if (i == robot_id) continue;
            std::stringstream ss;
            ss << "/robot_0" << i << "/path";
            auto sub = this->create_subscription<geometry_msgs::msg::Polygon>(
                ss.str(), 1,
                [this, i](const geometry_msgs::msg::Polygon::SharedPtr msg) {
                    cv::Mat mat = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);
                    for (const auto &point : msg->points) {
                        int x_map = static_cast<int>((point.x * n_x_spaces) / x_world) + (n_x_spaces / 2);
                        int y_map = n_y_spaces - (static_cast<int>((point.y * n_y_spaces) / y_world) + (n_y_spaces / 2));
                        if (x_map >= 0 && x_map < n_x_spaces && y_map >= 0 && y_map < n_y_spaces) {
                            mat.at<uchar>(y_map, x_map) = 255;
                        }
                    }
                    other_robot_path_mats[i] = mat;
                });
            other_path_subs.push_back(sub);
        }
    }

    void path_leader_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg) {   
        path_leader = *path_msg;
    }

    void update_subscription(){
        subs_path_leader.reset();
        std::stringstream ss;
        ss << "/robot_0" << leader_robot_id << "/path";
        subs_path_leader = this->create_subscription<geometry_msgs::msg::Polygon>(
            ss.str(), 1, std::bind(&Compute_Trajectory::path_leader_callback, this, _1));
    }

    void task_assigned_callback(const interfaces::msg::TaskDescription::SharedPtr task_msg){
        if (task_msg->leader_robot_id != 0 && task_msg->leader_robot_id != robot_id) {
            leader_robot_id = task_msg->leader_robot_id;
            update_subscription();
        } else {
            leader_robot_id = robot_id;
        }
        point_objective = task_msg->goal;
        angle_objective = task_msg->angle_goal;
        object_id = task_msg->obj_id;
        robot_state = task_msg->state;
    }

    void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
        object_id = obj_msg->obj_id;
        angle_objective = obj_msg->angle;
        point_objective = obj_msg->point;
        robot_state = obj_msg->robot_state;
    }

    // --- VISUALIZACIÓN RVIZ ---
    void publish_debug_grid(const cv::Mat& map_bin_ext) {
        nav_msgs::msg::OccupancyGrid grid_msg;
        grid_msg.header.stamp = this->now();
        grid_msg.header.frame_id = "marker_id_00";

        grid_msg.info.resolution = x_world / n_x_spaces; 
        grid_msg.info.width = n_x_spaces;
        grid_msg.info.height = n_y_spaces;
        
        grid_msg.info.origin.position.x = -x_world / 2.0;
        grid_msg.info.origin.position.y = -y_world / 2.0;
        grid_msg.info.origin.position.z = -0.1; 
        grid_msg.info.origin.orientation.w = 1.0;

        grid_msg.data.resize(n_x_spaces * n_y_spaces);

        for (int y = 0; y < n_y_spaces; y++) {
            for (int x = 0; x < n_x_spaces; x++) {
                int ros_y = n_y_spaces - 1 - y;
                int ros_idx = ros_y * n_x_spaces + x;
                // 100 = ocupado (negro), 0 = libre (blanco/transparente)
                if (map_bin_ext.at<uchar>(y, x) == 0) {
                    grid_msg.data[ros_idx] = 100;
                } else {
                    grid_msg.data[ros_idx] = 0;
                }
            }
        }
        publisher_grid_->publish(grid_msg);
    }

    void publish_debug_markers(int sx, int sy, int gx, int gy) {
        visualization_msgs::msg::MarkerArray markers;
        
        auto create_marker = [&](int id, float r, float g, float b, double x, double y, std::string ns) {
            visualization_msgs::msg::Marker m;
            m.header.frame_id = "marker_id_00";
            m.header.stamp = this->now();
            m.ns = ns;
            m.id = id;
            m.type = visualization_msgs::msg::Marker::SPHERE;
            m.action = visualization_msgs::msg::Marker::ADD;
            m.pose.position.x = (x - (n_x_spaces/2)) * x_world / n_x_spaces;
            m.pose.position.y = -(y - (n_y_spaces/2)) * y_world / n_y_spaces;
            m.pose.position.z = 0.2; 
            m.scale.x = 0.05; m.scale.y = 0.05; m.scale.z = 0.05;
            m.color.r = r; m.color.g = g; m.color.b = b; m.color.a = 1.0;
            return m;
        };

        markers.markers.push_back(create_marker(0, 0.0, 1.0, 0.0, sx, sy, "start_pt")); 
        markers.markers.push_back(create_marker(1, 1.0, 0.0, 0.0, gx, gy, "goal_pt"));  

        publisher_markers_->publish(markers);
    }

    // --- CALLBACK PRINCIPAL ---
    void response_callback(rclcpp::Client<interfaces::srv::PathFinding>::SharedFuture future) {
        request_pending_ = false; 
        
        try {
            auto result = future.get();
            int path_size = result->path_size;
            
            if (path_size > 0) {
                // LOG DINÁMICO: Usa el nombre del planner
                RCLCPP_INFO(this->get_logger(), "[%s] Camino Recibido (%d pts).", planner_name_.c_str(), path_size);
                
                geometry_msgs::msg::Polygon path_msg;
                nav_msgs::msg::Path nav_path; // Para RViz
                
                nav_path.header.stamp = this->now();
                nav_path.header.frame_id = "marker_id_00";

                std::vector<int> path_x = result->path_x;
                std::vector<int> path_y = result->path_y;
                
                for (int i = 0; i < path_size; i++) {
                    geometry_msgs::msg::Point32 point;
                    point.x = (path_x[i] - (n_x_spaces/2)) * x_world / n_x_spaces;
                    point.y = -(path_y[i] - (n_y_spaces/2)) * y_world / n_y_spaces;
                    path_msg.points.push_back(point);

                    geometry_msgs::msg::PoseStamped pose;
                    pose.header = nav_path.header;
                    pose.pose.position.x = point.x;
                    pose.pose.position.y = point.y;
                    pose.pose.position.z = 0.05; 
                    nav_path.poses.push_back(pose);
                }

                {
                    std::lock_guard<std::mutex> lock(path_mutex_);
                    path_ant = path_msg;
                }
                
                publisher_path->publish(path_msg);      
                publisher_nav_path_->publish(nav_path); 

            } else {
                // --- LÓGICA DE TRÁFICO (Ceder el paso) ---
                RCLCPP_WARN(this->get_logger(), "[TRAFFIC] Bloqueado. Cediendo el paso...");

                // Parada de emergencia
                geometry_msgs::msg::Polygon stop_msg;
                publisher_path->publish(stop_msg);

                // Espera aleatoria proporcional al ID
                int wait_ms = (robot_id * 200) + (std::rand() % 500);
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
            }

        } catch (const std::exception &e) {
            RCLCPP_ERROR(this->get_logger(), "Error Servicio: %s", e.what());
        }
    }

    void timer_callback()
    {
        n_obstacles = 0;
        obstacle_position.clear();
        angle_obstacle.clear();
        type_obstacle.clear();

        for (int marker=1; marker<30; marker++){
            std::stringstream ss_marker;
            ss_marker << "marker_id_" << (marker < 10 ? "0" : "") << marker;
            std::string marker_name = ss_marker.str();

            try {
                if(robot_id == marker){
                    std::stringstream ss_base; ss_base << "robot_0" << robot_id << "/base_link";
                    auto tf_base = tf_buffer_->lookupTransform("marker_id_00", ss_base.str(), tf2::TimePointZero);
                    robot_position.x = tf_base.transform.translation.x;
                    robot_position.y = tf_base.transform.translation.y;
                    tf2::Quaternion q(tf_base.transform.rotation.x, tf_base.transform.rotation.y, tf_base.transform.rotation.z, tf_base.transform.rotation.w);
                    tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r,p,y);
                    angle_robot = y;

                    std::stringstream ss_grip; ss_grip << "robot_0" << robot_id << "/gr_ref_link";
                    auto tf_grip = tf_buffer_->lookupTransform("marker_id_00", ss_grip.str(), tf2::TimePointZero);
                    gripper_position.x = tf_grip.transform.translation.x;
                    gripper_position.y = tf_grip.transform.translation.y;
                }
                else if(object_id == marker && robot_state >= 2){
                    auto tf_obj = tf_buffer_->lookupTransform("marker_id_00", marker_name, tf2::TimePointZero);
                    object_position.x = tf_obj.transform.translation.x;
                    object_position.y = tf_obj.transform.translation.y;
                    tf2::Quaternion q(tf_obj.transform.rotation.x, tf_obj.transform.rotation.y, tf_obj.transform.rotation.z, tf_obj.transform.rotation.w);
                    tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r,p,y);
                    angle_object = y;
                    type_object = (marker > 20) ? 2 : 1;
                }
                else {
                    auto tf_obs = tf_buffer_->lookupTransform("marker_id_00", marker_name, tf2::TimePointZero);
                    n_obstacles++;
                    geometry_msgs::msg::Point pt;
                    pt.x = tf_obs.transform.translation.x;
                    pt.y = tf_obs.transform.translation.y;
                    obstacle_position.push_back(pt);
                    
                    tf2::Quaternion q(tf_obs.transform.rotation.x, tf_obs.transform.rotation.y, tf_obs.transform.rotation.z, tf_obs.transform.rotation.w);
                    tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r,p,y);
                    angle_obstacle.push_back(y);
                    
                    if(marker < 10) type_obstacle.push_back(0);
                    else if(marker < 20) type_obstacle.push_back(1);
                    else type_obstacle.push_back(2);
                }
            }
            catch (tf2::TransformException &ex){
                continue; 
            }
        }

        // --- GRID GENERATION ---
        cv::Mat map_bin = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);
        map_bin = cv::Scalar(255); 

        // Obstáculos
        for(int i=0; i<n_obstacles; i++) {
            int ox = ((int)((obstacle_position[i].x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int oy = n_y_spaces - (((int)((obstacle_position[i].y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Size sz;
            if(type_obstacle[i]==0) sz = cv::Size(robot_size_x_cells, robot_size_y_cells);
            else if(type_obstacle[i]==1) sz = cv::Size(object_size_small_x_cells, object_size_small_y_cells);
            else sz = cv::Size(object_size_big_x_cells, object_size_big_y_cells);
            
            cv::RotatedRect obs_rect(cv::Point(ox, oy), sz, -angle_obstacle[i]*180/PI);
            cv::Point2f v_obs[4]; obs_rect.points(v_obs);
            std::vector<cv::Point> poly_obs; for(int j=0;j<4;j++) poly_obs.push_back(v_obs[j]);
            
            cv::fillConvexPoly(map_bin, poly_obs, cv::Scalar(0)); 
        }

        for (const auto& mat : other_robot_path_mats) {
            if (mat.empty()) continue;
             for (int i = 0; i < mat.rows; i++) {
                for (int j = 0; j < mat.cols; j++) {
                    if (mat.at<uchar>(i, j) == 255) map_bin.at<uchar>(i, j) = 0;
                }
            }
        }

        int start_x = ((int)((gripper_position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
        int start_y = n_y_spaces - (((int)((gripper_position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
        
        int goal_x = ((int)((point_objective.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
        int goal_y = n_y_spaces - (((int)((point_objective.y * n_y_spaces)/y_world)) + (n_y_spaces/2));

        auto clamp = [&](int &val, int max_val) { if(val<0) val=0; if(val>=max_val) val=max_val-1; };
        clamp(start_x, n_x_spaces); clamp(start_y, n_y_spaces);
        clamp(goal_x, n_x_spaces); clamp(goal_y, n_y_spaces);

        // Inflado
        cv::Mat map_bin_ext = map_bin.clone();
        int kernel_size = 2; 
        for (int i=0; i<n_y_spaces; i++){
            for (int j=0; j<n_x_spaces; j++){
                if (map_bin.at<uchar>(i,j) == 0){ 
                    for (int k = -kernel_size; k <= kernel_size; k++){
                        for (int l = -kernel_size; l <= kernel_size; l++){
                             int ny = i+k, nx = j+l;
                             if(ny>=0 && ny<n_y_spaces && nx>=0 && nx<n_x_spaces)
                                map_bin_ext.at<uchar>(ny, nx) = 0;
                        }
                    }
                }
            }
        }

        // --- PUBLICAR DEBUG (RVIZ) ---
        publish_debug_grid(map_bin_ext);
        publish_debug_markers(start_x, start_y, goal_x, goal_y);

        // --- PETICIÓN SERVICIO ---
        auto request = std::make_shared<interfaces::srv::PathFinding::Request>();
        request->src_x = start_x;
        request->src_y = start_y;
        request->dst_x = goal_x;
        request->dst_y = goal_y;
        
        std::vector<int> grid_vect;
        grid_vect.reserve(n_x_spaces * n_y_spaces);
        
        for(int i=0; i<n_y_spaces; i++) {
            for(int j=0; j<n_x_spaces; j++) {
                if ((i==start_y && j==start_x) || (i==goal_y && j==goal_x)) {
                     grid_vect.push_back(1);
                } else {
                     grid_vect.push_back(map_bin_ext.at<uchar>(i, j) == 0 ? 0 : 1);
                }
            }
        }
        request->grid = grid_vect;

        if (client->service_is_ready()) {
            if (!request_pending_) {
                request_pending_ = true; 
                client->async_send_request(request, 
                    std::bind(&Compute_Trajectory::response_callback, this, _1));
            }
        } 
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Compute_Trajectory>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}