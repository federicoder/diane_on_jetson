from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    encoder = Node(
        package='point_cloud_transport',
        executable='republish',
        name='diane_encoder',
        parameters=[{
            'qos.reliability': 'best_effort',
            'qos.durability': 'volatile',
            'pointcloud_diane.bandwidth': 8e9,
            'pointcloud_diane.fps': 30.0
        }],
        arguments=[
            '--out_transport', 'diane',
            '--in_transport', 'raw',
            '--ros-args',
            '--remap', 'in:=/zed/point_cloud',
            '--remap', 'out/diane:=/pointcloud_diane'
        ]
    )

    decoder = Node(
        package='point_cloud_transport',
        executable='republish',
        name='diane_decoder',
        parameters=[{
            'qos.reliability': 'best_effort',
            'qos.durability': 'volatile',
            'pointcloud_diane.bandwidth': 8e9,
            'pointcloud_diane.fps': 120.0
        }],
        arguments=[
            '--out_transport', 'raw',
            '--in_transport', 'diane',
            '--ros-args',
            '--remap', 'in/diane:=/pointcloud_diane',
            '--remap', 'out:=/decoded'
        ]
    )

    return LaunchDescription([encoder, decoder])
