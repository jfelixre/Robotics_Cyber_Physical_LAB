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
    # Opciones: 'astar', 'reverse', 'greedy'
    planner_type_arg = DeclareLaunchArgument(
        'planner',
        default_value='astar',
        description='Choose planner algorithm: astar, reverse, greedy'
    )

    # B) Selección del Controlador (Path Tracking)
    # Opciones: 'geometric' (control_trajectory_d original), 'pid' (Nuevo PID)
    controller_type_arg = DeclareLaunchArgument(
        'controller',
        default_value='geometric',
        description='Choose controller type: geometric, pid'
    )
    
    # Capturar valores de configuración
    planner_type = LaunchConfiguration('planner')
    controller_type = LaunchConfiguration('controller')

    # ---------------------------------------------------------
    # 2. CONFIGURACIÓN DE RUTAS Y DIRECTORIOS
    # ---------------------------------------------------------
    pkg_project_control_pkg = get_package_share_directory('control_pkg')
    pkg_project_launch_pkg = get_package_share_directory('launch_pkg')
    pkg_project_robot_custom_description = get_package_share_directory('robot_custom_description')
    pkg_project_inv_kinematics_pkg = get_package_share_directory('inv_kinematics_pkg')
    pkg_project_task_pkg = get_package_share_directory('task_pkg')

    # Archivos específicos para ROBOT 01
    bridge_config_file = os.path.join(pkg_project_launch_pkg, 'config', 'bridge_r01.yaml')
    robot_sdf_file = os.path.join(pkg_project_robot_custom_description, 'models', 'robot_01', 'model.sdf')
    robot_urdf_file = os.path.join(pkg_project_robot_custom_description, 'models', 'robot_01', 'model.urdf')

    # ---------------------------------------------------------
    # 3. INFRAESTRUCTURA (SIMULACIÓN Y PUENTES)
    # ---------------------------------------------------------

    # Spawn del ROBOT 01 en Gazebo
    gz_robot_spawn = ExecuteProcess(
        cmd=[[
            'ros2 run ros_gz_sim create --args -file "',
            robot_sdf_file,
            '" -name robot_01 -x -1 -y 1 -z 0.09'
        ]],
        shell=True
    )

    # Puente ROS2 - Gazebo
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

    # Robot State Publisher
    with open(robot_urdf_file, 'r') as infp:
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

    # ---------------------------------------------------------
    # 4. LÓGICA DE CONTROL (MÁQUINA DE ESTADOS Y CLIENTE)
    # ---------------------------------------------------------

    task_manager = Node(
        package='task_pkg',
        namespace='robot_01',
        executable='task_manager_node_client',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
    )
    
    event_driven_control = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='event_driven_control_refactored',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
    )
    
    # Cliente de Trayectoria
    compute_trajectory = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='compute_trajectory',
        parameters=[
            {'robot_id': 1, 'use_sim_time': True},
            {'planner_name': planner_type} 
        ],
        output='screen'
    )
    
    # ---------------------------------------------------------
    # 5. SERVIDORES DE PLANIFICACIÓN (PATH FINDING) - CONDICIONALES
    # ---------------------------------------------------------

    a_star_server = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='a_star_server',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'astar'"])
        ),
        output='screen'
    )

    reverse_a_star_server = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='reverse_a_star_server',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'reverse'"])
        ),
        output='screen'
    )

    greedy_server = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='greedy_server',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", planner_type, "' == 'greedy'"])
        ),
        output='screen'
    )

    # ---------------------------------------------------------
    # 6. NODOS DE SEGUIMIENTO (CONTROLLERS) - CONDICIONALES
    # ---------------------------------------------------------

    # A) Controlador Geométrico Original (Se ejecuta si controller == 'geometric')
    control_trajectory_geometric = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='control_trajectory_d',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
        condition=IfCondition(
            PythonExpression(["'", controller_type, "' == 'geometric'"])
        ),
        output='screen'
    )

    # B) Nuevo Controlador PID (Se ejecuta si controller == 'pid')
    control_trajectory_pid = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='pid_control_trajectory_node',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
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
        namespace='robot_01',
        executable='robot_platform_vel_node',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
    )
    
    arm_position = Node(
        package='inv_kinematics_pkg',
        namespace='robot_01',
        executable='arm_position_node',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
    )
    
    robot_state_service = Node(
        package='control_pkg',
        namespace='robot_01',
        executable='robot_state_service',
        parameters=[{'robot_id': 1, 'use_sim_time': True}],
    )
    
    # ---------------------------------------------------------
    # 8. ACCIONES TEMPORIZADAS (SOLTAR CUBOS)
    # ---------------------------------------------------------
    
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


    # ---------------------------------------------------------
    # 9. RETORNO FINAL
    # ---------------------------------------------------------
    return LaunchDescription([
        # Argumentos
        planner_type_arg,
        controller_type_arg,
        
        # Simulación y TF
        gz_robot_spawn,
        bridge,
        robot_state_publisher,
        
        # Lógica de Tareas
        task_manager,
        event_driven_control,
        compute_trajectory, # Cliente
        
        # Path Finding (Selección condicional)
        a_star_server,
        reverse_a_star_server,
        greedy_server,
        
        # Path Tracking (Selección condicional)
        control_trajectory_geometric,
        control_trajectory_pid,
        
        # Cinemática y Auxiliares
        robot_platform_vel,
        arm_position,
        robot_state_service,
        
        # Timers
        detach11,
        detach12,
        detach21,
    ])