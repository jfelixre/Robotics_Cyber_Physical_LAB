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

       
    detachr01c11 = ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_01/cube_11/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
    
    detachr01c12 = ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_01/cube_12/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
    
    detachr01c21 = ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_01/cube_21/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
    
    detachr02c11 = ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_02/cube_11/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
    
    detachr02c12 = ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_02/cube_12/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
    
    detachr02c21 = ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_02/cube_21/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )


    return LaunchDescription([
        detachr01c11,
        detachr01c12,
        detachr01c21,
        detachr02c11,
        detachr02c12,
        detachr02c21,
       
    ])