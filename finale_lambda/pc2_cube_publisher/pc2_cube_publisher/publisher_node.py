import rclpy
from rclpy.node import Node

from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header
import sensor_msgs_py.point_cloud2 as pc2
import numpy as np
import struct

class CubePointCloudPublisher(Node):
    def __init__(self):
        super().__init__('cube_pc2_publisher')
        self.pub = self.create_publisher(PointCloud2, 'cube_cloud', 10)

        # Cubo: 100k punti, lato 1m
        self.N = 100_000
        self.L = 1.0

        # Punti relativi statici
        self.pts = np.random.rand(self.N, 3) * self.L - (self.L / 2)
        # Colori RGB su z
        colors = ((self.pts[:, 2] + self.L / 2) / self.L * 255).astype(np.uint8)
        self.rgb = np.stack([colors, 255 - colors, np.full_like(colors, 128)], axis=1)

        # PointField
        self.fields = []
        for name, offset in [('x', 0), ('y', 4), ('z', 8), ('rgb', 12)]:
            pf = PointField()
            pf.name = name
            pf.offset = offset
            pf.datatype = PointField.FLOAT32
            pf.count = 1
            self.fields.append(pf)

        # Movimento
        self.x_offset = -7.5  # Partenza a sinistra
        self.velocita = 0.1   # metri a step
        self.timer_period = 0.1  # secondi
        self.direction = 1    # 1 avanti, -1 indietro

        self.x_min = -7.5
        self.x_max = 7.5

        self.timer = self.create_timer(self.timer_period, self.publish_cloud)

    def publish_cloud(self):
        # Update posizione
        self.x_offset += self.velocita * self.direction

        # Inversione ai limiti
        if self.x_offset >= self.x_max:
            self.x_offset = self.x_max
            self.direction = -1
        elif self.x_offset <= self.x_min:
            self.x_offset = self.x_min
            self.direction = 1

        # Applico offset
        pts_offset = self.pts.copy()
        pts_offset[:, 0] += self.x_offset

        # Preparo i dati
        cloud_data = []
        for (x, y, z), (r, g, b) in zip(pts_offset, self.rgb):
            rgb_int = (int(r) << 16) | (int(g) << 8) | int(b)
            rgb_float = struct.unpack('f', struct.pack('I', rgb_int))[0]
            cloud_data.append([x, y, z, rgb_float])

        header = Header(frame_id='world')
        header.stamp = self.get_clock().now().to_msg()
        msg = pc2.create_cloud(header, self.fields, cloud_data)
        self.pub.publish(msg)
        self.get_logger().info(f'PointCloud2 pubblicata (x_offset={self.x_offset:.2f}, dir={self.direction})')

def main(args=None):
    rclpy.init(args=args)
    node = CubePointCloudPublisher()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()

