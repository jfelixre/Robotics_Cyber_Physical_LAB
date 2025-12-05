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


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <stdio.h>

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <geometry_msgs/msg/pose.hpp>
#include <interfaces/msg/positions.hpp>
#include <interfaces/srv/a_star_service.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <geometry_msgs/msg/point32.h>
#include <interfaces/msg/robot_objective.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include "tf2_ros/buffer.h"
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>

#include <rmw/qos_profiles.h>
#include <rclcpp/qos.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;
# define PI 3.14159265358979323846

// geometry_msgs::msg::Pose Robot1, Robot2, Object1, Object2, Target, Initial, Saved;
// int n_objective = -1;
// float distance_objective = 0;
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
std::vector<geometry_msgs::msg::Point> obstacle_position;
std::vector<float> angle_obstacle;
std::vector<float> type_obstacle;   //0: Robot, 1: Single object, 2: Double object
int n_obstacles = 0;
int leader_robot_id = 0;
std::vector<geometry_msgs::msg::Polygon> other_robot_paths;
std::vector<cv::Mat> other_robot_path_mats;
std::vector<rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr> other_path_subs;
int robot_state = 0;
geometry_msgs::msg::Polygon path_leader;



float x_grid = 0.05;    //All dimensions in meters
float y_grid = 0.05;
float x_world = 6;        
float y_world = 3;          

int n_x_spaces = (int)x_world/x_grid;
int n_y_spaces = (int)y_world/y_grid;


geometry_msgs::msg::Polygon path_ant;

cv::Mat map_color(n_y_spaces, n_x_spaces, CV_8UC3, cv::Scalar(255, 255, 255));
cv::Point goal_f;
cv::Point Robot_grip_point_f;
cv::Point Robot_center_point_f;
cv::Mat path_leader_mat = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);

//rclcpp::Client<interfaces::srv::AStarService>::SharedPtr client;



