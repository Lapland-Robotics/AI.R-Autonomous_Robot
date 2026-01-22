FROM ros:humble

# Install dependencies
RUN apt-get update
RUN apt-get install -y ros-$ROS_DISTRO-rmw-zenoh-cpp
RUN apt-get install -y  libpcap-dev

RUN rm -rf /var/lib/apt/lists/*

# ROS setup
ENV RMW_IMPLEMENTATION=rmw_zenoh_cpp

SHELL ["/bin/bash", "-c"]

WORKDIR /app

# Copy robosense lidar driver files
COPY drivers/src/rslidar_msg drivers/src/rslidar_msg
COPY drivers/src/rslidar_sdk drivers/src/rslidar_sdk

# Build driver
RUN cd drivers && \
    source /opt/ros/${ROS_DISTRO}/setup.bash && \
    colcon build --packages-select rslidar_msg rslidar_sdk

ADD drivers_entrypoint.sh /
RUN chmod +x /drivers_entrypoint.sh

ENTRYPOINT ["/drivers_entrypoint.sh"]

CMD ["ros2", "run", "rslidar_sdk", "rslidar_sdk_node"]
