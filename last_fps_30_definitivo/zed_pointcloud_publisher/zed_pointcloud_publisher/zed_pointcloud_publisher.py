import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header
import numpy as np
import sensor_msgs_py.point_cloud2 as pc2
import pyzed.sl as sl
import time
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy
class ZedPointCloudPublisher(Node):
    def __init__(self):
        super().__init__('zed_pointcloud_publisher')
        # Setting QoS:
        qos_profile = QoSProfile(reliability=ReliabilityPolicy.RELIABLE, history=HistoryPolicy.KEEP_LAST, depth=15)
        # Publisher PointCloud2
        self.pc_pub = self.create_publisher(PointCloud2, 'zed/point_cloud', qos_profile)

        # ZED Initialization
        self.zed = sl.Camera()
        init_params = sl.InitParameters()
        init_params.camera_resolution = sl.RESOLUTION.HD720
        init_params.camera_fps = 30
        init_params.depth_mode = sl.DEPTH_MODE.PERFORMANCE
        init_params.coordinate_units = sl.UNIT.METER

        status = self.zed.open(init_params)
        if status != sl.ERROR_CODE.SUCCESS:
            self.get_logger().error(f"Impossibile aprire ZED: {status}")
            exit(1)

        # Variabili per il calcolo delle prestazioni
        self.last_time = time.time()
        self.frame_count = 0
        self.avg_fps = 0.0
        self.avg_process_time = 0.0
        self.timer = self.create_timer(1.0 / 30, self.timer_callback)
        # self.timer = self.create_timer(1.0 / 15, self.timer_callback)  # 15 Hz

    def timer_callback(self):
        start_time = time.time()
        
        runtime_parameters = sl.RuntimeParameters()
        if self.zed.grab(runtime_parameters) == sl.ERROR_CODE.SUCCESS:
            point_cloud = sl.Mat()
            
            # Imposta risoluzione 480x480
            resolution = sl.Resolution()
            resolution.width = 480
            resolution.height = 480
            
            # Recupera la point cloud
            err = self.zed.retrieve_measure(
                point_cloud, 
                sl.MEASURE.XYZRGBA, 
                sl.MEM.CPU,
                resolution
            )
            
            if err != sl.ERROR_CODE.SUCCESS:
                self.get_logger().error(f"Errore nel retrieve_measure: {err}")
                return
    
            # Converti in array numpy
            array = point_cloud.get_data()
            
            #height, width, _ = array.shape
            points = array.reshape(-1, 4)
            
            # Filtro punti validi
            #mask = ~np.isnan(points[:, 0])
            #points = points[mask] 
            
            # Costruzione del messaggio PointCloud2
            header = Header()
            header.stamp = self.get_clock().now().to_msg()
            header.frame_id = "zed_camera_center"
            
            fields = [
                PointField(name='x', offset=0, datatype=PointField.FLOAT32, count=1),
                PointField(name='y', offset=4, datatype=PointField.FLOAT32, count=1),
                PointField(name='z', offset=8, datatype=PointField.FLOAT32, count=1),
                PointField(name='rgba', offset=12, datatype=PointField.FLOAT32, count=1)
            ]
            
            pc2_msg = pc2.create_cloud(header, fields, points)
            self.pc_pub.publish(pc2_msg)

            # Calcolo prestazioni
            process_time = time.time() - start_time
            self.frame_count += 1
            
            # Calcola media mobile
            self.avg_process_time = 0.9 * self.avg_process_time + 0.1 * process_time
            
            # Stampa statistiche ogni secondo
            if time.time() - self.last_time >= 1.0:
                self.avg_fps = self.frame_count / (time.time() - self.last_time)
                self.get_logger().info(
                    f"FPS: {self.avg_fps:.2f} | "
                    f"Tempo elaborazione: {self.avg_process_time*1000:.2f}ms | "
                    f"Punti: {len(points)}"
                )
                self.frame_count = 0
                self.last_time = time.time()

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
