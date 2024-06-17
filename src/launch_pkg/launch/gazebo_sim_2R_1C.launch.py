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

    # Setup to launch the simulator and Gazebo world
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
        launch_arguments={'gz_args': PathJoinSubstitution([
            pkg_project_gazebo_plugin_sim,
            'worlds',
            'world_cam.sdf'
        ])}.items(),
    )

    # Bridge ROS topics and Gazebo messages for establishing communication
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_project_launch_pkg, 'config', 'bridge.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )

    #Unpause simulation
    bridge_unpause = ExecuteProcess(
        cmd=[[
            'ros2 run ros_gz_bridge parameter_bridge /world/world_cam/control@ros_gz_interfaces/srv/ControlWorld'
        ]],
        shell=True
    )

    unpause = ExecuteProcess(
        cmd=[[
            'ros2 service call /world/world_cam/control ros_gz_interfaces/srv/ControlWorld "{world_control: {pause: false}}"'
        ]],
        shell=True
    )

    #Launch robot_01
    launch_robot_01 = ExecuteProcess(
        cmd=[[
            'ros2 launch launch_pkg robot_01_sim.launch.py'
        ]],
        shell=True
    )

    #Launch robot_02
    launch_robot_02 = ExecuteProcess(
        cmd=[[
            'ros2 launch launch_pkg robot_02_sim.launch.py'
        ]],
        shell=True
    )

    #Launch cube_id_11
    launch_cube_id_11 = ExecuteProcess(
        cmd=[[
            'ros2 launch launch_pkg cube_id_11_sim.launch.py'
        ]],
        shell=True
    )
    
    #Launch cube_id_12
    launch_cube_id_12 = ExecuteProcess(
        cmd=[[
            'ros2 launch launch_pkg cube_id_12_sim.launch.py'
        ]],
        shell=True
    )

    #Launch cube_id_21
    launch_cube_id_21 = ExecuteProcess(
        cmd=[[
            'ros2 launch launch_pkg cube_id_21_sim.launch.py'
        ]],
        shell=True
    )

    #Launch image processing nodes
    img_proc_nodes = ExecuteProcess(
        cmd=[[
            'ros2 launch launch_pkg img_proc_nodes.launch.py'
        ]],
        shell=True
    )

    #Launch task scheduler node
    task_scheduler = ExecuteProcess(
        cmd=[[
            'ros2 run task_pkg task_scheduler_node'
        ]],
        shell=True
    )

    return LaunchDescription([
        gz_sim,
        bridge,
        bridge_unpause,
        unpause,
        #launch_robot_01,
        launch_robot_02,
        img_proc_nodes,
        task_scheduler,
        launch_cube_id_11,
        launch_cube_id_12,
        launch_cube_id_21,
        
    ])