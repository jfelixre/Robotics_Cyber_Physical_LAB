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

    #Load yaml parameters
    config = os.path.join(
    pkg_project_launch_pkg,
    'config',
    'params.yaml'
    )

    # Spawn cube in Gazebo
    file = os.path.join(
        pkg_project_robot_custom_description,
        'models',
        'single_cube_01',
        'model.sdf'
    )
    #Command to spawn cube in Gazebo on an especific position
    gz_cube_spawn = ExecuteProcess(
        cmd=[[
            'ros2 run ros_gz_sim create --args -file "',
            file,
            '" -name single_cube_01 -x 1 -y -1 -z 0.09'

        ]],
        shell=True
    )


    # Bridge ROS topics and Gazebo messages for establishing communication
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_project_launch_pkg, 'config', 'bridge_s_c_01.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )

    # Load the SDF file from "description" package
    sdf_file  =  os.path.join(pkg_project_robot_custom_description, 'models', 'single_cube_01', 'model.urdf')
    with open(sdf_file, 'r') as infp:
        cube_desc = infp.read()

    # Takes the description and joint angles as inputs and publishes the 3D poses of the cube links
    cube_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='single_cube_01',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': cube_desc}
        ],
        remappings=[
            ('/robot_description', '/robot_description/single_cube_01'),
        ]
    )


    return LaunchDescription([
        gz_cube_spawn,
        bridge,
        cube_state_publisher,
       
    ])