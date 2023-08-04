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


    gazebo = launch.actions.ExecuteProcess(
        cmd=['gazebo', '--verbose', 'src/gazebo_plugin_sim/worlds/2Robots_aruco.world'],
        output='screen'
    )

    gzserver = launch.actions.ExecuteProcess(
        cmd=['gzserver', '--verbose', 'src/gazebo_plugin_sim/worlds/2Robots_aruco.world'],
        output='screen'
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='true',
            description='Use simulation (Gazebo) clock if true'),
        #Node(
         #   package='robot_state_publisher',
         #   executable='robot_state_publisher',
         #   name='robot_state_publisher',
         #   output='screen',
         #   parameters=[{'use_sim_time': use_sim_time, 'robot_description': robot_desc}],
         #   arguments=[urdf]),
        #Node(
         #   package='robot_state_publisher',
          #  executable='robot_state_publisher',
           # name='robot_state_publisher',
           # output='screen',
           # parameters=[{'use_sim_time': use_sim_time, 'robot_description': origin_desc}],
           # arguments=[urdf_origin]),
        

        #Node(
        #    package='control_pkg',
        #    executable='control_trajectory_r1'),

        Node(
            package='inv_kinematics_pkg',
            executable='robot_1_platform_vel_server'),

        Node(
            package='control_pkg',
            executable='control_arm_pid_r1'),

        Node(
            package='control_pkg',
            executable='event_driven_control_r1'),

        Node(
            package='control_pkg',
            executable='csv_r1'),

  
        
    ])