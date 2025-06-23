#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import PointCloud2
from rclpy.serialization import serialize_message

import time
import argparse
from collections import deque
import subprocess

def parse_args():
    parser = argparse.ArgumentParser(
        description="Launcher dinamico per allineare bandwidth e fps del pointcloud encoder ai valori effettivi di /decoded, con banda minima garantita."
    )
    parser.add_argument(
        '--encoder-node',
        type=str,
        default='/point_cloud_encoder',
        help='Nome del nodo encoder da configurare'
    )
    parser.add_argument(
        '--param-bandwidth',
        type=str,
        default='pointcloud_diane.bandwidth',
        help='Nome parametro bandwidth'
    )
    parser.add_argument(
        '--param-fps',
        type=str,
        default='pointcloud_diane.fps',
        help='Nome parametro fps'
    )
    parser.add_argument(
        '--topic-bw',
        type=str,
        default='/decoded',
        help='Topic su cui misurare banda effettiva'
    )
    parser.add_argument(
        '--topic-hz',
        type=str,
        default='/decoded',
        help='Topic su cui misurare fps effettivi'
    )
    parser.add_argument(
        '--period',
        type=float,
        default=2.0,
        help='Periodo in secondi del timer di misura'
    )
    parser.add_argument(
        '--update-interval',
        type=float,
        default=10.0,
        help='Intervallo in secondi tra aggiornamenti dei parametri'
    )
    parser.add_argument(
        '--min-bandwidth',
        type=float,
        default=1e6,
        help='Valore minimo di banda (bytes/s) da garantire al compressore'
    )
    return parser.parse_args()


class DynamicEncoderLauncher(Node):
    def __init__(self, args):
        super().__init__('dynamic_encoder_launcher')
        self.encoder_node = args.encoder_node
        self.param_bandwidth = args.param_bandwidth
        self.param_fps = args.param_fps
        self.topic_bw = args.topic_bw
        self.topic_hz = args.topic_hz
        self.period = args.period
        self.update_interval = args.update_interval
        self.min_bandwidth = args.min_bandwidth

        # Deque per misure su finestra di update_interval
        self.bw_times = deque()
        self.bw_sizes = deque()
        self.hz_times = deque()

        # QoS per subscriber (best_effort per pointcloud)
        qos = QoSProfile(depth=10, reliability=ReliabilityPolicy.BEST_EFFORT)

        # Subscriber per banda (misura size e, se topic coincide, anche fps)
        self.create_subscription(
            PointCloud2,
            self.topic_bw,
            self.on_bw_message,
            qos
        )
        # Se topic diversi, aggiungo un secondo subscriber solo per fps
        if self.topic_hz != self.topic_bw:
            self.create_subscription(
                PointCloud2,
                self.topic_hz,
                self.on_hz_message,
                qos
            )

        # Timer per aggiornamenti
        self.last_update = time.time()
        self.timer = self.create_timer(self.period, self.timer_callback)
        self.get_logger().info("DynamicEncoderLauncher avviato.")

    def on_bw_message(self, msg: PointCloud2):
        t = time.time()
        size = len(serialize_message(msg))
        self.bw_times.append(t)
        self.bw_sizes.append(size)
        if self.topic_bw == self.topic_hz:
            self.hz_times.append(t)

    def on_hz_message(self, msg: PointCloud2):
        t = time.time()
        self.hz_times.append(t)

    def timer_callback(self):
        now = time.time()
        cutoff = now - self.update_interval

        # Purga dati più vecchi di update_interval
        while self.bw_times and self.bw_times[0] < cutoff:
            self.bw_times.popleft()
            self.bw_sizes.popleft()
        while self.hz_times and self.hz_times[0] < cutoff:
            self.hz_times.popleft()

        # Verifica se è il momento di aggiornare
        if now - self.last_update < self.update_interval:
            return

        window = self.update_interval
        if not self.bw_sizes or not self.hz_times:
            self.get_logger().warn("Nessun dato utile raccolto nel periodo di aggiornamento.")
            self.last_update = now
            return

        avg_bw = sum(self.bw_sizes) / window
        avg_fps = len(self.hz_times) / window
        bw_to_set = max(avg_bw, self.min_bandwidth)

        self.get_logger().info(
            f"[ultimi {window:.1f}s] bw={avg_bw/1e6:.2f} MB/s, "
            f"fps={avg_fps:.2f} Hz → imposto bw={bw_to_set/1e6:.2f} MB/s"
        )

        # Qui il fix: passo un float in stringa, no int()
        subprocess.run([
            'ros2', 'param', 'set',
            self.encoder_node,
            self.param_bandwidth,
            f"{bw_to_set:.2f}"
        ], check=False)
        subprocess.run([
            'ros2', 'param', 'set',
            self.encoder_node,
            self.param_fps,
            f"{avg_fps:.2f}"
        ], check=False)

        # Reset stato
        self.bw_times.clear()
        self.bw_sizes.clear()
        self.hz_times.clear()
        self.last_update = now

    def destroy_node(self):
        self.get_logger().info("DynamicEncoderLauncher terminato.")
        super().destroy_node()


def main(args=None):
    cli_args = parse_args()
    rclpy.init(args=args)
    node = DynamicEncoderLauncher(cli_args)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()

