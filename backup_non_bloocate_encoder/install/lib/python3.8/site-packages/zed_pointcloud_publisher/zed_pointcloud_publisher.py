import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header
import numpy as np
import sensor_msgs_py.point_cloud2 as pc2
import pyzed.sl as sl

class ZedPointCloudPublisher(Node):
    def __init__(self):
        super().__init__('zed_pointcloud_publisher')

        # Publisher PointCloud2
        self.pc_pub = self.create_publisher(PointCloud2, 'zed/point_cloud', 10)

        # ZED Initialization
        self.zed = sl.Camera()
        init_params = sl.InitParameters()
        init_params.camera_resolution = sl.RESOLUTION.HD720
        init_params.depth_mode = sl.DEPTH_MODE.ULTRA
        init_params.coordinate_units = sl.UNIT.METER

        status = self.zed.open(init_params)
        if status != sl.ERROR_CODE.SUCCESS:
            self.get_logger().error(f"Impossibile aprire ZED: {status}")
            exit(1)

        self.timer = self.create_timer(1.0 / 15, self.timer_callback)  # 15 Hz

    def timer_callback(self):
        runtime_parameters = sl.RuntimeParameters()
        if self.zed.grab(runtime_parameters) == sl.ERROR_CODE.SUCCESS:
            # Acquisizione point cloud
            point_cloud = sl.Mat()
            self.zed.retrieve_measure(point_cloud, sl.MEASURE.XYZRGBA, sl.MEM.CPU)
            array = point_cloud.get_data()

            # Riduci la point cloud a un array Nx4
            height, width, _ = array.shape
            points = array.reshape(-1, 4)

            # Filtro punti validi (x != nan)
            mask = ~np.isnan(points[:, 0])
            points = points[mask]

            # Costruzione del messaggio PointCloud2
            header = Header()
            header.stamp = self.get_clock().now().to_msg()
            header.frame_id = "zed_camera_center"

            fields = []

            field_x = PointField()
            field_x.name = 'x'
            field_x.offset = 0
            field_x.datatype = PointField.FLOAT32
            field_x.count = 1

            field_y = PointField()
            field_y.name = 'y'
            field_y.offset = 4
            field_y.datatype = PointField.FLOAT32
            field_y.count = 1

            field_z = PointField()
            field_z.name = 'z'
            field_z.offset = 8
            field_z.datatype = PointField.FLOAT32
            field_z.count = 1

            field_rgba = PointField()
            field_rgba.name = 'rgba'
            field_rgba.offset = 12
            field_rgba.datatype = PointField.FLOAT32
            field_rgba.count = 1

            fields = [field_x, field_y, field_z, field_rgba]

            pc2_msg = pc2.create_cloud(header, fields, points)

            self.pc_pub.publish(pc2_msg)

def main(args=None):
    rclpy.init(args=args)
    node = ZedPointCloudPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.zed.close()
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

