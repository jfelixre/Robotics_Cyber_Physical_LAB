#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <stdio.h>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/image_encodings.hpp>
//#include <std_msgs/msg/bool.hpp>
//#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <interfaces/msg/img_data.hpp>

using std::placeholders::_1;


using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;
namespace enc = sensor_msgs::image_encodings;

cv::Mat img_original;
cv::Mat img_mod;


int RHmin=105;
int RSmin=205;
int RVmin=197;
int RHmax=127;
int RSmax=255;
int RVmax=255;

int BKHmin=0;
int BKSmin=0;
int BKVmin=0;
int BKHmax=0;
int BKSmax=0;
int BKVmax=0;

int GHmin=28;
int GSmin=205;
int GVmin=197;
int GHmax=76;
int GSmax=255;
int GVmax=255;

int BHmin=0;
int BSmin=205;
int BVmin=197;
int BHmax=27;
int BSmax=255;
int BVmax=255;

int R1x=0;
int R1y=0;
float R1ang=0;
int R2x=0;
int R2y=0;
float R2ang=0;

int Obj1x=0;
int Obj1y=0;
float Obj1ang=0;
int Obj2x=0;
int Obj2y=0;
float Obj2ang=0;

int Tgt1x=0;
int Tgt1y=0;
float Tgt1ang=0;
int Tgt2x=0;
int Tgt2y=0;
float Tgt2ang=0;



