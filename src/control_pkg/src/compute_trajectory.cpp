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



float x_grid = 0.05;    //All dimensions in meters
float y_grid = 0.05;
float x_world = 6;        
float y_world = 6;          

int n_x_spaces = (int)x_world/x_grid;
int n_y_spaces = (int)y_world/y_grid;


geometry_msgs::msg::Polygon path_ant;

cv::Mat map_color(n_x_spaces, n_y_spaces, CV_8UC3, cv::Scalar(255, 255, 255));
cv::Point goal_f;
cv::Point Robot_grip_point_f;
cv::Point Robot_center_point_f;

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

            timer_ = this->create_wall_timer(
             500ms, std::bind(&Compute_Trajectory::timer_callback, this));
        }


    private:

        rclcpp::Client<interfaces::srv::AStarService>::SharedPtr client;
        rclcpp::CallbackGroup::SharedPtr client_cb_group;

        void subs_obj_callback(const interfaces::msg::RobotObjective::SharedPtr obj_msg){
            // n_objective = obj_msg->objective;
            // distance_objective = obj_msg->distance; 
            object_id = obj_msg->obj_id;
            angle_objective = obj_msg->angle;
            point_objective = obj_msg->point;

             RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Update objective");
        }

        void timer_callback()
        {      
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

                    else if(object_id==marker){
                        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Object detected ID %d", object_id);
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
                            if(object_id!=marker){
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
                            }
                        }
                        catch (tf2::TransformException &ex){
                            //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }
                    }
                



            }

            cv::Mat map = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_bin = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_bin_ext = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            

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

            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_x: %f" , gripper_position.x);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_y: %f" , gripper_position.y);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_x_map: %d" , Robot_x__grip_map);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Grip_y_map: %d" , Robot_y__grip_map);


            double Robot_angle_degrees= (angle_robot*180)/PI * -1;

            cv::Point Robot_center_point((((int)((robot_position.x * n_x_spaces)/x_world))+(n_x_spaces/2)), (n_y_spaces - (((int)((robot_position.y * n_y_spaces)/y_world)) + (n_y_spaces/2))));  //Not necessary if obtain the base_link position
            Robot_center_point_f = Robot_center_point;

            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_x: %f" , robot_position.x);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_y: %f" , robot_position.y);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_x_map: %d" , Robot_center_point.x);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Center_y_map: %d" , Robot_center_point.y);

            cv::Size Robot_size(14,12);
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
                cv::Size Object_size(4,4);
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
                cv::Size Object_size(12,4);
                cv::RotatedRect Object_rectangle(Object_point, Object_size, Object_angle_degrees);
                cv::Point2f vertices2f_Object[4];
                Object_rectangle.points(vertices2f_Object);

                std::vector<cv::Point> vertices_Object;

                for(int i=0; i<4; i++){
                    vertices_Object.push_back(vertices2f_Object[i]);
                }

                cv::fillConvexPoly(map,vertices_Object, cv::Scalar(1));
            }
            
            
            
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Nobs: %d" , n_obstacles);
            //Draw obstacles on map
            for (int i=0; i<n_obstacles; i++){
                int Obstacle_x_map = ((int)((obstacle_position[i].x * n_x_spaces)/x_world)) + (n_x_spaces/2);
                int Obstacle_y_map = n_y_spaces - (((int)((obstacle_position[i].y * n_y_spaces)/y_world)) + (n_y_spaces/2));
                cv::Point Obstacle_point(Obstacle_x_map,Obstacle_y_map);

                double Obstacle_angle_degrees= (angle_obstacle[i]*180)/PI * -1;
                if(type_obstacle[i]==0){
                    cv::Size Obstacle_size(14,12);
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
                    cv::Size Obstacle_size(4,4);
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
                    cv::Size Obstacle_size(12,4);
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
            for (int i=0; i<n_x_spaces; i++){
                for (int j=0; j<n_y_spaces; j++){
                    if (map_bin.at<cv::uint8_t>(i,j)==0){
                        for (int k = -2; k < 3; k++){
                            for (int l= -2; l < 3; l++){
                                //std::cout << k << l << std::endl;

                                int i_k = i+k;
                                int j_l = j+l;

                                if ((i_k)>=0 && (i_k)<n_x_spaces && (j_l)>=0 && (j_l)<n_y_spaces){
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
            for (int i=0; i<n_x_spaces; i++){
                for (int j=0; j<n_y_spaces; j++){

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

     

                        path_x = result->path_y;
                        path_y = result->path_x;

                        //RCLCPP_INFO(get_logger(), "Checkpoint_5");

                        geometry_msgs::msg::Polygon path_msg;

                        for (int i=2; i<path_size; i++){
                            map_color.at<cv::Vec3b>(path_x[i], path_y[i]) = cv::Vec3b(0,0,255);
                            geometry_msgs::msg::Point32 point;
                            point.y = ((path_x[i]-(n_x_spaces/2))*x_world)/n_x_spaces * -1;
                            point.x = ((path_y[i]-(n_y_spaces/2))*y_world)/n_y_spaces;
                            path_msg.points.push_back(point);

                        }

                        int size_path = path_msg.points.size();
                        int size_path_ant = path_ant.points.size();

                        if (!path_msg.points.empty()){
                            if (size_path != size_path_ant){
                                publisher_path -> publish(path_msg);
                                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Path send");
                            }
                        }
                        else {
                            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Path empty......");
                        }
                        
                        path_ant = path_msg;

                        ///////////////////////////////////////////////

                        cv::circle(map_color,Robot_grip_point_f,1,cv::Scalar(0,0,255),1);
                        cv::circle(map_color,Robot_center_point_f,2,cv::Scalar(255,0,0),1);
                        cv::circle(map_color,goal_f,1,cv::Scalar(255,0,0),1);

                        //cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
                        //cv::imshow("Display Image", map);

                        //cv::namedWindow("MAP_R1", cv::WINDOW_NORMAL );

                        bool check_img = cv::imwrite("map.png", map_color);
                        if(check_img==false){
                            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Error saving image");
                        }
                        //cv::imshow("Display_Map", map_color);
                        //cv::waitKey(1);
                        // RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_6");

                        
                   // }

                                        
                };

            auto future = client->async_send_request(request, handle_response);

             //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Checkpoint_7");

            // cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
            // cv::imshow("Display Image", map);
            // cv::waitKey(1);

                
            
            



















          //   std::cout << "enviar request" << std::endl;

//             auto result = client->async_send_request(request);

// /*              

//             if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result) ==
//                 rclcpp::FutureReturnCode::SUCCESS){
//                     RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "A_Star complete");
//                 }
//                 else{
//                     RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service A_Star");
//                 }
// */  
//             auto status = result.wait_for(3s);  //not spinning here!
//             if (status == std::future_status::ready)
//             {   
//                 int path_size = result.get()->path_size;
//                 RCLCPP_INFO(get_logger(), "Result received %d", result.get()->path_size);
                
//                 RCLCPP_INFO(get_logger(), "Checkpoint");

//                 if (result.get()->path_size == 0){
//                     RCLCPP_INFO(get_logger(), "No path found");
//                 }
//                 else{
//                     RCLCPP_INFO(get_logger(), "Path found");
//                     std::vector<int> path_x;
//                     std::vector<int> path_y;


                    

//                     path_x.resize(path_size);
//                     path_y.resize(path_size);

//                 // std::cout << "rezise" << std::endl;


//                     path_x = result.get()->path_y;
//                     path_y = result.get()->path_x;

//                     RCLCPP_INFO(get_logger(), "Checkpoint_2");
//                     //int path_size = path_x.size();
//         /*
//                     for (int i=0; i<path_x.size(); i++){
//                         std::cout << "x = " << path_x[i] << std::endl;
//                         std::cout << "y = " << path_y[i] << std::endl;
//                     }
//         */
//                     geometry_msgs::msg::Polygon path_msg;

//                     for (int i=2; i<path_size; i++){
//                         map_color.at<cv::Vec3b>(path_x[i], path_y[i]) = cv::Vec3b(0,0,255);
//                         geometry_msgs::msg::Point32 point;
//                         point.y = ((path_x[i]-(n_x_spaces/2))*x_world)/n_x_spaces * -1;
//                         point.x = ((path_y[i]-(n_y_spaces/2))*y_world)/n_y_spaces;
//                         path_msg.points.push_back(point);

//                     }

//                     int size_path = path_msg.points.size();
//                     int size_path_ant = path_ant.points.size();

//                     if (!path_msg.points.empty()){
//                         if (size_path != size_path_ant){
//                             publisher_path -> publish(path_msg);
//                             RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Path send");
//                         }
//                     }
//                     else {
//                         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Path empty......");
//                     }
                    
//                     path_ant = path_msg;

//                     ///////////////////////////////////////////////

//                     cv::circle(map_color,Robot_point,1,cv::Scalar(0,0,255),1);
//                     cv::circle(map_color,Robot_center_point,1,cv::Scalar(255,0,0),1);
//                     cv::circle(map_color,goal_f,1,cv::Scalar(255,0,0),1);

//                     //cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
//                     //cv::imshow("Display Image", map);

//                     cv::namedWindow("MAP_R1", cv::WINDOW_NORMAL );
//                     cv::imshow("MAP_R1", map_color);
//                     cv::waitKey(1);
//                 }

                
            
//             }
//             else
//             {
//             RCLCPP_ERROR(get_logger(), "Not received");

//             }
         //    std::cout << "esperar" << std::endl;
            //result.wait();
           // while( result.get()->path_x.empty()){
            //   std::cout << "esperainterfaces_for(std::chrono::milliseconds(1000));


           // std::cout << "termina espera" << std::endl;


        }



    rclcpp::Subscription<interfaces::msg::RobotObjective>::SharedPtr subs_objective;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Publisher<geometry_msgs::msg::Polygon>::SharedPtr publisher_path;
    rclcpp::TimerBase::SharedPtr timer_;
};

/*
class Node_Client_A_Star : public rclcpp::Node
{
    public:
        Node_Client_A_Star() : Node("node_client_a_star")
        {


            client = this -> create_client<interfaces::srv::AStarService>("a_star_server");

            //std::cout<<n_x_spaces<< std::endl;
        }


    private:

            
        

};
*/


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Compute_Trajectory>();
    //auto node_client_a_star = std::make_shared<Node_Client_A_Star>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    //executor.add_node(node_client_a_star);
    executor.spin();
    //rclcpp::spin(std::make_shared<Compute_Trajectory>());
    rclcpp::shutdown();
    return 0;
}