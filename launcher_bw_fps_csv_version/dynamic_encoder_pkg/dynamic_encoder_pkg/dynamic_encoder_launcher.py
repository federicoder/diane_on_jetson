#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import subprocess
import shlex
import re
import time
import argparse

def parse_args():
    parser = argparse.ArgumentParser(description="Launcher dinamico per settaggio parametri encoder da ros2 topic bw.")
    parser.add_argument('--encoder-node', type=str, default='/point_cloud_encoder', help='Nome del nodo encoder da configurare')
    parser.add_argument('--param-bandwidth', type=str, default='pointcloud_diane.bandwidth', help='Nome parametro bandwidth')
    parser.add_argument('--param-fps', type=str, default='pointcloud_diane.fps', help='Nome parametro fps')
    parser.add_argument('--bw-topic', type=str, default='/pointcloud_diane', help='Topic per ros2 topic bw')
    parser.add_argument('--period', type=float, default=2.0, help='Periodo in secondi')
    return parser.parse_args()

class DynamicEncoderLauncher(Node):
    def __init__(self, encoder_node, param_bandwidth, param_fps, bw_topic, period):
        super().__init__('dynamic_encoder_launcher')
        self.encoder_node = encoder_node
        self.param_bandwidth = param_bandwidth
        self.param_fps = param_fps
        self.bw_topic = bw_topic
        self.period = period

        self.timer = self.create_timer(self.period, self.timer_callback)
    def timer_callback(self):
        try:
            # Esegui il comando ros2 topic bw (solo una misurazione)
            cmd = f"ros2 topic bw {shlex.quote(self.bw_topic)}"
            self.get_logger().info(f"Eseguo: {cmd}")
            proc = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
            # Dormi per self.period secondi così ros2 topic bw riceve almeno qualche messaggio
            time.sleep(self.period)
            proc.terminate()
            output, _ = proc.communicate()
            self.get_logger().info(f"Output ros2 topic bw:\n{output}")
            # Estrai bandwidth e fps
            # Tipico output contiene: "Average bandwidth: 90000.0 Bps\nAverage rate: 120.0 Hz"
            matches = re.findall(r'([\d.]+) MB/s from ([\d.]+) messages', output)
            if matches:
                # Prendi l'ultimo valore letto
                bandwidth_mb_s, num_messages = matches[-1]
                bandwidth = float(bandwidth_mb_s) * 1_000_000  # MB/s -> Byte/s
                fps = float(num_messages) / self.period  # messaggi ricevuti in self.period secondi
                self.get_logger().info(f"Setting {self.param_bandwidth}={bandwidth}, {self.param_fps}={fps}")
                # Set parametri tramite ros2 param set
                subprocess.run(
                    ['ros2', 'param', 'set', self.encoder_node, self.param_bandwidth, str(bandwidth)],
                    check=False
                )
                subprocess.run(
                    ['ros2', 'param', 'set', self.encoder_node, self.param_fps, str(fps)],
                    check=False
                )
            else:
                self.get_logger().warn("Impossibile estrarre bandwidth/fps dall'output.")
        except Exception as e:
            self.get_logger().error(f"Errore durante la misurazione o il settaggio parametri: {e}")

def main(args=None):
    cli_args = parse_args()
    rclpy.init(args=args)
    node = DynamicEncoderLauncher(
        cli_args.encoder_node,
        cli_args.param_bandwidth,
        cli_args.param_fps,
        cli_args.bw_topic,
        cli_args.period
    )
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()

