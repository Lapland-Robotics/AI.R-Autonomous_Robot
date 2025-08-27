#!/usr/bin/env python3
import os
import time
import logging

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy

from std_msgs.msg import String

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
        qos_profile = QoSProfile(depth=10, reliability=ReliabilityPolicy.SYSTEM_DEFAULT)
        self.subscription = self.create_subscription(String, '/power_board/state', self.listener_callback, qos_profile)


    def listener_callback(self, msg):
        self.get_logger().info('power_pcb status: "%s"' % msg.data)
        self.file_logger.info(msg)

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
