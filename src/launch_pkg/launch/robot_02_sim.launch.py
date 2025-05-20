import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.actions import ExecuteProcess, TimerAction
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

    # Spawn robot in Gazebo
    file = os.path.join(
        pkg_project_robot_custom_description,
        'models',
        'robot_02',
        'model.sdf'
    )
    #Command to spawn robot in Gazebo on an especific position
    gz_robot_spawn = ExecuteProcess(
        cmd=[[
            'ros2 run ros_gz_sim create --args -file "',
            file,
            '" -name robot_02 -x -1 -y -1 -z 0.09'

        ]],
        shell=True
    )


    # Bridge ROS topics and Gazebo messages for establishing communication
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_project_launch_pkg, 'config', 'bridge_r02.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )

    # Load the SDF file from "description" package
    sdf_file  =  os.path.join(pkg_project_robot_custom_description, 'models', 'robot_02', 'model.urdf')
    with open(sdf_file, 'r') as infp:
        robot_desc = infp.read()

    # Takes the description and joint angles as inputs and publishes the 3D poses of the robot links
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_02',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc}
        ],
        remappings=[
            ('/robot_description', '/robot_description/robot_02'),
        ]
    )

    #Launch task manager node for robot_02
    task_manager = Node(
        package='task_pkg',
        namespace='robot_02',
        executable='task_manager_node_client',
        parameters=[{'robot_id': 2}],
        )
    
    #Launch event driven control node for robot_02
    event_driven_control = Node(
        package='control_pkg',
        namespace='robot_02',
        executable='event_driven_control_refactored',
        parameters=[{'robot_id': 2}],
        )
    
        #Launch compute_trajectory node for robot_02
    compute_trajectory = Node(
            package='control_pkg',
            namespace='robot_02',
            executable='compute_trajectory',
            parameters=[{'robot_id': 2}],
            #arguments=['--ros-args', '--log-level', 'debug'],
            )
    
    #Launch a_star_server node for robot_02
    a_star_server = Node(
            package='control_pkg',
            namespace='robot_02',
            executable='a_star_server',
            parameters=[{'robot_id': 2}],
            )
    
    #Launch control trajectory node for robot_02
    control_trajectory = Node(
            package='control_pkg',
            namespace='robot_02',
            executable='control_trajectory_d',
            parameters=[{'robot_id': 2}],
            )
    
    #Launch robot platform vel node for robot_02
    robot_platform_vel = Node(
            package='inv_kinematics_pkg',
            namespace='robot_02',
            executable='robot_platform_vel_node',
            parameters=[{'robot_id': 2}],
            )
    
    #Launch arm position node for robot_02
    arm_position = Node(
            package='inv_kinematics_pkg',
            namespace='robot_02',
            executable='arm_position_node',
            parameters=[{'robot_id': 2}],
            )
    
    #launch robot_state_service node for robot_02
    robot_state_service = Node(
            package='control_pkg',
            namespace='robot_02',
            executable='robot_state_service',
            parameters=[{'robot_id': 2}],
            )
    
    #Detach Cube from robot
    detach11 = TimerAction(
        period=10.0, #Delay in seconds
        actions=[
            ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_02/cube_11/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
        ]
    )

    detach12 = TimerAction(
        period=13.0, #Delay in seconds
        actions=[
            ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_02/cube_12/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
        ]
    )

    detach21 = TimerAction(
        period=16.0, #Delay in seconds
        actions=[
            ExecuteProcess(
                cmd=[[
                    'ros2 topic pub --once /robot_02/cube_21/detach std_msgs/msg/Empty',
                    ]],
                    shell=True
            )
        ]
    )




    return LaunchDescription([
        task_manager,
        gz_robot_spawn,
        bridge,
        robot_state_publisher,
        event_driven_control,
        compute_trajectory,
        a_star_server,
        control_trajectory,
        #robot_platform_vel,
        arm_position,
        #robot_state_service,
        detach11,
        detach12,
        detach21,
       
    ])