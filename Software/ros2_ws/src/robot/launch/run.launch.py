import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument
import xacro

def generate_launch_description():
    # Use sim time true if using a simulator (Gazebo)
    use_sim_time = LaunchConfiguration('use_sim_time', default='false')

    # Process the URDF file robot.urdf.xacro 
    xacro_file = os.path.join('ros2_ws/src/robot', 'urdf','snower.urdf.xacro')
    robot_description_config = xacro.process_file(xacro_file)
    
    # Create a robot_state_publisher node
    params = {'robot_description': robot_description_config.toxml(), 'use_sim_time': use_sim_time}
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[params]
    )

    return LaunchDescription([
        Node(
            package='robot',
            executable='robot_debug',
            name='robot_debug',
            output='screen'
        ),
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use sim time true if using a simulator (Gazebo)'
        ),
        node_robot_state_publisher,
        Node(
            package='robot_localization',
            executable='ekf_node',
            name='ekf_filter_node',
            output='screen',
            parameters=['ros2_ws/src/robot/config/robot_localization.yaml'],
            remappings=[('/odometry/filtered', '/odom')]
        ),
        Node(
            package='robot',
            executable='wheel_odom_publisher',
            name='wheel_odom_publisher',
            output='screen'
        ),
    ])
