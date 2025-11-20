#!/bin/bash

export HOME=/home/$USER

export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
export CYCLONEDDS_URI=file://$AIR_AR_PATH/Software/cyclonedds-config.xml

source /opt/ros/humble/setup.bash