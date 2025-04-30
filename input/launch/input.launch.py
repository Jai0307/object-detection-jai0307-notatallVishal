from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='input',
            executable='input_monitor_node',
            output='screen'),
    ])