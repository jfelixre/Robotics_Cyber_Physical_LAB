#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "../include/img_proc_pkg/aruco_nano.h"

class CalibrationNode : public rclcpp::Node {
public:
    CalibrationNode() : Node("calibration_node") {
        this->declare_parameter<std::string>("camera_topic", "/cameras/cam_1");
        this->declare_parameter<std::string>("output_file", "/home/javierfr/Robotics_Cyber_Physical_LAB/src/img_proc_pkg/config/calibration_result.yaml");
        this->declare_parameter<int>("samples", 100);
        this->declare_parameter<double>("capture_delay", 1.0); // Seconds between captures

        std::string camera_topic = this->get_parameter("camera_topic").as_string();
        output_file_ = this->get_parameter("output_file").as_string();
        target_samples_ = this->get_parameter("samples").as_int();
        capture_delay_ = this->get_parameter("capture_delay").as_double();

        // Initialize last_capture_time_ with the correct clock type
        last_capture_time_ = rclcpp::Time(int64_t(0), this->get_clock()->get_clock_type());

        // Initialize fixed markers map (Based on world_cam.sdf)
        fixed_markers_[0] = cv::Point3f(0.0, 0.0, 0.0);
        fixed_markers_[99] = cv::Point3f(1.0, 2.0, 0.0);
        fixed_markers_[98] = cv::Point3f(1.0, 0.0, 0.0);
        fixed_markers_[97] = cv::Point3f(1.0, -2.0, 0.0);
        fixed_markers_[96] = cv::Point3f(0.0, 2.0, 0.0);
        fixed_markers_[95] = cv::Point3f(0.0, -2.0, 0.0);
        fixed_markers_[94] = cv::Point3f(-1.0, 2.0, 0.0);
        fixed_markers_[93] = cv::Point3f(-1.0, 0.0, 0.0);
        fixed_markers_[92] = cv::Point3f(-1.0, -2.0, 0.0);

        sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            camera_topic, 10, std::bind(&CalibrationNode::imageCallback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Calibration Node Started. Listening on %s. Collecting %d samples...", camera_topic.c_str(), target_samples_);
    }

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg) {
        if (samples_collected_ >= target_samples_) return;

        // Check time delay
        rclcpp::Time current_time = this->now();
        if ((current_time - last_capture_time_).seconds() < capture_delay_) {
            return;
        }

        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }

        image_size_ = cv_ptr->image.size();
        auto markers = aruconano::MarkerDetector::detect(cv_ptr->image);

        std::vector<cv::Point3f> frame_obj_points;
        std::vector<cv::Point2f> frame_img_points;

        for (const auto& m : markers) {
            if (fixed_markers_.count(m.id)) {
                // Marker size 0.12m (Exact mesh size from DAE)
                float s = 0.12f / 2.0f;
                cv::Point3f center = fixed_markers_[m.id];

                // Assuming markers are rotated 90 degrees Clockwise (Top points to +X)
                // TL of texture is at (+s, +s) in World Frame
                // TR of texture is at (+s, -s)
                // BR of texture is at (-s, -s)
                // BL of texture is at (-s, +s)
                
                frame_obj_points.push_back(cv::Point3f(center.x + s, center.y + s, 0)); // TL
                frame_obj_points.push_back(cv::Point3f(center.x + s, center.y - s, 0)); // TR
                frame_obj_points.push_back(cv::Point3f(center.x - s, center.y - s, 0)); // BR
                frame_obj_points.push_back(cv::Point3f(center.x - s, center.y + s, 0)); // BL

                frame_img_points.push_back(m[0]);
                frame_img_points.push_back(m[1]);
                frame_img_points.push_back(m[2]);
                frame_img_points.push_back(m[3]);
            }
        }

        if (!frame_obj_points.empty()) {
            all_obj_points_.push_back(frame_obj_points);
            all_img_points_.push_back(frame_img_points);
            samples_collected_++;
            last_capture_time_ = this->now();
            
            if (samples_collected_ % 10 == 0) {
                RCLCPP_INFO(this->get_logger(), "Collected %d/%d samples", samples_collected_, target_samples_);
            }

            if (samples_collected_ >= target_samples_) {
                performCalibration();
            }
        }
    }

    void performCalibration() {
        RCLCPP_INFO(this->get_logger(), "Starting calibration with %zu frames...", all_obj_points_.size());
        cv::Mat cameraMatrix, distCoeffs;
        std::vector<cv::Mat> rvecs, tvecs;
        
        // Initial guess for 4K camera (3840x2160)
        // HFOV = 1.39626 rad (~80 deg)
        // fx = W / (2 * tan(HFOV/2)) = 3840 / (2 * tan(0.698)) = 3840 / 1.678 = 2288
        double fx_guess = 2288.0; 
        double fy_guess = 2288.0;
        double cx_guess = image_size_.width / 2.0;
        double cy_guess = image_size_.height / 2.0;

        cameraMatrix = (cv::Mat_<double>(3, 3) << 
            fx_guess, 0, cx_guess,
            0, fy_guess, cy_guess,
            0, 0, 1);
        
        distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

        // Use INTRINSIC_GUESS and FIX_DISTORTION for Gazebo (perfect lens)
        // Note: OpenCV 3/4 uses CALIB_ZERO_TANGENT_DIST to fix tangential (p1, p2) to zero.
        // There are no explicit CALIB_FIX_T1/T2 flags, they are part of the distortion model.
        int flags = cv::CALIB_USE_INTRINSIC_GUESS | 
                    cv::CALIB_FIX_K1 | cv::CALIB_FIX_K2 | cv::CALIB_FIX_K3 |
                    cv::CALIB_ZERO_TANGENT_DIST;
        
        double rms = cv::calibrateCamera(all_obj_points_, all_img_points_, image_size_, 
                                         cameraMatrix, distCoeffs, rvecs, tvecs, flags);

        RCLCPP_INFO(this->get_logger(), "Calibration finished. RMS Error: %f", rms);
        saveCalibration(cameraMatrix, distCoeffs, rms);
        
        // Keep node alive but stop collecting? Or shutdown?
        // Shutdown is better for a tool.
        rclcpp::shutdown();
    }

    void saveCalibration(const cv::Mat& K, const cv::Mat& D, double rms) {
        cv::FileStorage fs(output_file_, cv::FileStorage::WRITE);
        
        time_t rawtime;
        time(&rawtime);
        fs << "calibration_date" << asctime(localtime(&rawtime));
        
        fs << "image_width" << image_size_.width;
        fs << "image_height" << image_size_.height;
        fs << "camera_name" << "calibrated_camera";
        fs << "camera_matrix" << K;
        fs << "distortion_coefficients" << D;
        fs << "avg_reprojection_error" << rms;
        fs.release();
        RCLCPP_INFO(this->get_logger(), "Saved calibration to %s", output_file_.c_str());
    }

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    std::map<int, cv::Point3f> fixed_markers_;
    std::vector<std::vector<cv::Point3f>> all_obj_points_;
    std::vector<std::vector<cv::Point2f>> all_img_points_;
    int samples_collected_ = 0;
    int target_samples_;
    double capture_delay_;
    rclcpp::Time last_capture_time_ = rclcpp::Time(0);
    std::string output_file_;
    cv::Size image_size_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CalibrationNode>());
    rclcpp::shutdown();
    return 0;
}
