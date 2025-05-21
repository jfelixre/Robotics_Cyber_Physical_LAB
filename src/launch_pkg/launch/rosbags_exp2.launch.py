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

    delete1 = ExecuteProcess(
        cmd=[[
            'rm -r bags/positions ',
        ]],
        shell=True
    )

    delete2 = ExecuteProcess(
        cmd=[[
            'rm -r bags/error_r1 ',
        ]],
        shell=True
    )

    delete3 = ExecuteProcess(
        cmd=[[
            'rm -r bags/error_r2 ',
        ]],
        shell=True
    )

    delete4 = ExecuteProcess(
        cmd=[[
            'rm -r bags/state ',
        ]],
        shell=True
    )

    #Rosbag positions
    bagpos = ExecuteProcess(
        cmd=[[
            'ros2 bag record -o bags/positions /positions/marker/id_01 '
            '                                  /positions/robot_01/base_link '
            '                                  /positions/robot_01/gr_ref_link '
            '                                  /positions/marker/id_02 '
            '                                  /positions/robot_02/base_link '
            '                                  /positions/robot_02/gr_ref_link '
            '                                  /positions/marker/id_21 '
            '                                  /positions/cube/id_21 '
                                               

        ]],
        shell=True
    )

    bagerror_r1 = ExecuteProcess(
        cmd=[[
            'ros2 bag record -o bags/error_r1 /robot_01/plot_data '

        ]],
        shell=True
    )

    bagerror_r2 = ExecuteProcess(
        cmd=[[
            'ros2 bag record -o bags/error_r2 /robot_02/plot_data '

        ]],
        shell=True
    )

    bag_state = ExecuteProcess(
        cmd=[[
            'ros2 bag record -o bags/state /robot_01/robot_state /robot_02/robot_state '

        ]],
        shell=True
    )

    return LaunchDescription([
        delete1,
        delete2,
        delete3,
        delete4,
        bagpos,
        bagerror_r1,
        bagerror_r2,
        bag_state,
       
    ])