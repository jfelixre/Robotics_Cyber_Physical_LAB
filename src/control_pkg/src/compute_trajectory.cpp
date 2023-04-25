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
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <geometry_msgs/msg/pose.hpp>
#include <interfaces/msg/positions.hpp>
#include <interfaces/srv/a_star_service.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;
# define PI 3.14159265358979323846

geometry_msgs::msg::Pose Robot1, Robot2, Object1, Object2, Target;


float x_grid = 0.025;    //All dimensions in meters
float y_grid = 0.025;
float x_world = 3;        
float y_world = 3;          

int n_x_spaces = (int)x_world/x_grid;
float n_y_spaces = (int)y_world/y_grid;




class Compute_Trajectory : public rclcpp::Node
{
    public:
        Compute_Trajectory() : Node("compute_trajectory")
        {
           

            subs_position = this->create_subscription<interfaces::msg::Positions>(
                "/positions", 1, std::bind(&Compute_Trajectory::subs_callback,this,_1));

            client = this -> create_client<interfaces::srv::AStarService>("a_star_server");

            //std::cout<<n_x_spaces<< std::endl;
        }
    

    private:

        rclcpp::Client<interfaces::srv::AStarService>::SharedPtr client;


        void subs_callback(const interfaces::msg::Positions::SharedPtr pos_msg)
        {

            cv::Mat map = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);
            cv::Mat map_bin = cv::Mat::zeros(n_x_spaces, n_y_spaces, CV_8UC1);

             map = cv::Scalar(255);
             map_bin = cv::Scalar(1);


            Robot1=pos_msg->pos_robot1;
            Robot2=pos_msg->pos_robot2;
            Object1=pos_msg->pos_object1;
            Object2=pos_msg->pos_object2;
            Target=pos_msg->pos_target;




            //ROBOT 1
            int R1_x_map = ((int)((Robot1.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int R1_y_map = 120 - (((int)((Robot1.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point R1_point(R1_x_map,R1_y_map); 
            
            tf2::Quaternion R1_quat(Robot1.orientation.x, Robot1.orientation.y, Robot1.orientation.z, Robot1.orientation.w);

            tf2::Matrix3x3 R1_m(R1_quat);

            double R1_orientation_x, R1_orientation_y, R1_orientation_z;

            R1_m.getRPY(R1_orientation_x, R1_orientation_y, R1_orientation_z);
            double R1_angle_degrees= (R1_orientation_z*180)/PI * -1;

            cv::Point R1_center_point(((int)((5*cos(R1_angle_degrees))+R1_x_map)),((int)((4*(sin(R1_angle_degrees)))+R1_y_map)));

            cv::Size R1_size(12,14);
            cv::RotatedRect R1_rectangle(R1_center_point, R1_size, R1_angle_degrees);
            cv::Point2f vertices2f_R1[4];
            R1_rectangle.points(vertices2f_R1);

            std::vector<cv::Point> vertices_R1;

            for(int i=0; i<4; i++){
                vertices_R1.push_back(vertices2f_R1[i]);
            }

            cv::fillConvexPoly(map,vertices_R1, cv::Scalar(1));



            //ROBOT 2
            int R2_x_map = ((int)((Robot2.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int R2_y_map = 120 - (((int)((Robot2.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point R2_point(R2_x_map,R2_y_map);
            
            tf2::Quaternion R2_quat(Robot2.orientation.x, Robot2.orientation.y, Robot2.orientation.z, Robot2.orientation.w);

            tf2::Matrix3x3 R2_m(R2_quat);

            double R2_orientation_x, R2_orientation_y, R2_orientation_z;

            R2_m.getRPY(R2_orientation_x, R2_orientation_y, R2_orientation_z);
            double R2_angle_degrees= (R2_orientation_z*180)/PI * -1;

            cv::Point R2_center_point(((int)((5*cos(R2_angle_degrees))+R2_x_map)),((int)((4*(sin(R2_angle_degrees)))+R2_y_map)));


            cv::Size R2_size(12,14);
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

            int O1_x_map = ((int)((Object1.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
            int O1_y_map = 120 - (((int)((Object1.position.y * n_y_spaces)/y_world)) + (n_y_spaces/2));
            cv::Point O1_point(O1_x_map,O1_y_map);
            
            tf2::Quaternion O1_quat(Object1.orientation.x, Object1.orientation.y, Object1.orientation.z, Object1.orientation.w);

            tf2::Matrix3x3 O1_m(O1_quat);

            double O1_orientation_x, O1_orientation_y, O1_orientation_z;

            O1_m.getRPY(O1_orientation_x, O1_orientation_y, O1_orientation_z);
            double O1_angle_degrees= (O1_orientation_z*180)/PI * -1;
            cv::Size O1_size(4,4);
            cv::RotatedRect O1_rectangle(O1_point, O1_size, O1_angle_degrees);
            cv::Point2f vertices2f_O1[4];
            O1_rectangle.points(vertices2f_O1);

            std::vector<cv::Point> vertices_O1;

            for(int i=0; i<4; i++){
                vertices_O1.push_back(vertices2f_O1[i]);
            }

            cv::fillConvexPoly(map,vertices_O1, cv::Scalar(3));






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

            int Tg_x_map = ((int)((Target.position.x * n_x_spaces)/x_world)) + (n_x_spaces/2);
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
            cv::fillConvexPoly(map_bin,vertices_Tg, cv::Scalar(0));




            //call a_star_service
            auto request = std::make_shared<interfaces::srv::AStarService::Request>();
            request->src_x = R1_center_point.x;
            request->src_y = R1_center_point.y;
            request->dst_x = O1_point.x;
            request->dst_y = O1_point.y;
            
            std::vector<int> grid_vect;

            
            int grid_index = 0;
            for (int i=0; i<120; i++){
                for (int j=0; j<120; j++){
                   // grid_vect[grid_index] = map_bin.at<int32_t>(i,j);
                   std::cout << map_bin.at<u_int8_t>(i,j) << std::endl;
                }
            }

           // std::cout << grid_vect[5] << std::endl;



            /*
            request->grid = grid_vect;

            while (!client->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service A_Star. Exiting.");
                }
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service A_Star not available, waiting again...");
            }

            auto result = client->async_send_request(request);

            if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result) ==
                rclcpp::FutureReturnCode::SUCCESS){
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "A_Star complete");
                }
                else{
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service A_Star");
                }
            ///////////////////////////////////////////////
*/
            cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
            cv::imshow("Display Image", map);
            cv::namedWindow("Display bin", cv::WINDOW_NORMAL );
            cv::imshow("Display bin", map_bin);
            cv::waitKey(1);

        }







    rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subs_position;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Compute_Trajectory>());
    rclcpp::shutdown();
    return 0;
}