class Compute_Trajectory : public rclcpp::Node
{
    public:
        Compute_Trajectory() : Node("compute_trajectory")
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Number of spaces x: %d", n_x_spaces);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Number of spaces y: %d", n_y_spaces);

            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            // Parameters for sizes in meters
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

            // Convert to grid cells
            robot_size_x_cells = static_cast<int>(r_x / x_grid);
            robot_size_y_cells = static_cast<int>(r_y / y_grid);
            object_size_small_x_cells = static_cast<int>(os_x / x_grid);
            object_size_small_y_cells = static_cast<int>(os_y / y_grid);
            object_size_big_x_cells = static_cast<int>(ob_x / x_grid);
            object_size_big_y_cells = static_cast<int>(ob_y / y_grid);


            tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/path";
            std::string topic_name = ss_topic_name.str();

            publisher_path = this->create_publisher<geometry_msgs::msg::Polygon>(topic_name,10);

            client_cb_group = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            std::stringstream ss_topic_name_2;
            ss_topic_name_2 << "/robot_0" << robot_id << "/objective";
            std::string topic_name_2 = ss_topic_name_2.str();

            subs_objective = this->create_subscription<interfaces::msg::RobotObjective>(
                topic_name_2, 1, std::bind(&Compute_Trajectory::subs_obj_callback,this,_1));

            std::stringstream ss_service_name;
            ss_service_name << "/robot_0" << robot_id << "/a_star_server";
            std::string service_name = ss_service_name.str();

            client = this -> create_client<interfaces::srv::AStarService>(service_name, rclcpp::ServicesQoS(), client_cb_group);

            std::stringstream ss_topic_name_3;
            ss_topic_name_3 << "/robot_0" << robot_id << "/task_assigned";
            std::string topic_name_3 = ss_topic_name_3.str();
            subs_task_assigned = this->create_subscription<interfaces::msg::TaskDescription>(
                topic_name_3, 1, std::bind(&Compute_Trajectory::task_assigned_callback, this, _1));


            // Intentar suscribirse a los paths de robot_01 a robot_09 (excepto el propio)
            for (int i = 1; i <= 9; ++i) {
                if (i == robot_id) continue;
                std::stringstream ss_topic_name;
                ss_topic_name << "/robot_0" << i << "/path";
                std::string topic_name = ss_topic_name.str();
                try {
                    auto sub = this->create_subscription<geometry_msgs::msg::Polygon>(
                        topic_name, 1,
                        [this, i](const geometry_msgs::msg::Polygon::SharedPtr msg) {
                            if (other_robot_paths.size() <= i) other_robot_paths.resize(i+1);
                            if (other_robot_path_mats.size() <= i) other_robot_path_mats.resize(i+1);
                            other_robot_paths[i] = *msg;
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
                    RCLCPP_INFO(this->get_logger(), "Intentando suscribirse a: %s", topic_name.c_str());
                } catch (const std::exception &e) {
                    RCLCPP_WARN(this->get_logger(), "No se pudo suscribir a: %s", topic_name.c_str());
                }
            }

            timer_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            timer_ = this->create_wall_timer(
             500ms, std::bind(&Compute_Trajectory::timer_callback, this),timer_cb_group_);

           
        }


    private:
        int robot_size_x_cells;
        int robot_size_y_cells;
        int object_size_small_x_cells;
        int object_size_small_y_cells;
        int object_size_big_x_cells;
        int object_size_big_y_cells;

        rclcpp::Client<interfaces::srv::AStarService>::SharedPtr client;
        rclcpp::CallbackGroup::SharedPtr client_cb_group;
        rclcpp::Subscription<interfaces::msg::TaskDescription>::SharedPtr subs_task_assigned;
        rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr subs_path_leader;
        rclcpp::CallbackGroup::SharedPtr timer_cb_group_;


        void path_leader_callback(const geometry_msgs::msg::Polygon::SharedPtr path_msg)
        {   
            
            path_leader = *path_msg;
            //convert path_leader to cv::Mat
            // Create a blank cv::Mat to represent the leader's path
            path_leader_mat = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);


            // Iterate through the points in the path_leader
            for (const auto &point : path_leader.points)
            {
                // Convert the world coordinates to map coordinates
                int x_map = static_cast<int>((point.x * n_x_spaces) / x_world) + (n_x_spaces / 2);
                int y_map = n_y_spaces - (static_cast<int>((point.y * n_y_spaces) / y_world) + (n_y_spaces / 2));

                // Ensure the coordinates are within the bounds of the matrix
                if (x_map >= 0 && x_map < n_x_spaces && y_map >= 0 && y_map < n_y_spaces)
                {
                    // Mark the corresponding cell in the matrix
                    path_leader_mat.at<uchar>(y_map, x_map) = 255; // Use 255 to represent the path
                }
            }

            // Debug: Display the path_leader_mat
            cv::namedWindow("Path Leader", cv::WINDOW_NORMAL);
            cv::imshow("Path Leader", path_leader_mat);
            cv::waitKey(1);
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Received path from leader robot");
        }

        void update_subscription(){
            subs_path_leader.reset();

            std::stringstream ss_topic_name_subs;
            ss_topic_name_subs << "/robot_0" << leader_robot_id << "/path";
            std::string topic_name_subs = ss_topic_name_subs.str();
            subs_path_leader = this->create_subscription<geometry_msgs::msg::Polygon>(
                topic_name_subs, 1, std::bind(&Compute_Trajectory::path_leader_callback, this, _1));
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Update subscription to path from leader robot %d", leader_robot_id);
        }


        void task_assigned_callback(const interfaces::msg::TaskDescription::SharedPtr task_msg){
            leader_robot_id = task_msg->leader_robot_id;
            if(leader_robot_id!=robot_id){
                update_subscription();
            }
           
        }

        void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
            // n_objective = obj_msg->objective;
            // distance_objective = obj_msg->distance; 
            object_id = obj_msg->obj_id;
            angle_objective = obj_msg->angle;
            point_objective = obj_msg->point;
            robot_state = obj_msg->robot_state;

             RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Update objective");
        }

        void timer_callback()
        {
            

            
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Enter to callback");
            cv::Scalar white(255, 255, 255);
            map_color.setTo(white);   //reset the map
            //cv::namedWindow("Display_Map", cv::WINDOW_NORMAL );
            n_obstacles = 0; //Reset the number of obstacles
            obstacle_position.clear();
            angle_obstacle.clear();
            type_obstacle.clear();
            //For to save the position of every marker on the scene
            for (int marker=1; marker<30; marker++){
                std::stringstream ss_marker;
                if (marker<10){
                    ss_marker << "marker_id_0" << marker;
                }
                else{
                ss_marker << "marker_id_" << marker;
                }

                std::string marker_name = ss_marker.str();

                //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Object ID %d", object_id);

                
                    if(robot_id==marker){
                        //robot_position.x = transformStamped.transform.translation.x;
                        //robot_position.y = transformStamped.transform.translation.y;
                        
                        
                        std::stringstream ss_gripper;
                        ss_gripper << "robot_0" << robot_id << "/gr_ref_link";
                        std::string gripper_name = ss_gripper.str();


                        try{
                            geometry_msgs::msg::TransformStamped transformStamped_gripper = tf_buffer_->lookupTransform("marker_id_00", gripper_name, tf2::TimePointZero);
                            gripper_position.x = transformStamped_gripper.transform.translation.x;
                            gripper_position.y = transformStamped_gripper.transform.translation.y;
                        }
                        catch (tf2::TransformException &ex){
                            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
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
                            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }

                    }

                    else if(object_id==marker && robot_state >=2){
                        //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Robot state %d", robot_state);
                        //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Object detected ID %d", object_id);
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
                            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }
                    }
                    
                    else{
                        try{
                            geometry_msgs::msg::TransformStamped transformStamped = tf_buffer_->lookupTransform("marker_id_00", marker_name, tf2::TimePointZero);
                            //if(object_id!=marker){
                                n_obstacles++;
                                geometry_msgs::msg::Point obstacle_point;
                                obstacle_point.x = transformStamped.transform.translation.x;
                                obstacle_point.y = transformStamped.transform.translation.y;
                                obstacle_position.push_back(obstacle_point);
                                tf2::Quaternion quat(transformStamped.transform.rotation.x, transformStamped.transform.rotation.y, transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
                                tf2::Matrix3x3 m(quat);
                                double roll, pitch, yaw;
                                m.getRPY(roll,pitch,yaw);
                                angle_obstacle.push_back(yaw);
                                if(marker<10){
                                    type_obstacle.push_back(0);
                                }
                                else if(marker>10 && marker<20){
                                    type_obstacle.push_back(1);
                                }
                                else if(marker>20){
                                    type_obstacle.push_back(2);
                                }
                            //}
                        }
                        catch (tf2::TransformException &ex){
                            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }
                    }
                



            }

            cv::Mat map = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);
            cv::Mat map_bin = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);
            cv::Mat map_bin_ext = cv::Mat::zeros(n_y_spaces, n_x_spaces, CV_8UC1);
            

             map = cv::Scalar(255);
             map_bin = cv::Scalar(255);
             map_bin_ext = cv::Scalar(255);


            // //if (pos_msg->pos_robot1.orientation.x >= 0 && pos_msg->pos_robot1.orientation.y >=0 && pos_msg->pos_robot1.orientation.z >=0){
            // Robot1=pos_msg->pos_robot1;
            // //}
            // //if (pos_msg->pos_robot2.orientation.x >= 0 && pos_msg->pos_robot2.orientation.y >=0 && pos_msg->pos_robot2.orientation.z >=0){
            // Robot2=pos_msg->pos_robot2;
            // //}
            // Object1=pos_msg->pos_object1;
            // Object2=pos_msg->pos_object2;
            // Target=pos_msg->pos_target;




            //Draw Robot on map
            int Robot_x__grip_map = ((int)((gripper_position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int Robot_y__grip_map = n_y_spaces - (((int)((gripper_position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point Robot_grip_point(Robot_x__grip_map,Robot_y__grip_map); 
            Robot_grip_point_f = Robot_grip_point;

            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_x: %f" , gripper_position.x);
            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_y: %f" , gripper_position.y);
            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_x_map: %d" , Robot_x__grip_map);
            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_y_map: %d" , Robot_y__grip_map);


            double Robot_angle_degrees= (angle_robot*180)/PI * -1;

            cv::Point Robot_center_point((((int)((robot_position.x * n_x_spaces)/x_world))+(n_x_spaces/2)), (n_y_spaces - (((int)((robot_position.y * n_y_spaces)/y_world)) + (n_y_spaces/2))));  //Not necessary if obtain the base_link position
            Robot_center_point_f = Robot_center_point;

            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_x: %f" , robot_position.x);
            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_y: %f" , robot_position.y);
            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_x_map: %d" , Robot_center_point.x);
            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_y_map: %d" , Robot_center_point.y);

            cv::Size Robot_size(robot_size_x_cells, robot_size_y_cells);
            cv::RotatedRect Robot_rectangle(Robot_center_point, Robot_size, Robot_angle_degrees);
            cv::Point2f vertices2f_R[4];
            Robot_rectangle.points(vertices2f_R);

            std::vector<cv::Point> vertices_R;

            for(int i=0; i<4; i++){
                vertices_R.push_back(vertices2f_R[i]);
            }
            cv::fillConvexPoly(map,vertices_R, cv::Scalar(1));




            //Draw object on map
            int Object_x_map = ((int)((object_position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int Object_y_map = n_y_spaces - (((int)((object_position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point Object_point(Object_x_map,Object_y_map);

            

            double Object_angle_degrees= (angle_object*180)/PI * -1;
            if(type_object==1){
                cv::Size Object_size(object_size_small_x_cells, object_size_small_y_cells);
                cv::RotatedRect Object_rectangle(Object_point, Object_size, Object_angle_degrees);
                cv::Point2f vertices2f_Object[4];
                Object_rectangle.points(vertices2f_Object);

                std::vector<cv::Point> vertices_Object;

                for(int i=0; i<4; i++){
                    vertices_Object.push_back(vertices2f_Object[i]);
                }

                cv::fillConvexPoly(map,vertices_Object, cv::Scalar(1));
            }

            else if(type_object==2){
                cv::Size Object_size(object_size_big_x_cells, object_size_big_y_cells);
                cv::RotatedRect Object_rectangle(Object_point, Object_size, Object_angle_degrees);
                cv::Point2f vertices2f_Object[4];
                Object_rectangle.points(vertices2f_Object);

                std::vector<cv::Point> vertices_Object;

                for(int i=0; i<4; i++){
                    vertices_Object.push_back(vertices2f_Object[i]);
                }

                cv::fillConvexPoly(map,vertices_Object, cv::Scalar(1));
            }
            
            
            
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Nobs: %d" , n_obstacles);
            //Draw obstacles on map
            for (int i=0; i<n_obstacles; i++){
                int Obstacle_x_map = ((int)((obstacle_position[i].x * n_x_spaces)/x_world)) + (n_x_spaces/2);
                int Obstacle_y_map = n_y_spaces - (((int)((obstacle_position[i].y * n_y_spaces)/y_world)) + (n_y_spaces/2));
                cv::Point Obstacle_point(Obstacle_x_map,Obstacle_y_map);

                double Obstacle_angle_degrees= (angle_obstacle[i]*180)/PI * -1;
                if(type_obstacle[i]==0){
                    cv::Size Obstacle_size(robot_size_x_cells, robot_size_y_cells);
                    cv::RotatedRect Obstacle_rectangle(Obstacle_point, Obstacle_size, Obstacle_angle_degrees);
                    cv::Point2f vertices2f_Obstacle[4];
                    Obstacle_rectangle.points(vertices2f_Obstacle);

                    std::vector<cv::Point> vertices_Obstacle;

                    for(int j=0; j<4; j++){
                        vertices_Obstacle.push_back(vertices2f_Obstacle[j]);
                    }

                    cv::fillConvexPoly(map,vertices_Obstacle, cv::Scalar(4));
                    cv::fillConvexPoly(map_bin,vertices_Obstacle, cv::Scalar(0));
                }
                else if(type_obstacle[i]==1){
                    cv::Size Obstacle_size(object_size_small_x_cells, object_size_small_y_cells);
                    cv::RotatedRect Obstacle_rectangle(Obstacle_point, Obstacle_size, Obstacle_angle_degrees);
                    cv::Point2f vertices2f_Obstacle[4];
                    Obstacle_rectangle.points(vertices2f_Obstacle);

                    std::vector<cv::Point> vertices_Obstacle;

                    for(int j=0; j<4; j++){
                        vertices_Obstacle.push_back(vertices2f_Obstacle[j]);
                    }

                    cv::fillConvexPoly(map,vertices_Obstacle, cv::Scalar(4));
                    cv::fillConvexPoly(map_bin,vertices_Obstacle, cv::Scalar(0));
                }
                else if(type_obstacle[i]==2){
                        cv::Size Obstacle_size(object_size_big_x_cells, object_size_big_y_cells);
                        cv::RotatedRect Obstacle_rectangle(Obstacle_point, Obstacle_size, Obstacle_angle_degrees);
                        cv::Point2f vertices2f_Obstacle[4];
                        Obstacle_rectangle.points(vertices2f_Obstacle);

                        std::vector<cv::Point> vertices_Obstacle;

                        for(int j=0; j<4; j++){
                            vertices_Obstacle.push_back(vertices2f_Obstacle[j]);
                        }

                        cv::fillConvexPoly(map,vertices_Obstacle, cv::Scalar(4));
                        cv::fillConvexPoly(map_bin,vertices_Obstacle, cv::Scalar(0));

                    
                }
                

            }

            // Marcar los caminos de todos los otros robots como obstáculos
            for (const auto& mat : other_robot_path_mats) {
                if (mat.empty()) continue;
                for (int i = 0; i < mat.rows; i++) {
                    for (int j = 0; j < mat.cols; j++) {
                        if (mat.at<uchar>(i, j) == 255) {
                            map.at<uchar>(i, j) = 4; // Obstáculo por path de otro robot
                            map_bin.at<uchar>(i, j) = 0; // Celda bloqueada
                        }
                    }
                }
            }


            //cv::fillConvexPoly(map_bin,vertices_Tg, cv::Scalar(0));


            //cv::namedWindow("Display bin", cv::WINDOW_NORMAL );
            //cv::imshow("Display bin", map_bin);

            int goal_x = ((int)((point_objective.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int goal_y = n_y_spaces - (((int)((point_objective.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point goal(goal_x,goal_y);
            goal_f = goal;

            //goal_f.x = point_objective.x;
            //goal_f.y = point_objective.y;

            

            //call a_star_service
            auto request = std::make_shared<interfaces::srv::AStarService::Request>();
            request->src_x = Robot_grip_point_f.x;
            request->src_y = Robot_grip_point_f.y;
            request->dst_x = goal_f.x;
            request->dst_y = goal_f.y;

            

            std::vector<int> grid_vect(14400,1);


            int grid_index = 0;

            


            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_1");
            for (int i=0; i<n_y_spaces; i++){
                for (int j=0; j<n_x_spaces; j++){
                    if (map_bin.at<cv::uint8_t>(i,j)==0){
                        for (int k = -2; k < 3; k++){
                            for (int l= -2; l < 3; l++){
                                //std::cout << k << l << std::endl;

                                int i_k = i+k;
                                int j_l = j+l;

                                if ((i_k)>=0 && (i_k)<n_y_spaces && (j_l)>=0 && (j_l)<n_x_spaces){
                                   map_bin_ext.at<cv::uint8_t>(i+k,j+l)=cv::uint8_t(0);
                                 //  std::cout << i_k << j_l << std::endl;                                    
                                }
                            }
                        }
                        
                    }
                }
            }


            //cv::namedWindow("Display bin_ext", cv::WINDOW_NORMAL );
            //cv::imshow("Display bin_ext", map_bin_ext);
            //cv::waitKey(1);


            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_2");
            for (int i=0; i<n_y_spaces; i++){
                for (int j=0; j<n_x_spaces; j++){

                    if (map.at<cv::uint8_t>(i,j) == 1){
                        map_color.at<cv::Vec3b>(i,j) = cv::Vec3b(0,255,0);
                    }

                    if (map.at<cv::uint8_t>(i,j) == 2){
                        map_color.at<cv::Vec3b>(i,j) = cv::Vec3b(255,0,0);
                    }

                    if (map.at<cv::uint8_t>(i,j) == 3){
                        map_color.at<cv::Vec3b>(i,j) = cv::Vec3b(0,255,0);
                    }

                    if (map.at<cv::uint8_t>(i,j) == 4){
                        map_color.at<cv::Vec3b>(i,j) = cv::Vec3b(255,0,0);
                    }

                    if (map.at<cv::uint8_t>(i,j) == 5){
                        map_color.at<cv::Vec3b>(i,j) = cv::Vec3b(255,0,0);
                    }
                    
                   

                   if (map_bin_ext.at<cv::uint8_t>(i,j) == 0){

                    grid_vect[grid_index] = 0;

                    //std::cout<<"aqui";

                    

                   }
                   
                    grid_index++;
                    //std::cout <<grid_vect[grid_index];
                }
               // std::cout<<std::endl;
            }
            
            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_3");
            request->grid = grid_vect;

            while (!client->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service A_Star. Exiting.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service A_Star not available, waiting again...");
            }

            // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_4");

            auto handle_response =
                [this](rclcpp::Client<interfaces::srv::AStarService>::SharedFuture future) {
                    auto result = future.get();
                    // Handle the result

                    int path_size = result->path_size;

                    // if (path_size == 0){
                    //     RCLCPP_INFO(get_logger(), "No path found");
                    // }
                    // else{
                    //    RCLCPP_INFO(get_logger(), "Path found");
                        std::vector<int> path_x;
                        std::vector<int> path_y;


                        

                        path_x.resize(path_size);
                        path_y.resize(path_size);

     

                        path_x = result->path_x;
                        path_y = result->path_y;

                        //RCLCPP_INFO(get_logger(), "Checkpoint_5");

                        geometry_msgs::msg::Polygon path_msg;

                        for (int i=2; i<path_size; i++){
                            map_color.at<cv::Vec3b>(path_y[i], path_x[i]) = cv::Vec3b(0,0,255);
                            geometry_msgs::msg::Point32 point;
                            point.x = ((path_x[i]-(n_x_spaces/2))*x_world)/n_x_spaces;
                            point.y = ((path_y[i]-(n_y_spaces/2))*y_world)/n_y_spaces * -1;
                            path_msg.points.push_back(point);

                        }

                        int size_path = path_msg.points.size();
                        int size_path_ant = path_ant.points.size();

                        if (!path_msg.points.empty()){
                            if (size_path != size_path_ant){
                                publisher_path -> publish(path_msg);
                                //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Path send");
                            }
                        }
                        else {
                            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Path empty......");
                        }
                        
                        path_ant = path_msg;

                        ///////////////////////////////////////////////

                        cv::circle(map_color,Robot_grip_point_f,1,cv::Scalar(0,0,255),1);
                        cv::circle(map_color,Robot_center_point_f,2,cv::Scalar(255,0,0),1);
                        cv::circle(map_color,goal_f,1,cv::Scalar(255,0,0),1);

                        //cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
                        //cv::imshow("Display Image", map);

                        

                        // std::stringstream ss_image_name;
                        // ss_image_name << "map_robot_0" << robot_id << ".png";
                        // std::string image_name = ss_image_name.str();


                        // bool check_img = cv::imwrite(image_name, map_color);
                        // if(check_img==false){
                        //     RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Error saving image");
                        // }
                        
                        std::stringstream ss_window_name;
                        ss_window_name << "map_robot_0" << robot_id;
                        std::string window_name = ss_window_name.str();

                        cv::namedWindow(window_name, cv::WINDOW_NORMAL );

                        cv::imshow(window_name, map_color);
                        cv::waitKey(1);

                        // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_6");

                        
                   // }

                                        
                };

            auto future = client->async_send_request(request, handle_response);

        }



    rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;
    
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Publisher<geometry_msgs::msg::Polygon>::SharedPtr publisher_path;
    rclcpp::TimerBase::SharedPtr timer_;
};


// class Node_Trajectory_Robot_State_Check : public rclcpp::Node
// {
//     public:
//         Node_Trajectory_Robot_State_Check() : Node("node_trajectory_robot_state_check")
//         {

//             std::stringstream ss_topic_name_5;
//             ss_topic_name_5 << "robot_0" << robot_id << "/robot_state";
//             std::string topic_name_5 = ss_topic_name_5.str();
//             subs_robot_state = this->create_subscription<interfaces::msg::RobotState>(
//                 topic_name_5, 1, std::bind(&Node_Trajectory_Robot_State_Check::robot_state_callback, this, _1));
            
//         }


//     private:

//         rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subs_robot_state;

//         void robot_state_callback(const interfaces::msg::RobotState::SharedPtr state_msg){
//             robot_state = state_msg->robot_state;
//             RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "State of robot %d is %d", robot_id, robot_state);
//         }
        

// };



int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    //auto node = std::make_shared<Compute_Trajectory>();
    //auto node_trajectory_robot_state_check = std::make_shared<Node_Trajectory_Robot_State_Check>();

    // rclcpp::executors::MultiThreadedExecutor executor;
    // executor.add_node(node);
    // //executor.add_node(node_trajectory_robot_state_check);
    // executor.spin();
    rclcpp::spin(std::make_shared<Compute_Trajectory>());
    rclcpp::shutdown();
    return 0;
}