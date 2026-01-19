#!/bin/bash

export HOME=/home/$USER

export RMW_IMPLEMENTATION=rmw_zenoh_cpp
export AIR_AR_PATH=$HOME/repos/AI.R-Autonomous_Robot
export ZENOH_ROUTER_CONFIG_URI=$AIR_AR_PATH/Software/zenoh-router-config.json5
# export CYCLONEDDS_URI=file://$AIR_AR_PATH/Software/cyclonedds-config.xml

source /opt/ros/humble/setup.bash
source $HOME/ws_rmw_zenoh/install/setup.bash