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
        help='Topic su cui misurare banda effettiva (ros2 topic bw)'
    )
    parser.add_argument(
        '--topic-hz',
        type=str,
        default='/decoded',
        help='Topic su cui misurare fps effettivi (ros2 topic hz)'
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
    def __init__(
        self,
        encoder_node,
        param_bandwidth,
        param_fps,
        topic_bw,
        topic_hz,
        period,
        update_interval,
        min_bandwidth
    ):
        super().__init__('dynamic_encoder_launcher')
        self.encoder_node = encoder_node
        self.param_bandwidth = param_bandwidth
        self.param_fps = param_fps
        self.topic_bw = topic_bw
        self.topic_hz = topic_hz
        self.period = period
        self.update_interval = update_interval
        self.min_bandwidth = min_bandwidth

        # Stato per campionamento
        self.last_update = time.time()
        self.bw_samples = []
        self.fps_samples = []

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
            self.get_logger().warn("Impossibile misurare banda o fps da /decoded.")
            return

        # Accumulo campioni
        self.bw_samples.append(bw)
        self.fps_samples.append(fps)

        # Se non è ancora ora di aggiornare, esco
        now = time.time()
        if now - self.last_update < self.update_interval:
            return

        # Calcolo media campioni
        avg_bw = sum(self.bw_samples) / len(self.bw_samples)
        avg_fps = sum(self.fps_samples) / len(self.fps_samples)

        # Reset campioni
        self.bw_samples.clear()
        self.fps_samples.clear()
        self.last_update = now

        # Applico banda minima
        bw_to_set = max(avg_bw, self.min_bandwidth)

        # Log delle medie e decisione
        self.get_logger().info(
            f"Media ultimi {self.update_interval}s: bw {avg_bw/1e6:.2f} MB/s, fps {avg_fps:.2f} Hz. "
            f"Imposto bandwidth min {self.min_bandwidth/1e6:.2f} MB/s -> {bw_to_set/1e6:.2f} MB/s"
        )

        # Imposto i parametri dell'encoder
        subprocess.run([
            'ros2', 'param', 'set', self.encoder_node,
            self.param_bandwidth, str(bw_to_set)
        ], check=False)
        subprocess.run([
            'ros2', 'param', 'set', self.encoder_node,
            self.param_fps, str(avg_fps)
        ], check=False)

        self.get_logger().info(
            f"Aggiornati parametri: bandwidth={bw_to_set:.0f} B/s, fps={avg_fps:.2f}"
        )


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
        cli_args.update_interval,
        cli_args.min_bandwidth
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

