from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Nodo compute_trajectory para Robot 1
        Node(
            package='control_pkg',
            executable='compute_trajectory',
            name='compute_trajectory',
            namespace='robot_01',
            output='screen',
            parameters=[{'robot_id': 1}],
        ),
    ])
