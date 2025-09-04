FROM ros:humble

# Install dependencies
RUN apt-get update
RUN apt-get install -y ros-$ROS_DISTRO-rmw-cyclonedds-cpp
RUN apt-get install -y  libpcap-dev

RUN rm -rf /var/lib/apt/lists/*

# ROS setup
ENV RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
ENV CYCLONEDDS_URI=file:///app/cyclonedds-config.xml

SHELL ["/bin/bash", "-c"]

WORKDIR /app

# Copy robosense lidar driver files
COPY cyclonedds-config.xml /app/
COPY drivers/src/robosense_rslidar_msg drivers/src/robosense_rslidar_msg
COPY drivers/src/robosense_rslidar_sdk drivers/src/robosense_rslidar_sdk

# Build driver
RUN cd drivers && \
    source /opt/ros/${ROS_DISTRO}/setup.bash && \
    colcon build --packages-select robosense_rslidar_msg robosense_rslidar_sdk

ADD drivers_entrypoint.sh /

ENTRYPOINT ["/drivers_entrypoint.sh"]

CMD ["ros2", "run", "rslidar_sdk", "rslidar_sdk_node"]
