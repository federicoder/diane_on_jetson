import rclpy
from rclpy.node import Node
import sys
from diane_encoder.diane_encoder import Encoder, diane_multinomial_i16
#import pyzed.sl as sl

from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header
#import numpy as np
import sensor_msgs_py.point_cloud2 as pc2

class Resolution:
    def __init__(self, width, height):
        self.width = width
        self.height = height


class DianePublisher(Node):
    def __init__(self, topic_name):
        super().__init__('diane_publisher')
        self.subscription = self.create_subscription(
            PointCloud2,
            "zed/point_cloud",
            self.listener_callback,
            10
        )
        self.get_logger().info(f'Subscribed to topic: {topic_name}')
        self.pub = self.create_publisher(PointCloud2, 'diane_encoded', 10)
        #self.timer = self.create_timer(1.0, self.publish_pointcloud)
    def listener_callback(self, msg):
        # Solo per demo: stampo alcune info della PointCloud2
        self.get_logger().info(
            f"Ricevuta PointCloud2: width={msg.width}, height={msg.height}, fields={len(msg.fields)}, frame_id={msg.header.frame_id}"
        )
        encode_pointcloud_and_republish(msg)
        
        
def encode_pointcloud_and_republish(msg):
    # section to take into account pointlcoud2:
    custom_resolution = Resolution(width=480, height=480)
    encoder = Encoder(custom_resolution)
    # section to encode pointcloud2:
    [output_pc,bytes] = encoder.encode(msg, bw=80000000, fps=120)
    encoded_size = len(bytes)
    
    # section to republish pointcloud2 "encoded":
    
    header = Header()
    header.stamp = self.get_clock().now().to_msg()
    header.frame_id = "zed_camera_center"
    fields = [
        PointField(name='x', offset=0,  datatype=PointField.FLOAT32, count=1),
        PointField(name='y', offset=4,  datatype=PointField.FLOAT32, count=1),
        PointField(name='z', offset=8,  datatype=PointField.FLOAT32, count=1),
    ]
    pc2_msg = pc2.create_cloud(header, fields, points)
    self.pub.publish(pc2_msg)
    self.get_logger().info(f"Published PointCloud2 with {N} points.")

    
    
    

def main(args=None):
    rclpy.init(args=args)

    # Prendo nome topic da argomento o parametro
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--topic', type=str, required=True, help='Nome topic PointCloud2')
    arg = parser.parse_args()

    node = DianePublisher(arg.topic)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

