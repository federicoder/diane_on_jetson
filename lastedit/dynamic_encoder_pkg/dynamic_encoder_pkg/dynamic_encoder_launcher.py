#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import subprocess
import shlex
import re
import time
import argparse

def parse_args():
    parser = argparse.ArgumentParser(description="Launcher dinamico per settaggio parametri encoder da ros2 topic bw e hz.")
    parser.add_argument('--encoder-node', type=str, default='/point_cloud_encoder', help='Nome del nodo encoder da configurare')
    parser.add_argument('--param-bandwidth', type=str, default='pointcloud_diane.bandwidth', help='Nome parametro bandwidth')
    parser.add_argument('--param-fps', type=str, default='pointcloud_diane.fps', help='Nome parametro fps')
    parser.add_argument('--topic-decoded', type=str, default='/decoded', help='Topic da cui leggere ros2 topic bw/hz')
    parser.add_argument('--topic-encoded', type=str, default='/pointcloud_diane', help='Topic encoder (solo descrittivo)')
    parser.add_argument('--period', type=float, default=2.0, help='Periodo in secondi')
    return parser.parse_args()

class DynamicEncoderLauncher(Node):
    def __init__(self, encoder_node, param_bandwidth, param_fps, topic_decoded, topic_encoded, period):
        super().__init__('dynamic_encoder_launcher')
        self.encoder_node = encoder_node
        self.param_bandwidth = param_bandwidth
        self.param_fps = param_fps
        self.topic_decoded = topic_decoded
        self.topic_encoded = topic_encoded
        self.period = period

        self.timer = self.create_timer(self.period, self.timer_callback)
    def measure_bw(self):
        cmd = f"ros2 topic bw {shlex.quote(self.topic_decoded)} --window 10"
        proc = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        time.sleep(self.period)
        proc.terminate()
        output, _ = proc.communicate()
        match = re.search(r'([\d.]+)\s*(MB|KB|B)/s from ([\d.]+) messages', output)
        if match:
            val, unit, _ = match.groups()
            factor = {'B': 1, 'KB': 1e3, 'MB': 1e6}[unit]
            return float(val) * factor
        return None

    def measure_fps(self):
        cmd = f"ros2 topic hz {shlex.quote(self.topic_decoded)} --window 10"
        proc = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        time.sleep(self.period)
        proc.terminate()
        output, _ = proc.communicate()
        match = re.search(r'average rate:\s*([\d.]+)', output)
        if match:
            return float(match.group(1))
        return None

    def timer_callback(self):
        bandwidth = self.measure_bw()
        fps = self.measure_fps()

        if bandwidth and fps:
            self.get_logger().info(f"Setting {self.param_bandwidth}={bandwidth:.2f} bytes/s, {self.param_fps}={fps:.2f} Hz")

            subprocess.run(
                ['ros2', 'param', 'set', self.encoder_node, self.param_bandwidth, str(bandwidth)],
                check=False
            )
            subprocess.run(
                ['ros2', 'param', 'set', self.encoder_node, self.param_fps, str(fps)],
                check=False
            )
        else:
            self.get_logger().warn("Impossibile misurare bandwidth o fps dal topic.")

def main(args=None):
    cli_args = parse_args()
    rclpy.init(args=args)
    node = DynamicEncoderLauncher(
        cli_args.encoder_node,
        cli_args.param_bandwidth,
        cli_args.param_fps,
        cli_args.topic_decoded,
        cli_args.topic_encoded,
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

