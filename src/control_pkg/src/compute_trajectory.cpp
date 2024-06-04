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
std::vector<geometry_msgs::msg::Point> obstacle_position;
std::vector<float> angle_obstacle;
int n_obstacles = 0;



float x_grid = 0.025;    //All dimensions in meters
float y_grid = 0.025;
float x_world = 3;        
float y_world = 3;          

int n_x_spaces = (int)x_world/x_grid;
float n_y_spaces = (int)y_world/y_grid;

geometry_msgs::msg::Polygon path_ant;

//rclcpp::Client<interfaces::srv::AStarService>::SharedPtr client;



class Compute_Trajectory : public rclcpp::Node
{
    public:
        Compute_Trajectory() : Node("compute_trajectory")
        {

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

            client = this -> create_client<interfaces::srv::AStarService>("a_star_server", rmw_qos_profile_services_default, client_cb_group);

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
            n_obstacles = 0; //Reset the number of obstacles
            obstacle_position.clear();
            angle_obstacle.clear();
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

                try{
                    geometry_msgs::msg::TransformStamped transformStamped = tf_buffer_->lookupTransform("marker_id_00", marker_name, tf2::TimePointZero);
                    if(robot_id==marker){
                        robot_position.x = transformStamped.transform.translation.x;
                        robot_position.y = transformStamped.transform.translation.y;
                        tf2::Quaternion quat(transformStamped.transform.rotation.x, transformStamped.transform.rotation.y, transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
                        tf2::Matrix3x3 m(quat);
                        double roll, pitch, yaw;
                        m.getRPY(roll,pitch,yaw);
                        angle_robot = yaw;
                        
                        std::stringstream ss_gripper;
                        ss_gripper << "robot_id_0" << robot_id << "/gr_ref_link";
                        std::string gripper_name = ss_gripper.str();

                        try{
                            geometry_msgs::msg::TransformStamped transformStamped_gripper = tf_buffer_->lookupTransform("marker_id_00", gripper_name, tf2::TimePointZero);
                            gripper_position.x = transformStamped_gripper.transform.translation.x;
                            gripper_position.y = transformStamped_gripper.transform.translation.y;
                        }
                        catch (tf2::TransformException &ex){
                            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                            continue;
                        }

                    }
                    else if(object_id==marker){
                        object_position.x = transformStamped.transform.translation.x;
                        object_position.y = transformStamped.transform.translation.y;
                        tf2::Quaternion quat(transformStamped.transform.rotation.x, transformStamped.transform.rotation.y, transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
                        tf2::Matrix3x3 m(quat);
                        double roll, pitch, yaw;
                        m.getRPY(roll,pitch,yaw);
                        angle_object = yaw;
                    }
                    
                    else{
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
                    }
                }
                catch (tf2::TransformException &ex){
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", ex.what());
                    continue;
                }



            }

            cv::Mat map = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_bin = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_bin_ext = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_color(120, 120, CV_8UC3, cv::Scalar(255, 255, 255));

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
            int Robot_x_map = ((int)((Robot1.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int Robot_y_map = 120 - (((int)((Robot1.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point Robot_point(Robot_x_map,Robot_y_map); 

            double Robot_angle_degrees= (angle_robot*180)/PI * -1;

            cv::Point Robot_center_point(((int)((5*cos(angle_robot))+Robot_x_map)),((int)((-5*(sin(angle_robot)))+Robot_y_map)));

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
            int Object_y_map = 120 - (((int)((object_position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point Object_point(Object_x_map,Object_y_map);

            double Object_angle_degrees= (angle_object*180)/PI * -1;
            cv::Size Object_size(4,4);
            cv::RotatedRect Object_rectangle(Object_point, Object_size, Object_angle_degrees);
            cv::Point2f vertices2f_Object[4];
            Object_rectangle.points(vertices2f_Object);

            std::vector<cv::Point> vertices_Object;

            for(int i=0; i<4; i++){
                vertices_Object.push_back(vertices2f_Object[i]);
            }

            cv::fillConvexPoly(map,vertices_Object, cv::Scalar(3));
            



            //ROBOT 2
            int R2_x_map = ((int)((Robot2.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int R2_y_map = 120 - (((int)((Robot2.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point R2_point(R2_x_map,R2_y_map);

            tf2::Quaternion R2_quat(Robot2.orientation.x, Robot2.orientation.y, Robot2.orientation.z, Robot2.orientation.w);

            tf2::Matrix3x3 R2_m(R2_quat);

            double R2_orientation_x, R2_orientation_y, R2_orientation_z;

            R2_m.getRPY(R2_orientation_x, R2_orientation_y, R2_orientation_z);
            double R2_angle_degrees= (R2_orientation_z*180)/PI * -1;

            cv::Point R2_center_point(((int)((5*cos(R2_orientation_z))+R2_x_map)),((int)((-5*(sin(R2_orientation_z)))+R2_y_map)));


            cv::Size R2_size(14,12);
            cv::RotatedRect R2_rectangle(R2_center_point, R2_size, R2_angle_degrees);
            cv::Point2f vertices2f_R2[4];
            R2_rectangle.points(vertices2f_R2);

            std::vector<cv::Point> vertices_R2;

            for(int i=0; i<4; i++){
                vertices_R2.push_back(vertices2f_R2[i]);
            }

            cv::fillConvexPoly(map,vertices_R2, cv::Scalar(2));
            cv::fillConvexPoly(map_bin,vertices_R2, cv::Scalar(0));



            //OBJECT 1

            int Object_x_map = ((int)((Object1.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int Object_y_map = 120 - (((int)((Object1.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point Object_point(Object_x_map,Object_y_map);

            tf2::Quaternion Object_quat(Object1.orientation.x, Object1.orientation.y, Object1.orientation.z, Object1.orientation.w);

            tf2::Matrix3x3 Object_m(Object_quat);

            double Object_orientation_x, Object_orientation_y, Object_orientation_z;

            Object_m.getRPY(Object_orientation_x, Object_orientation_y, Object_orientation_z);
            double Object_angle_degrees= (Object_orientation_z*180)/PI * -1;
            cv::Size Object_size(4,4);
            cv::RotatedRect Object_rectangle(Object_point, Object_size, Object_angle_degrees);
            cv::Point2f vertices2f_Object[4];
            Object_rectangle.points(vertices2f_Object);

            std::vector<cv::Point> vertices_Object;

            for(int i=0; i<4; i++){
                vertices_Object.push_back(vertices2f_Object[i]);
            }

            cv::fillConvexPoly(map,vertices_Object, cv::Scalar(3));






            //OBJECT 2

            int O2_x_map = ((int)((Object2.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int O2_y_map = 120 - (((int)((Object2.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point O2_point(O2_x_map,O2_y_map);

            tf2::Quaternion O2_quat(Object2.orientation.x, Object2.orientation.y, Object2.orientation.z, Object2.orientation.w);

            tf2::Matrix3x3 O2_m(O2_quat);

            double O2_orientation_x, O2_orientation_y, O2_orientation_z;

            O2_m.getRPY(O2_orientation_x, O2_orientation_y, O2_orientation_z);
            double O2_angle_degrees= (O2_orientation_z*180)/PI * -1;
            cv::Size O2_size(4,4);
            cv::RotatedRect O2_rectangle(O2_point, O2_size, O2_angle_degrees);
            cv::Point2f vertices2f_O2[4];
            O2_rectangle.points(vertices2f_O2);

            std::vector<cv::Point> vertices_O2;

            for(int i=0; i<4; i++){
                vertices_O2.push_back(vertices2f_O2[i]);
            }

            cv::fillConvexPoly(map,vertices_O2, cv::Scalar(4));
            cv::fillConvexPoly(map_bin,vertices_O2, cv::Scalar(0));





            //TARGET

            int Tg_x_map = ((int)((Target.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2) -2;
            int Tg_y_map = 120 - (((int)((Target.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point Tg_point(Tg_x_map,Tg_y_map);

            tf2::Quaternion Tg_quat(Target.orientation.x, Target.orientation.y, Target.orientation.z, Target.orientation.w);

            tf2::Matrix3x3 Tg_m(Tg_quat);

            double Tg_orientation_x, Tg_orientation_y, Tg_orientation_z;

            Tg_m.getRPY(Tg_orientation_x, Tg_orientation_y, Tg_orientation_z);
            double Tg_angle_degrees= (Tg_orientation_z*180)/PI * -1;
            cv::Size Tg_size(5,5);
            cv::RotatedRect Tg_rectangle(Tg_point, Tg_size, Tg_angle_degrees);
            cv::Point2f vertices2f_Tg[4];
            Tg_rectangle.points(vertices2f_Tg);

            std::vector<cv::Point> vertices_Tg;

            for(int i=0; i<4; i++){
                vertices_Tg.push_back(vertices2f_Tg[i]);
            }

            cv::fillConvexPoly(map,vertices_Tg, cv::Scalar(5));
            //cv::fillConvexPoly(map_bin,vertices_Tg, cv::Scalar(0));


            //cv::namedWindow("Display bin", cv::WINDOW_NORMAL );
            //cv::imshow("Display bin", map_bin);


            cv::Point goal;

            switch (n_objective)
            {
            case -1:
                Initial.position.x = R1_center_point.x;
                Initial.position.y = R1_center_point.y;
                goal.x = Initial.position.x;
                goal.y = Initial.position.y;
                break;

            case 0:
                goal.x = Initial.position.x;
                goal.y = Initial.position.y;
                break;

            case 1:
                goal.x = Object_point.x;// + (distance_objective * sin(Object_orientation_z));
                goal.y = Object_point.y + (distance_objective * cos(Object_orientation_z));
                Saved.position.x = Object_point.x;
                Saved.position.y = Object_point.y;
                //Saved.orientation.z = Object_orientation_z;
                break;

            case 2:
                goal.x = Tg_point.x + (distance_objective * sin(Tg_orientation_z));
                goal.y = Tg_point.y + (distance_objective * cos(Tg_orientation_z));
                break;    

            case 3:
                goal.x = Saved.position.x;
                goal.y = Saved.position.y + (distance_objective * sin(M_PI_2));
                break;            
            
            default:
                break;
            }

            //std::cout<<"nobj= " << n_objective << std::endl;
            //std::cout<<"goalx = " << goal.x << " goaly = " << goal.y << std::endl;




            //call a_star_service
            auto request = std::make_shared<interfaces::srv::AStarService::Request>();
            request->src_x = R1_center_point.x;
            request->src_y = R1_center_point.y;
            request->dst_x = goal.x;
            request->dst_y = goal.y;

            std::vector<int> grid_vect(14400,1);


            int grid_index = 0;

            



            for (int i=0; i<120; i++){
                for (int j=0; j<120; j++){
                    if (map_bin.at<cv::uint8_t>(i,j)==0){
                        for (int k = -2; k < 3; k++){
                            for (int l= -2; l < 3; l++){
                                //std::cout << k << l << std::endl;

                                int i_k = i+k;
                                int j_l = j+l;

                                if ((i_k)>=0 && (i_k)<120 && (j_l)>=0 && (j_l)<120){
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



            for (int i=0; i<120; i++){
                for (int j=0; j<120; j++){

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





            request->grid = grid_vect;

            while (!client->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service A_Star. Exiting.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service A_Star not available, waiting again...");
            }


          //   std::cout << "enviar request" << std::endl;

            auto result = client->async_send_request(request);
/*
            if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result) ==
                rclcpp::FutureReturnCode::SUCCESS){
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "A_Star complete");
                }
                else{
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service A_Star");
                }
*/

         //    std::cout << "esperar" << std::endl;
            result.wait();
           // while( result.get()->path_x.empty()){
            //   std::cout << "esperainterfaces_for(std::chrono::milliseconds(1000));


           // std::cout << "termina espera" << std::endl;

            std::vector<int> path_x;
            std::vector<int> path_y;


            int path_size = result.get()->path_size;

            path_x.resize(path_size);
            path_y.resize(path_size);

           // std::cout << "rezise" << std::endl;


            path_x = result.get()->path_y;
            path_y = result.get()->path_x;


            //int path_size = path_x.size();
/*
            for (int i=0; i<path_x.size(); i++){
                std::cout << "x = " << path_x[i] << std::endl;
                std::cout << "y = " << path_y[i] << std::endl;
            }
*/
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

            cv::circle(map_color,R1_point,1,cv::Scalar(0,0,255),1);
            cv::circle(map_color,R1_center_point,1,cv::Scalar(255,0,0),1);
            cv::circle(map_color,goal,1,cv::Scalar(255,0,0),1);

            //cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
            //cv::imshow("Display Image", map);

            cv::namedWindow("MAP_R1", cv::WINDOW_NORMAL );
            cv::imshow("MAP_R1", map_color);
            cv::waitKey(1);

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