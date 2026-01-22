FROM ros:humble

# install ros package
RUN apt-get update
RUN apt-get install -y python3-opencv
RUN apt-get install -y ros-$ROS_DISTRO-cv-bridge
RUN apt-get install -y ros-$ROS_DISTRO-tf-transformations
RUN apt-get install -y ros-$ROS_DISTRO-xacro
RUN apt-get install -y ros-$ROS_DISTRO-robot-localization
RUN apt-get install -y ros-$ROS_DISTRO-navigation2
RUN apt-get install -y ros-$ROS_DISTRO-nav2-bringup
RUN apt-get install -y ros-$ROS_DISTRO-rmw-zenoh-cpp
RUN apt-get install -y ros-$ROS_DISTRO-slam-toolbox
RUN apt-get install -y ros-$ROS_DISTRO-pointcloud-to-laserscan
RUN apt-get install -y ros-$ROS_DISTRO-laser-filters
RUN apt-get install -y python3-pip
RUN pip install pcl
RUN pip install Pillow
RUN pip install open3d

RUN rm -rf /var/lib/apt/lists/*
RUN echo 'source "/opt/ros/$ROS_DISTRO/setup.bash"' >> ~/.bashrc

ENV RMW_IMPLEMENTATION=rmw_zenoh_cpp

SHELL ["/bin/bash", "-c"]

WORKDIR /app

COPY ros2_ws ros2_ws/

RUN cd ros2_ws && \
    rm -rf build install log && \
    source /opt/ros/${ROS_DISTRO}/setup.bash && \
    colcon build --packages-select power_pcb_msgs robot navigator data_collector iot_monitor performance_testing --cmake-args -DCMAKE_BUILD_TYPE=Release

ADD ros_entrypoint.sh /

ENTRYPOINT ["/ros_entrypoint.sh"]

CMD ["ros2", "launch", "robot", "run.launch.py"]
