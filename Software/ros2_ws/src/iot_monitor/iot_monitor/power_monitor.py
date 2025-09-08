#!/usr/bin/env python3
import os
import time
import logging

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy

from std_msgs.msg import String
from power_pcb_msgs.msg import PowerDatas

class PowerMonitorNode(Node):

    def __init__(self):
        super().__init__('power_monitor')

        log_dir = os.path.realpath("src/iot_monitor/logs/")
        os.makedirs(log_dir, exist_ok=True)
        now = time.strftime("%Y-%m-%d_%H-%M-%S")
        
        log_file = os.path.join(log_dir+ "/power_log_" + now + '.log')
        self.file_logger = logging.getLogger('power_monitor_file')
        self.file_logger.setLevel(logging.INFO)
        fh = logging.FileHandler(log_file, mode='a')
        fh.setFormatter(logging.Formatter('%(asctime)s  %(message)s', datefmt='%Y-%m-%d %H:%M:%S'))
        self.file_logger.addHandler(fh)
        qos_profile = QoSProfile(depth=10, reliability=ReliabilityPolicy.BEST_EFFORT)
        self.subscription = self.create_subscription(PowerDatas, '/power_pcb/state', self.listener_callback, qos_profile)


    def listener_callback(self, msg):
        log_message = (
            f'[POWER PCB] '
            f'temp : {msg.temp:.2f}°C, '
            f'total current : {msg.tot_current_a:.2f}A, '
            f'{msg.ina_sensors[1].label} : '
            f'{msg.ina_sensors[1].bus_v:.2f}V, '
            f'{msg.ina_sensors[1].current_ma:.2f}mA, '
            f'{msg.ina_sensors[1].power_mw:.2f}mW; '
            f'{msg.ina_sensors[2].label} : '
            f'{msg.ina_sensors[2].bus_v:.2f}V, '
            f'{msg.ina_sensors[2].current_ma:.2f}mA, '
            f'{msg.ina_sensors[2].power_mw:.2f}mW; '
            f'{msg.ina_sensors[3].label} : '
            f'{msg.ina_sensors[3].bus_v:.2f}V, '
            f'{msg.ina_sensors[3].current_ma:.2f}mA, '
            f'{msg.ina_sensors[3].power_mw:.2f}mW; '
            f'{msg.ina_sensors[4].label} : '
            f'{msg.ina_sensors[4].bus_v:.2f}V, '
            f'{msg.ina_sensors[4].current_ma:.2f}mA, '
            f'{msg.ina_sensors[4].power_mw:.2f}mW; '
        )
        self.get_logger().info(log_message)
        self.file_logger.info(log_message)

    def powerDatas_to_string(self, msg) :
        string = (
            f'[POWER PCB] '
            f'temp : {msg.temp}°C, '
            f'total current : {msg.tot_current_a}A, '
            f'{msg.ina_sensors[1].label} : '
            f'{msg.ina_sensors[1].bus_v}V, '
            f'{msg.ina_sensors[1].current_ma}mA, '
            f'{msg.ina_sensors[1].power_mw}mW; '
            f'{msg.ina_sensors[2].label} : '
            f'{msg.ina_sensors[2].bus_v}V, '
            f'{msg.ina_sensors[2].current_ma}mA, '
            f'{msg.ina_sensors[2].power_mw}mW; '
            f'{msg.ina_sensors[3].label} : '
            f'{msg.ina_sensors[3].bus_v}V, '
            f'{msg.ina_sensors[3].current_ma}mA, '
            f'{msg.ina_sensors[3].power_mw}mW; '
            f'{msg.ina_sensors[4].label} : '
            f'{msg.ina_sensors[4].bus_v}V, '
            f'{msg.ina_sensors[4].current_ma}mA, '
            f'{msg.ina_sensors[4].power_mw}mW; '
        )
        return string

def main(args=None):
    rclpy.init(args=args)
    node = PowerMonitorNode()
    try:
        rclpy.spin(node)
    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()

if __name__ == '__main__':
    main()
