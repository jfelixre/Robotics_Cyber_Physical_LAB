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
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/aruco.hpp> 
#include <stdio.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/imgproc.hpp>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/image_encodings.hpp>
//#include <std_msgs/msg/bool.hpp>
//#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <interfaces/msg/img_data.hpp>
#include "/opt/opencv_contrib/modules/aruco/samples/aruco_samples_utility.hpp"

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"



using std::placeholders::_1;


using namespace std::chrono_literals;
using namespace sensor_msgs::image_encodings;
namespace enc = sensor_msgs::image_encodings;

using namespace std;
using namespace cv;

cv::Mat img_original;
cv::Mat imageCopy;
cv::Mat cameraMatrix;
cv::Mat distCoeffs;

int squaresX = 5;
int squaresY = 8;
float squareLength = 0.027;
float markerLength = 0.022;
string outputFile = "src/img_proc_pkg/config/camera_calib_charuco.yaml";
int calibrationFlags = 0;
float aspectRatio = 1;
Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();
int dictionaryId = 0;
Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));
int waitTime=10;


// create charuco board object
Ptr<aruco::CharucoBoard> charucoboard = aruco::CharucoBoard::create(squaresX, squaresY, squareLength, markerLength, dictionary);
Ptr<aruco::Board> board = charucoboard.staticCast<aruco::Board>();

// collect data from each frame
vector< vector< vector< Point2f > > > allCorners;
vector< vector< int > > allIds;
vector< Mat > allImgs;
Size imgSize;



class Calibrate_Camera_Charuco_Node : public rclcpp::Node
{
  public:
    Calibrate_Camera_Charuco_Node() : Node("calibrate_camera_charuco_node")
    {
      //bool readOk = readCameraParameters("src/img_proc_pkg/config/camera_calib_charuco.yaml", cameraMatrix, distCoeffs);

      subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "/image_raw", 10, std::bind(&Calibrate_Camera_Charuco_Node::topic_callback, this, _1));
      
  


    }

  private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
      //RCLCPP_INFO(this->get_logger(), "Received image" );
      
      cv_bridge::CvImageConstPtr image_bridge;
	    //cv_bridge::toCvCopy(msg, RGB8)->image;
      image_bridge=cv_bridge::toCvCopy(msg);
      img_original = image_bridge->image;
      cv::cvtColor(img_original,img_original,cv::COLOR_BGR2RGB);
      //cv::imwrite("image.jpg",img_original);
      int n_cols = img_original.cols;
      int n_rows = img_original.rows;

      vector< int > ids;
      vector< vector< Point2f > > corners, rejected;

      // detect markers
      aruco::detectMarkers(img_original, dictionary, corners, ids, detectorParams, rejected);

      // interpolate charuco corners
      Mat currentCharucoCorners, currentCharucoIds;
      if(ids.size() > 0)
        aruco::interpolateCornersCharuco(corners, ids, img_original, charucoboard, currentCharucoCorners,
                                             currentCharucoIds);

      // draw results
      img_original.copyTo(imageCopy);
      if(ids.size() > 0) aruco::drawDetectedMarkers(imageCopy, corners);

      if(currentCharucoCorners.total() > 0)
            aruco::drawDetectedCornersCharuco(imageCopy, currentCharucoCorners, currentCharucoIds);

      putText(imageCopy, "Press 'c' to add current frame. 'ESC' to finish and calibrate",
              Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2);

      imshow("out", imageCopy);
      char key = (char)waitKey(waitTime);
      
      if(key == 'c' && ids.size() > 0) {
            RCLCPP_INFO(this->get_logger(),"Frame captured");
            allCorners.push_back(corners);
            allIds.push_back(ids);
            allImgs.push_back(img_original);
            imgSize = img_original.size();
      }

      if(key == 27){
        if(allIds.size() < 1) {
        RCLCPP_INFO(this->get_logger(), "Not enough captures for calibration" );
        }

        vector< Mat > rvecs, tvecs;
        double repError;

        // prepare data for calibration
        vector< vector< Point2f > > allCornersConcatenated;
        vector< int > allIdsConcatenated;
        vector< int > markerCounterPerFrame;
        markerCounterPerFrame.reserve(allCorners.size());
        for(unsigned int i = 0; i < allCorners.size(); i++) {
            markerCounterPerFrame.push_back((int)allCorners[i].size());
            for(unsigned int j = 0; j < allCorners[i].size(); j++) {
                allCornersConcatenated.push_back(allCorners[i][j]);
                allIdsConcatenated.push_back(allIds[i][j]);
            }
        }

        // calibrate camera using aruco markers
        double arucoRepErr;
        arucoRepErr = aruco::calibrateCameraAruco(allCornersConcatenated, allIdsConcatenated,
                                                  markerCounterPerFrame, board, imgSize, cameraMatrix,
                                                  distCoeffs, noArray(), noArray(), calibrationFlags);

        // prepare data for charuco calibration
        int nFrames = (int)allCorners.size();
        vector< Mat > allCharucoCorners;
        vector< Mat > allCharucoIds;
        vector< Mat > filteredImages;
        allCharucoCorners.reserve(nFrames);
        allCharucoIds.reserve(nFrames);

        for(int i = 0; i < nFrames; i++) {
            // interpolate using camera parameters
            Mat currentCharucoCorners, currentCharucoIds;
            aruco::interpolateCornersCharuco(allCorners[i], allIds[i], allImgs[i], charucoboard,
                                             currentCharucoCorners, currentCharucoIds, cameraMatrix,
                                             distCoeffs);

            allCharucoCorners.push_back(currentCharucoCorners);
            allCharucoIds.push_back(currentCharucoIds);
            filteredImages.push_back(allImgs[i]);
        }

        if(allCharucoCorners.size() < 4) {
            RCLCPP_INFO(this->get_logger(),"Not enough corners for calibration");
        }

        // calibrate camera using charuco
        repError =
            aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds, charucoboard, imgSize,
                                          cameraMatrix, distCoeffs, rvecs, tvecs, calibrationFlags);

        bool saveOk =  saveCameraParams(outputFile, imgSize, aspectRatio, calibrationFlags,
                                        cameraMatrix, distCoeffs, repError);
        if(!saveOk) {
            RCLCPP_INFO(this->get_logger(),"Cannot save output file");
        }

        //cout << "Rep Error: " << repError << endl;
        //cout << "Rep Error Aruco: " << arucoRepErr << endl;
        //cout << "Calibration saved to " << outputFile << endl;
        RCLCPP_INFO(this->get_logger(), "Calibration Saved");


      }



      
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Calibrate_Camera_Charuco_Node>());
  rclcpp::shutdown();
  return 0;
}