class Img_Proc_Id_Node : public rclcpp::Node
{
  public:
    Img_Proc_Id_Node() : Node("img_proc_id_node")
    {
      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "demo_cam/camera1/image_raw", 10, std::bind(&Img_Proc_Id_Node::topic_callback, this, _1));
      //topic "imagen_original" para img_publ_test_node
      //topic "demo_cam/camera1/image_raw" para gazebo camera_test.world
      publisher = this->create_publisher<interfaces::msg::ImgData>("image_data",1);

    }

  private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg) const
    {
      RCLCPP_INFO(this->get_logger(), "Received image" );
      
      cv_bridge::CvImageConstPtr image_bridge;
	    //cv_bridge::toCvCopy(msg, RGB8)->image;
      image_bridge=cv_bridge::toCvCopy(msg);
      img_original = image_bridge->image;
      cv::cvtColor(img_original,img_original,cv::COLOR_BGR2RGB);
      //cv::imwrite("image.jpg",img_original);
      int n_cols = img_original.cols;
      int n_rows = img_original.rows;

      img_mod=img_original.clone();
      cv::Mat img_hsv;

      cv::cvtColor(img_original,img_hsv, cv::COLOR_RGB2HSV);     //Convert to HSV
      
      //-------------------------------Filtro Rojo para robots-------------------------------------------------------
      cv::Mat img_red_bin;
      
      cv::inRange(img_hsv, cv::Scalar(RHmin,RSmin,RVmin), cv::Scalar(RHmax,RSmax,RVmax), img_red_bin);  //HSV filter

      std::vector<std::vector<cv::Point>> red_contours;
      std::vector<cv::Vec4i> hierarchy;
      cv::findContours(img_red_bin,red_contours,hierarchy,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

      //Esto debe de ir dentro del for de contours

      for (int i=0; i<red_contours.size(); i++)
      { 
        cv::Moments moments;  //Moments para calcular el centro de la figura
        int X, Y;

        moments = cv::moments(red_contours[i]);
        X = moments.m10 / moments.m00;
        Y = moments.m01 / moments.m00;

        

        cv::Mat img_red_crop;
        cv::Mat mask = cv::Mat::zeros(n_rows,n_cols,1);

        cv::drawContours(mask,red_contours,i, cv::Scalar(255), cv::FILLED);
        cv::bitwise_or(img_original,img_original,img_red_crop,mask);


        cv::inRange(img_red_crop, cv::Scalar(BKHmin,BKSmin,BKVmin), cv::Scalar(BKHmax,BKSmax,BKVmax), img_red_crop);
        cv::bitwise_not(img_red_crop,img_red_crop);
        //cv::erode(img_red_crop, img_red_crop, cv::Mat(), cv::Point(-1,-1), 1);
        //cv::dilate(img_red_crop, img_red_crop, cv::Mat(), cv::Point(-1,-1), 1);

        std::vector<std::vector<cv::Point>> red_subcontours;
        std::vector<cv::Vec4i> sub_hierarchy;
        cv::findContours(img_red_crop,red_subcontours,sub_hierarchy,cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

        int Xc,Yc,Xt,Yt;  //Coordenadas para triangulo y cuadrado
        int Ncircles=0;
        int cuad=0;
        int trian=0;

        //Aqui va ir otro for con subcontours
        for (int ii=1; ii<red_subcontours.size(); ii++)
        {
          //cv::drawContours(img_red_crop,red_subcontours,1, cv::Scalar(255), cv::FILLED);

          cv::Mat poly;
          double perim=0;
          cv::Moments sub_moments;
          int Xsub, Ysub;
          perim = cv::arcLength(red_subcontours[ii], true);
          cv::approxPolyDP(red_subcontours[ii], poly, 0.04 * perim, true);

          sub_moments = cv::moments(red_subcontours[ii]);
          Xsub = sub_moments.m10 / sub_moments.m00;
          Ysub = sub_moments.m01 / sub_moments.m00;

          cv::circle(img_mod, cv::Point2i(Xsub, Ysub), 1, cv::Scalar(0, 0, 255), 3); //Dibujar punto

          if(poly.rows==3)
          {
            Xt=Xsub;
            Yt=Ysub;
            trian=1;
          }

          else if (poly.rows==4){
            Xc=Xsub;
            Yc=Ysub;
            cuad=1;
          }

          else
          {
            Ncircles++;
          }

        }

        if(trian==1 && cuad==1)  //Si se cumple que esta el triangulo y cuadrado en la figura, calcula el angulo y asigna #robot
        {
          float angle=-1;
          angle = atan2((Yt-Yc),(Xt-Xc));

          if(Ncircles==1)
          {
            R1x = X+(45*cos(angle));
            R1y = Y+(45*sin(angle));
            R1ang = angle;

            cv::circle(img_mod, cv::Point2i(R1x, R1y), 2, cv::Scalar(0, 0, 255), 3); //Dibujar punto

            char Data[80];
            sprintf(Data, "Robot 1");
            cv::putText(img_mod, Data, cv::Point(R1x + 35, R1y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "x= %d", R1x);
            cv::putText(img_mod, Data, cv::Point(R1x + 35, R1y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "y= %d", R1y);
            cv::putText(img_mod, Data, cv::Point(R1x + 35, R1y+24), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "ang= %f", R1ang);
            cv::putText(img_mod, Data, cv::Point(R1x + 35, R1y+36), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
          }
          else if(Ncircles==2)
          {
            R2x = X+(45*cos(angle));
            R2y = Y+(45*sin(angle));
            R2ang = angle;

            cv::circle(img_mod, cv::Point2i(R2x, R2y), 2, cv::Scalar(0, 0, 255), 3); //Dibujar punto

            char Data[80];
            sprintf(Data, "Robot 2");
            cv::putText(img_mod, Data, cv::Point(R2x + 35, R2y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "x= %d", R2x);
            cv::putText(img_mod, Data, cv::Point(R2x + 35, R2y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "y= %d", R2y);
            cv::putText(img_mod, Data, cv::Point(R2x + 35, R2y+24), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "ang= %f", R2ang);
            cv::putText(img_mod, Data, cv::Point(R2x + 35, R2y+36), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
          }

        }
      }


      //-------------------------------Filtro Azul para Objetos-------------------------------------------------------
      cv::Mat img_blue_bin;
      
      cv::inRange(img_hsv, cv::Scalar(BHmin,BSmin,BVmin), cv::Scalar(BHmax,BSmax,BVmax), img_blue_bin);  //HSV filter

      std::vector<std::vector<cv::Point>> blue_contours;
      std::vector<cv::Vec4i> hierarchy_blue;
      cv::findContours(img_blue_bin,blue_contours,hierarchy_blue,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

      //Esto debe de ir dentro del for de contours

      for (int i=0; i<blue_contours.size(); i++)
      { 
        cv::Moments moments;  //Moments para calcular el centro de la figura
        int X, Y;

        moments = cv::moments(blue_contours[i]);
        X = moments.m10 / moments.m00;
        Y = moments.m01 / moments.m00;

        cv::circle(img_mod, cv::Point2i(X, Y), 2, cv::Scalar(255, 0, 0), 3); //Dibujar punto

        cv::Mat img_blue_crop;
        cv::Mat mask = cv::Mat::zeros(n_rows,n_cols,1);

        cv::drawContours(mask,blue_contours,i, cv::Scalar(255), cv::FILLED);
        cv::bitwise_or(img_original,img_original,img_blue_crop,mask);


        cv::inRange(img_blue_crop, cv::Scalar(BKHmin,BKSmin,BKVmin), cv::Scalar(BKHmax,BKSmax,BKVmax), img_blue_crop);
        cv::bitwise_not(img_blue_crop,img_blue_crop);
        //cv::erode(img_blue_crop, img_blue_crop, cv::Mat(), cv::Point(0,0), 1);

        std::vector<std::vector<cv::Point>> blue_subcontours;
        std::vector<cv::Vec4i> sub_hierarchy;
        cv::findContours(img_blue_crop,blue_subcontours,sub_hierarchy,cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

        int Xc,Yc,Xt,Yt;  //Coordenadas para triangulo y cuadrado
        int Ncircles=0;
        int cuad=0;
        int trian=0;


        //Aqui va ir otro for con subcontours
        for (int ii=1; ii<blue_subcontours.size(); ii++)
        {
          //cv::drawContours(img_blue_crop,blue_subcontours,1, cv::Scalar(255), cv::FILLED);



          cv::Mat poly;
          double perim=0;
          cv::Moments sub_moments;
          int Xsub, Ysub;
          perim = cv::arcLength(blue_subcontours[ii], true);
          cv::approxPolyDP(blue_subcontours[ii], poly, 0.04 * perim, true);

          sub_moments = cv::moments(blue_subcontours[ii]);
          Xsub = sub_moments.m10 / sub_moments.m00;
          Ysub = sub_moments.m01 / sub_moments.m00;

          cv::circle(img_mod, cv::Point2i(Xsub, Ysub), 1, cv::Scalar(255, 0, 0), 3); //Dibujar punto

          if(poly.rows==3)
          {
            Xt=Xsub;
            Yt=Ysub;
            trian=1;
          }

          else if (poly.rows==4){
            Xc=Xsub;
            Yc=Ysub;
            cuad=1;
          }

          else
          {
            Ncircles++;
          }




        }

        if(trian==1 && cuad==1)  //Si se cumple que esta el triangulo y cuadrado en la figura, calcula el angulo y asigna #robot
        {
          float angle=-1;
          angle = atan2((Yt-Yc),(Xt-Xc));

          if(Ncircles==1)
          {
            Obj1x = X;
            Obj1y = Y;
            Obj1ang = angle;

            char Data[80];
            sprintf(Data, "Object 1");
            cv::putText(img_mod, Data, cv::Point(Obj1x + 35, Obj1y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "x= %d", Obj1x);
            cv::putText(img_mod, Data, cv::Point(Obj1x + 35, Obj1y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "y= %d", Obj1y);
            cv::putText(img_mod, Data, cv::Point(Obj1x + 35, Obj1y+24), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "ang= %f", Obj1ang);
            cv::putText(img_mod, Data, cv::Point(Obj1x + 35, Obj1y+36), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
          }
          else if(Ncircles==2)
          {
            Obj2x = X;
            Obj2y = Y;
            Obj2ang = angle;

            char Data[80];
            sprintf(Data, "Object 2");
            cv::putText(img_mod, Data, cv::Point(Obj2x + 35, Obj2y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "x= %d", Obj2x);
            cv::putText(img_mod, Data, cv::Point(Obj2x + 35, Obj2y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "y= %d", Obj2y);
            cv::putText(img_mod, Data, cv::Point(Obj2x + 35, Obj2y+24), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "ang= %f", Obj2ang);
            cv::putText(img_mod, Data, cv::Point(Obj2x + 35, Obj2y+36), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
          }

        }
      }


      //-------------------------------Filtro Verde para Objetivos-------------------------------------------------------
      cv::Mat img_green_bin;
      
      cv::inRange(img_hsv, cv::Scalar(GHmin,GSmin,GVmin), cv::Scalar(GHmax,GSmax,GVmax), img_green_bin);  //HSV filter

      std::vector<std::vector<cv::Point>> green_contours;
      std::vector<cv::Vec4i> hierarchy_green;
      cv::findContours(img_green_bin,green_contours,hierarchy_green,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

      //Esto debe de ir dentro del for de contours

      for (int i=0; i<green_contours.size(); i++)
      { 
        cv::Moments moments;  //Moments para calcular el centro de la figura
        int X, Y;

        moments = cv::moments(green_contours[i]);
        X = moments.m10 / moments.m00;
        Y = moments.m01 / moments.m00;

        cv::circle(img_mod, cv::Point2i(X, Y), 2, cv::Scalar(0, 255, 0), 3); //Dibujar punto

        cv::Mat img_green_crop;
        cv::Mat mask = cv::Mat::zeros(n_rows,n_cols,1);

        cv::drawContours(mask,green_contours,i, cv::Scalar(255), cv::FILLED);
        cv::bitwise_or(img_original,img_original,img_green_crop,mask);


        cv::inRange(img_green_crop, cv::Scalar(BKHmin,BKSmin,BKVmin), cv::Scalar(BKHmax,BKSmax,BKVmax), img_green_crop);
        cv::bitwise_not(img_green_crop,img_green_crop);
        //cv::erode(img_green_crop, img_green_crop, cv::Mat(), cv::Point(0,0), 1);

        std::vector<std::vector<cv::Point>> green_subcontours;
        std::vector<cv::Vec4i> sub_hierarchy;
        cv::findContours(img_green_crop,green_subcontours,sub_hierarchy,cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

        int Xc,Yc,Xt,Yt;  //Coordenadas para triangulo y cuadrado
        int Ncircles=0;
        int cuad=0;
        int trian=0;

        //Aqui va ir otro for con subcontours
        for (int ii=1; ii<green_subcontours.size(); ii++)
        {
          //cv::drawContours(img_green_crop,green_subcontours,1, cv::Scalar(255), cv::FILLED);



          cv::Mat poly;
          double perim=0;
          cv::Moments sub_moments;
          int Xsub, Ysub;
          perim = cv::arcLength(green_subcontours[ii], true);
          cv::approxPolyDP(green_subcontours[ii], poly, 0.04 * perim, true);

          sub_moments = cv::moments(green_subcontours[ii]);
          Xsub = sub_moments.m10 / sub_moments.m00;
          Ysub = sub_moments.m01 / sub_moments.m00;

          cv::circle(img_mod, cv::Point2i(Xsub, Ysub), 1, cv::Scalar(0, 255, 0), 3); //Dibujar punto

          if(poly.rows==3)
          {
            Xt=Xsub;
            Yt=Ysub;
            trian=1;
          }

          else if (poly.rows==4){
            Xc=Xsub;
            Yc=Ysub;
            cuad=1;
          }

          else
          {
            Ncircles++;
          }




        }

        if(trian==1 && cuad==1)  //Si se cumple que esta el triangulo y cuadrado en la figura, calcula el angulo y asigna #robot
        {
          float angle=-1;
          angle = atan2((Yt-Yc),(Xt-Xc));

          if(Ncircles==1)
          {
            Tgt1x = X;
            Tgt1y = Y;
            Tgt1ang = angle;

            char Data[80];
            sprintf(Data, "Target 1");
            cv::putText(img_mod, Data, cv::Point(Tgt1x + 35, Tgt1y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "x= %d", Tgt1x);
            cv::putText(img_mod, Data, cv::Point(Tgt1x + 35, Tgt1y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "y= %d", Tgt1y);
            cv::putText(img_mod, Data, cv::Point(Tgt1x + 35, Tgt1y+24), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "ang= %f", Tgt1ang);
            cv::putText(img_mod, Data, cv::Point(Tgt1x + 35, Tgt1y+36), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
          }
          else if(Ncircles==2)
          {
            Tgt2x = X;
            Tgt2y = Y;
            Tgt2ang = angle;

            char Data[80];
            sprintf(Data, "Target 2");
            cv::putText(img_mod, Data, cv::Point(Tgt2x + 35, Tgt2y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "x= %d", Tgt2x);
            cv::putText(img_mod, Data, cv::Point(Tgt2x + 35, Tgt2y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "y= %d", Tgt2y);
            cv::putText(img_mod, Data, cv::Point(Tgt2x + 35, Tgt2y+24), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            sprintf(Data, "ang= %f", Tgt2ang);
            cv::putText(img_mod, Data, cv::Point(Tgt2x + 35, Tgt2y+36), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
          }

        }
      }
      
      


      interfaces::msg::ImgData data_image;

      data_image.r1_x = R1x;
      data_image.r1_y = R1y;
      data_image.r1_ang = R1ang;
      data_image.r2_x = R2x;
      data_image.r2_y = R2y;
      data_image.r2_ang = R2ang;
      data_image.obj1_x = Obj1x;
      data_image.obj1_y = Obj1y;
      data_image.obj1_ang = Obj1ang;
      data_image.obj2_x = Obj2x;
      data_image.obj2_y = Obj2y;
      data_image.obj2_ang = Obj2ang;
      data_image.tgt_x = Tgt1x;
      data_image.tgt_y = Tgt1y;
      data_image.tgt_ang = Tgt1ang;

      publisher->publish(data_image);

      

  	  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  	  cv::imshow("Display Image", img_mod);
  	  cv::waitKey(1);
      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    rclcpp::Publisher<interfaces::msg::ImgData>::SharedPtr publisher;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);


  cv::namedWindow("HSV_RED", cv::WINDOW_AUTOSIZE);
  cv::createTrackbar("Hmin", "HSV_RED", &RHmin, 180);
  cv::createTrackbar("Hmax", "HSV_RED", &RHmax, 180);
  cv::createTrackbar("Smin", "HSV_RED", &RSmin, 255);
  cv::createTrackbar("Smax", "HSV_RED", &RSmax, 255);
  cv::createTrackbar("Vmin", "HSV_RED", &RVmin, 255);
  cv::createTrackbar("Vmax", "HSV_RED", &RVmax, 255);
  //cv::resizeWindow("HSV_RED", 350, 350); // No sirven

  cv::namedWindow("HSV_BK", cv::WINDOW_AUTOSIZE);
  cv::createTrackbar("Hmin", "HSV_BK", &BKHmin, 180);
  cv::createTrackbar("Hmax", "HSV_BK", &BKHmax, 180);
  cv::createTrackbar("Smin", "HSV_BK", &BKSmin, 255);
  cv::createTrackbar("Smax", "HSV_BK", &BKSmax, 255);
  cv::createTrackbar("Vmin", "HSV_BK", &BKVmin, 255);
  cv::createTrackbar("Vmax", "HSV_BK", &BKVmax, 255);
  //cv::resizeWindow("HSV_BK", 350, 350);

  cv::namedWindow("HSV_B", cv::WINDOW_AUTOSIZE);
  cv::createTrackbar("Hmin", "HSV_B", &BHmin, 180);
  cv::createTrackbar("Hmax", "HSV_B", &BHmax, 180);
  cv::createTrackbar("Smin", "HSV_B", &BSmin, 255);
  cv::createTrackbar("Smax", "HSV_B", &BSmax, 255);
  cv::createTrackbar("Vmin", "HSV_B", &BVmin, 255);
  cv::createTrackbar("Vmax", "HSV_B", &BVmax, 255);

  cv::namedWindow("HSV_G", cv::WINDOW_AUTOSIZE);
  cv::createTrackbar("Hmin", "HSV_G", &GHmin, 180);
  cv::createTrackbar("Hmax", "HSV_G", &GHmax, 180);
  cv::createTrackbar("Smin", "HSV_G", &GSmin, 255);
  cv::createTrackbar("Smax", "HSV_G", &GSmax, 255);
  cv::createTrackbar("Vmin", "HSV_G", &GVmin, 255);
  cv::createTrackbar("Vmax", "HSV_G", &GVmax, 255);


  rclcpp::spin(std::make_shared<Img_Proc_Id_Node>());



  rclcpp::shutdown();



  return 0;
}