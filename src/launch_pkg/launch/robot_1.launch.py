import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
import launch
import time
import launch.actions
import launch_ros.actions

def generate_launch_description():

    use_sim_time = LaunchConfiguration('use_sim_time', default='true')

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='true',
            description='Use simulation (Gazebo) clock if true'),

        Node(
            package='task_pkg',
            namespace='robot_1',
            executable='task_manager_node',
            parameters=[{'robot_id': 1}],
            ),

        Node(
            package='control_pkg',
            namespace='robot_1',
            executable='event_driven_control',
            parameters=[{'robot_id': 1}],
            ),
     
    ])