from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Nodo A* Server para Robot 01
        Node(
            package='control_pkg',
            executable='a_star_server',
            name='a_star_server',
            namespace='robot_01',
            output='screen',
            parameters=[{'robot_id': 1}],
        ),
    ])
