import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.actions import ExecuteProcess
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution

from launch_ros.actions import Node

def generate_launch_description():
    # Configure ROS nodes for launch

    # Setup project paths
    pkg_project_control_pkg = get_package_share_directory('control_pkg')
    pkg_project_gazebo_plugin_sim = get_package_share_directory('gazebo_plugin_sim')
    pkg_project_img_proc_pkg = get_package_share_directory('img_proc_pkg')
    pkg_project_interfaces = get_package_share_directory('interfaces')
    pkg_project_inv_kinematics_pkg = get_package_share_directory('inv_kinematics_pkg')
    pkg_project_launch_pkg = get_package_share_directory('launch_pkg')
    pkg_project_robot_custom_description = get_package_share_directory('robot_custom_description')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

   #Aruco nano detection node for Camera 1
    aruco_cam1 = Node(
        package='img_proc_pkg',
        executable='aruco_nano_detector',
        name='aruco_detector_cam1',
        parameters=[{
            'camera_topic': '/cameras/cam_1',
            'camera_frame': 'cam_1',
            'calibration_file': os.path.join(pkg_project_img_proc_pkg, 'config', 'camera_calib_charuco.yaml')
        }]
    )

    # Example for a second camera (commented out)
    # aruco_cam2 = Node(
    #     package='img_proc_pkg',
    #     executable='aruco_nano_detector',
    #     name='aruco_detector_cam2',
    #     parameters=[{
    #         'camera_topic': '/cameras/cam_2',
    #         'camera_frame': 'cam_2',
    #         'calibration_file': os.path.join(pkg_project_img_proc_pkg, 'config', 'camera_calib_charuco.yaml')
    #     }]
    # )

    return LaunchDescription([
        aruco_cam1,
        # aruco_cam2
    ])