from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='action_client',
            executable='track_object_client',
            output='screen'),
    ])