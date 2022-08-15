#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>

#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/bool.hpp>









int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;

	cv::Mat image = cv::imread("/home/javierfr/Downloads/image.jpeg");
	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
	cv::imshow("Display Image", image);
	cv::waitKey(0);





  return 0;
}
