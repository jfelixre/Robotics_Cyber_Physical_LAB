import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, TimerAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node

def generate_launch_description():
    # ---------------------------------------------------------
    # 1. DEFINICIÓN DE ARGUMENTOS DE LANZAMIENTO
    # ---------------------------------------------------------
    
    # A) Selección del Planificador (Path Finding)
    planner_type_arg = DeclareLaunchArgument(
        'planner',
        default_value='astar',
        description='Choose planner algorithm: astar, reverse, greedy'
    )

    # B) Selección del Controlador (Path Tracking)
    controller_type_arg = DeclareLaunchArgument(
        'controller',
        default_value='geometric',
        description='Choose controller type: geometric, pid'
    )
    
    # C) Timestamp para experimento compartido (✨ DETECCIÓN AUTOMÁTICA)
    # Detecta automáticamente si hay un timestamp de experimento activo
    auto_timestamp = os.environ.get('EXPERIMENT_TIMESTAMP', '')
    experiment_timestamp_arg = DeclareLaunchArgument(
        'experiment_timestamp',
        default_value=auto_timestamp,
        description='Shared timestamp for multi-robot experiments (auto-detected from environment)'
    )
    
    planner_type = LaunchConfiguration('planner')
    controller_type = LaunchConfiguration('controller')
    experiment_timestamp = LaunchConfiguration('experiment_timestamp')

    # ---------------------------------------------------------
    # 2. CONFIGURACIÓN DE RUTAS Y DIRECTORIOS
    # ---------------------------------------------------------
    pkg_project_control_pkg = get_package_share_directory('control_pkg')
    pkg_project_launch_pkg = get_package_share_directory('launch_pkg')
    pkg_project_robot_custom_description = get_package_share_directory('robot_custom_description')
    pkg_project_inv_kinematics_pkg = get_package_share_directory('inv_kinematics_pkg')
    pkg_project_task_pkg = get_package_share_directory('task_pkg')

    # Archivos específicos para ROBOT 03
    bridge_config_file = os.path.join(pkg_project_launch_pkg, 'config', 'bridge_r03.yaml')
    robot_sdf_file = os.path.join(pkg_project_robot_custom_description, 'models', 'robot_03', 'model.sdf')
    robot_urdf_file = os.path.join(pkg_project_robot_custom_description, 'models', 'robot_03', 'model.urdf')

    # ---------------------------------------------------------
    # 3. INFRAESTRUCTURA (SIMULACIÓN Y PUENTES)
    # ---------------------------------------------------------

    gz_robot_spawn = ExecuteProcess(
        cmd=[[
            'ros2 run ros_gz_sim create --args -file "',
            robot_sdf_file,
            '" -name robot_03 -x -1.5 -y 0 -z 0.09'
        ]],
        shell=True
    )

    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': bridge_config_file,
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
            'use_sim_time': True
        }],
        output='screen'
    )

    with open(robot_urdf_file, 'r') as infp:
        robot_desc = infp.read()

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_03',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc}
        ],
        remappings=[
            ('/robot_description', '/robot_description/robot_03'),
        ]
    )

    # ---------------------------------------------------------
    # 4. LÓGICA DE CONTROL (MÁQUINA DE ESTADOS Y CLIENTE)
    # ---------------------------------------------------------

    task_manager = Node(
        package='task_pkg',
        namespace='robot_03',
        executable='task_manager_node_client',
        parameters=[{'robot_id': 3, 'use_sim_time': False}],
    )
    
    event_driven_control = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='event_driven_control_refactored',
        parameters=[{'robot_id': 3, 'use_sim_time': False}],
    )
    
    compute_trajectory = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='compute_trajectory',
        parameters=[
            {'robot_id': 3, 'use_sim_time': True},
            {'planner_name': planner_type} 
        ],
        output='screen'
    )
    
    # ---------------------------------------------------------
    # 5. SERVIDORES DE PLANIFICACIÓN (PATH FINDING) - CONDICIONALES
    # ---------------------------------------------------------

    a_star_server = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='a_star_server',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'astar'"])
        ),
        output='screen'
    )

    reverse_a_star_server = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='reverse_a_star_server',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'reverse'"])
        ),
        output='screen'
    )

    greedy_server = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='greedy_server',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'greedy'"])
        ),
        output='screen'
    )

    # ---------------------------------------------------------
    # 6. NODOS DE SEGUIMIENTO (CONTROLLERS) - CONDICIONALES
    # ---------------------------------------------------------

    control_trajectory_geometric = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='control_trajectory_d',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", controller_type, "' == 'geometric'"])
        ),
        output='screen'
    )

    control_trajectory_pid = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='pid_control_trajectory_node',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", controller_type, "' == 'pid'"])
        ),
        output='screen'
    )
    
    # ---------------------------------------------------------
    # 7. CINEMÁTICA Y ESTADO
    # ---------------------------------------------------------

    robot_platform_vel = Node(
        package='inv_kinematics_pkg',
        namespace='robot_03',
        executable='robot_platform_vel_node',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
    )
    
    arm_position = Node(
        package='inv_kinematics_pkg',
        namespace='robot_03',
        executable='arm_position_node',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
    )
    
    robot_state_service = Node(
        package='control_pkg',
        namespace='robot_03',
        executable='robot_state_service',
        parameters=[{'robot_id': 3, 'use_sim_time': True}],
    )
    
    # ---------------------------------------------------------
    # 8. ACCIONES TEMPORIZADAS (SOLTAR CUBOS)
    # ---------------------------------------------------------
    
    detach11 = TimerAction(
        period=10.0, 
        actions=[ExecuteProcess(cmd=[[f'ros2 topic pub --once /robot_03/cube_11/detach std_msgs/msg/Empty']], shell=True)]
    )

    detach12 = TimerAction(
        period=13.0, 
        actions=[ExecuteProcess(cmd=[[f'ros2 topic pub --once /robot_03/cube_12/detach std_msgs/msg/Empty']], shell=True)]
    )

    detach21 = TimerAction(
        period=16.0, 
        actions=[ExecuteProcess(cmd=[[f'ros2 topic pub --once /robot_03/cube_21/detach std_msgs/msg/Empty']], shell=True)]
    )

    # ---------------------------------------------------------
    # 9. DATA LOGGING - DESHABILITADO (Robot 01 maneja todos los datos)  
    # ---------------------------------------------------------
    # robot_data_logger = Node(
    #     package='control_pkg',
    #     executable='robot_data_logger.py',
    #     name='robot_03_data_logger',
    #     parameters=[{
    #         'experiment_name': 'multi_robot_exp',
    #         'robots': 1,
    #         'robot_offset': 3,
    #         'output_dir': '/home/javierfr/Robotics_Cyber_Physical_LAB/csv/',
    #         'log_rate': 10.0,
    #         'use_timestamp': True,
    #         'shared_timestamp': experiment_timestamp
    #     }],
    #     output='screen'
    # )

    # ---------------------------------------------------------
    # 10. RETORNO FINAL
    # ---------------------------------------------------------
    return LaunchDescription([
        planner_type_arg,
        controller_type_arg,
        experiment_timestamp_arg,
        gz_robot_spawn,
        bridge,
        robot_state_publisher,
        task_manager,
        event_driven_control,
        compute_trajectory,
        a_star_server,
        reverse_a_star_server,
        greedy_server,
        control_trajectory_geometric,
        control_trajectory_pid,
        robot_platform_vel,
        arm_position,
        robot_state_service,
        # robot_data_logger,  # DESHABILITADO - Robot 01 maneja todos
        detach11,
        detach12,
        detach21,
    ])
