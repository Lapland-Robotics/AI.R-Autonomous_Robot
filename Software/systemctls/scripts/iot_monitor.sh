#!/bin/bash

set -e # exit immediately on any command failure

export HOME=~/$USER

source $AIR_AR_PATH/Software/systemctls/scripts/ros2_config.bash
source $AIR_AR_PATH/Software/ros2_ws/install/setup.bash

ros2 run iot_monitor power_monitor
