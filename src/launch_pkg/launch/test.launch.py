from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Nodo compute_trajectory para Robot 1
        Node(
            package='task_pkg',
        namespace='robot_01',
        executable='task_manager_node_client',
        parameters=[{'robot_id': 1, 'use_sim_time': False}],
        ),
    ])
