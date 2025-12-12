import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.actions import ExecuteProcess, TimerAction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, PythonExpression

from launch_ros.actions import Node

def generate_launch_description():
    # 1. ARGUMENTO DE SELECCIÓN DE PLANIFICADOR
    # Opciones válidas: 'astar', 'reverse', 'greedy'
    # Por defecto usa 'astar'
    planner_type_arg = DeclareLaunchArgument(
        'planner',
        default_value='astar',
        description='Choose planner algorithm: astar, reverse, greedy'
        #Lanzar como:
        #ros2 launch launch_pkg robot_01_sim.launch.py planner:=astar
        #ros2 launch launch_pkg robot_01_sim.launch.py planner:=reverse
        #ros2 launch launch_pkg robot_01_sim.launch.py planner:=greedy
    )
    
    # Capturar el valor de la configuración
    planner_type = LaunchConfiguration('planner')

    # Setup project paths
    pkg_project_control_pkg = get_package_share_directory('control_pkg')
    pkg_project_gazebo_plugin_sim = get_package_share_directory('gazebo_plugin_sim')
    pkg_project_img_proc_pkg = get_package_share_directory('img_proc_pkg')
    pkg_project_interfaces = get_package_share_directory('interfaces')
    pkg_project_inv_kinematics_pkg = get_package_share_directory('inv_kinematics_pkg')
    pkg_project_launch_pkg = get_package_share_directory('launch_pkg')
    pkg_project_robot_custom_description = get_package_share_directory('robot_custom_description')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

    # Load yaml parameters
    config = os.path.join(
        pkg_project_launch_pkg,
        'config',
        'params.yaml'
    )

    # Spawn robot in Gazebo
    file = os.path.join(
        pkg_project_robot_custom_description,
        'models',
        'robot_01',
        'model.sdf'
    )
    
    gz_robot_spawn = ExecuteProcess(
        cmd=[[
            'ros2 run ros_gz_sim create --args -file "',
            file,
            '" -name robot_01 -x -1 -y 1 -z 0.09'
        ]],
        shell=True
    )

    # Bridge
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_project_launch_pkg, 'config', 'bridge_r01.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )

    # Robot State Publisher
    sdf_file = os.path.join(pkg_project_robot_custom_description, 'models', 'robot_01', 'model.urdf')
    with open(sdf_file, 'r') as infp:
        robot_desc = infp.read()

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_01',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc}
        ],
        remappings=[
            ('/robot_description', '/robot_description/robot_01'),
        ]
    )

    # Task Manager
    task_manager = Node(
        package='task_pkg',
        namespace='robot_01',
        executable='task_manager_node_client',
        parameters=[{'robot_id': 1}],
    )
    
    # Event Driven Control
    event_driven_control = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='event_driven_control_refactored',
        parameters=[{'robot_id': 1}],
    )
    
    # --- NODO CLIENTE (Trajectory) ---
    # Le pasamos el nombre del planner para que los logs sean correctos
    compute_trajectory = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='compute_trajectory',
        parameters=[
            {'robot_id': 1},
            {'planner_name': planner_type} # <--- Pasa el argumento al nodo
        ],
        output='screen'
    )
    
    # --- SERVIDORES DE PATH FINDING (CONDICIONALES) ---

    # 1. A* Clásico (Se ejecuta si planner == 'astar')
    a_star_server = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='a_star_server',
        parameters=[{'robot_id': 1}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'astar'"])
        ),
        output='screen'
    )

    # 2. Reverse A* (Se ejecuta si planner == 'reverse')
    reverse_a_star_server = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='reverse_a_star_server',
        parameters=[{'robot_id': 1}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'reverse'"])
        ),
        output='screen'
    )

    # 3. Greedy Best-First (Se ejecuta si planner == 'greedy')
    greedy_server = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='greedy_server',
        parameters=[{'robot_id': 1}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'greedy'"])
        ),
        output='screen'
    )
    
    # --- OTROS NODOS DE CONTROL ---

    control_trajectory = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='control_trajectory_d',
        parameters=[{'robot_id': 1}],
    )
    
    robot_platform_vel = Node(
        package='inv_kinematics_pkg',
        namespace='robot_01',
        executable='robot_platform_vel_node',
        parameters=[{'robot_id': 1}],
    )
    
    arm_position = Node(
        package='inv_kinematics_pkg',
        namespace='robot_01',
        executable='arm_position_node',
        parameters=[{'robot_id': 1}],
    )
    
    robot_state_service = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='robot_state_service',
        parameters=[{'robot_id': 1}],
    )
    
    # --- ACCIONES TEMPORIZADAS (SOLTAR CUBOS) ---
    
    detach11 = TimerAction(
        period=10.0, 
        actions=[ExecuteProcess(cmd=[['ros2 topic pub --once /robot_01/cube_11/detach std_msgs/msg/Empty']], shell=True)]
    )

    detach12 = TimerAction(
        period=13.0, 
        actions=[ExecuteProcess(cmd=[['ros2 topic pub --once /robot_01/cube_12/detach std_msgs/msg/Empty']], shell=True)]
    )

    detach21 = TimerAction(
        period=16.0, 
        actions=[ExecuteProcess(cmd=[['ros2 topic pub --once /robot_01/cube_21/detach std_msgs/msg/Empty']], shell=True)]
    )

    detach_robots = TimerAction(
        period=5.0,
        actions=[ExecuteProcess(cmd=[['ros2 topic pub --once /R01R02/detach std_msgs/msg/Empty']], shell=True)]
    )

    return LaunchDescription([
        planner_type_arg, # <--- IMPORTANTE: Registrar el argumento
        task_manager,
        gz_robot_spawn,
        bridge,
        robot_state_publisher,
        event_driven_control,
        
        compute_trajectory, # Cliente
        
        # Servidores (Solo uno se activará)
        a_star_server,
        reverse_a_star_server,
        greedy_server,
        
        control_trajectory,
        robot_platform_vel,
        arm_position,
        # robot_state_service,
        
        detach11,
        detach12,
        detach21,
        detach_robots,
    ])