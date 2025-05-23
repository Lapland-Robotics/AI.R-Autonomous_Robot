import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    return LaunchDescription([
        Node(
            package='data_collector',
            executable='data_collector',
            name='data_collector',
            output='screen'
        ), 
        Node(
            package='data_collector',
            executable='thermal_camera_publisher',
            name='thermal_camera_publisher',
            output='screen'
        ),
    ])
