#include <iostream>
#include <sstream>
#include <fstream>

#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp> 
#include <opencv2/aruco.hpp> 
#include <opencv2/core.hpp> 

using namespace std;
using namespace cv;


const float calibrationSquareDimensions = 0.01905f;//meters
const float arucoSquareDimension = 0.1016f;//meters
const Size chessboardDimensions = Size(8,5);


void createArucoMarkers()
{   Mat outputMarker;
    //Aruco Marker Dictionary objects
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
   //Aruco Markers put into our Mat - To print
   for(int i = 0; i<50; i++){
    aruco::drawMarker(markerDictionary,i,500,outputMarker,1);
    //Create Aruco file inside folder
    ostringstream convert;
    string imageName = "4x4Marker_";
    convert << imageName << i << ".jpg";
    imwrite(convert.str(),outputMarker);
   }
}

void createKnowBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners){
    //Points calculated are going to be stored in a Point3f, we have an X, Y and Z coordinate. 
    //However that is a flat plane, so Z coordinates are going to be 0.
    for (int i = 0; i< boardSize.height; i++){
         for (int j = 0; j< boardSize.width; j++){
            corners.push_back(Point3f(j*squareEdgeLength, i* squareEdgeLength, 0.0f));
        }
    }
}
//Extract from an image the chessboard corners that are being detected
void getChessBoardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResults = false){
    for(vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++){
        vector<Point2f> pointBuf; 
        bool found = findChessboardCorners(*iter,Size(8,5),pointBuf,cv::CALIB_CB_ADAPTIVE_THRESH|cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found){
            allFoundCorners.push_back(pointBuf);
        }
        if (showResults){
            drawChessboardCorners(*iter, Size(8,5), pointBuf, found);
            imshow("Looking for corners", *iter);
            waitKey(0);
        }
    }   
}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoefficients)
{
    vector<vector<Point2f>> checkerboardImageSpacePoints;
    getChessBoardCorners(calibrationImages,checkerboardImageSpacePoints,false);
    vector<vector<Point3f>> worldSpaceCornerPoints(1);

    createKnowBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

    vector<Mat> rVectors, tVectors;
    distanceCoefficients = Mat::zeros(8,1,CV_64F);

    calibrateCamera(worldSpaceCornerPoints,checkerboardImageSpacePoints,boardSize,cameraMatrix,distanceCoefficients,rVectors,tVectors);



}

bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients)
{
    ofstream outStream(name);
    if(outStream)
    {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for ( int r = 0; r < rows; r++)
        {
            for (int c  = 0; c < columns; c++)
            {
                double value = cameraMatrix.at<double>(r,c);
                outStream << value << endl; 
            }
            
        }
        


        rows = distanceCoefficients.rows;
        columns = distanceCoefficients.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for ( int r = 0; r < rows; r++)
        {
            for (int c  = 0; c < columns; c++)
            {
                double value = distanceCoefficients.at<double>(r,c);
                outStream << value << endl; 
            }
            
        }

        outStream.close();
        return true;
        
    }
    return false;
}




int main(int argv, char** argc)
{
    
    //createArucoMarkers();
    Mat frame;
    Mat drawToFrame;

    Mat cameraMatrix = Mat::eye(3,3,CV_64F);

    Mat distanceCoefficients;

    vector<Mat> savedImages; 

    vector<vector<Point2f>> markerCorners, rejectedCandidates;

    VideoCapture vid(2);

    if (!vid.isOpened()){
        return 0;
    }
 
    int framePerSecond = 20;
    namedWindow("LogiTech Camera",cv::WINDOW_AUTOSIZE);

    while (true)
    {
        if(!vid.read(frame))
            break;

        vector<Vec2f> foundPoints;
        bool found = false;
        
        found = findChessboardCorners(frame, chessboardDimensions, foundPoints, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE );
        frame.copyTo(drawToFrame);

        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
        
        if(found)
            imshow("LogiTechCam", drawToFrame);
        else
            imshow("LogiTechCam",frame);
        
        char character = waitKey(1000/framePerSecond);

        switch (character)
        {
        case ' ':
            if (found){
                Mat temp;
                frame.copyTo(temp);
                savedImages.push_back(temp);
            }
        //saving image
            break;
        case 13:
        //start calibration
            if (savedImages.size() > 15){
                cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimensions, cameraMatrix,distanceCoefficients);
                saveCameraCalibration("CameraCalibration",cameraMatrix, distanceCoefficients);
                
            }
            break;
        case 27:
            //exit
            return 0;
            break;
        }
        
    }
    
    return 0;

}
