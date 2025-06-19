#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import subprocess
import shlex
import re
import time
import argparse


def parse_args():
    parser = argparse.ArgumentParser(
        description="Launcher dinamico per controllo proporzionale di bandwidth e fps per il pointcloud encoder."
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
        default='/pointcloud_diane',
        help='Topic su cui misurare banda compressa (ros2 topic bw)'
    )
    parser.add_argument(
        '--topic-hz',
        type=str,
        default='/decoded',
        help='Topic su cui misurare fps (ros2 topic hz)'
    )
    parser.add_argument(
        '--period',
        type=float,
        default=2.0,
        help='Periodo in secondi del controllo'
    )
    parser.add_argument(
        '--fps-target',
        type=float,
        default=30.0,
        help='Frame rate target da mantenere'
    )
    parser.add_argument(
        '--kp',
        type=float,
        default=0.1,
        help='Ganho proporzionale per il P-controller'
    )
    return parser.parse_args()


class DynamicEncoderLauncher(Node):
    def __init__(
        self,
        encoder_node,
        param_bandwidth,
        param_fps,
        topic_bw,
        topic_hz,
        period,
        fps_target,
        kp
    ):
        super().__init__('dynamic_encoder_launcher')
        self.encoder_node = encoder_node
        self.param_bandwidth = param_bandwidth
        self.param_fps = param_fps
        self.topic_bw = topic_bw
        self.topic_hz = topic_hz
        self.period = period
        self.fps_target = fps_target
        self.kp = kp

        self.timer = self.create_timer(self.period, self.timer_callback)

    def measure_bw(self):
        cmd = f"ros2 topic bw {shlex.quote(self.topic_bw)} --window 10"
        proc = subprocess.Popen(
            shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
        )
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
        cmd = f"ros2 topic hz {shlex.quote(self.topic_hz)} --window 10"
        proc = subprocess.Popen(
            shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
        )
        time.sleep(self.period)
        proc.terminate()
        output, _ = proc.communicate()
        match = re.search(r'average rate:\s*([\d.]+)', output)
        if match:
            return float(match.group(1))
        return None

    def timer_callback(self):
        bw = self.measure_bw()
        fps = self.measure_fps()

        if bw is None or fps is None:
            self.get_logger().warn("Impossibile misurare bandwidth o fps dal topic.")
            return

        # Calcolo dell'errore rispetto al frame rate target
        error = self.fps_target - fps
        # P-controller per modulare la banda
        new_bw = bw * (1 + self.kp * error / self.fps_target)
        # Limite inferiore di banda (es. 1 MB/s)
        min_bw = 1e6
        if new_bw < min_bw:
            new_bw = min_bw

        # Log delle regolazioni
        self.get_logger().info(
            f"Regolo bandwidth a {new_bw:.0f} B/s per mantenere {self.fps_target:.1f} Hz "
            f"(misurati {fps:.2f} Hz, kp={self.kp})"
        )

        # Impostazione dei parametri sul nodo encoder
        subprocess.run([
            'ros2', 'param', 'set', self.encoder_node,
            self.param_bandwidth, str(new_bw)
        ], check=False)
        subprocess.run([
            'ros2', 'param', 'set', self.encoder_node,
            self.param_fps, str(self.fps_target)
        ], check=False)


def main(args=None):
    cli_args = parse_args()
    rclpy.init(args=args)
    node = DynamicEncoderLauncher(
        cli_args.encoder_node,
        cli_args.param_bandwidth,
        cli_args.param_fps,
        cli_args.topic_bw,
        cli_args.topic_hz,
        cli_args.period,
        cli_args.fps_target,
        cli_args.kp
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

