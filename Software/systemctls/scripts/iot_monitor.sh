#!/bin/bash

set -e # exit immediately on any command failure

source /home/robotics/repos/AI.R-Autonomous_Robot/Software/systemctls/scripts/ros2_config.bash
source /home/robotics/repos/AI.R-Autonomous_Robot/Software/ros2_ws/install/setup.bash

ros2 run iot_monitor power_monitor
