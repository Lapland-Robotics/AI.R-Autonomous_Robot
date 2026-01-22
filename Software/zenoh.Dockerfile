FROM ros:humble

# install ros package
RUN apt-get update
RUN apt-get install -y ros-$ROS_DISTRO-rmw-zenoh-cpp
RUN apt-get install -y python3-pip
RUN apt-get install -y git
RUN apt-get install -y cargo
RUN apt-get install -y clang
RUN apt-get install -y nlohmann-json3-dev
RUN apt-get install -y ros-humble-ament-cmake-vendor-package

RUN echo 'source "/opt/ros/$ROS_DISTRO/setup.bash"' >> ~/.bashrc

ENV RMW_IMPLEMENTATION=rmw_zenoh_cpp
ENV ZENOH_ROUTER_CONFIG_URI=/app/zenoh-router-config.json5

SHELL ["/bin/bash", "-c"]

WORKDIR /app/ros2_ws

COPY zenoh-router-config.json5 /app/

RUN mkdir -p src && cd src && git clone https://github.com/ros2/rmw_zenoh.git -b humble

RUN rosdep update 
RUN rosdep install --from-paths src --ignore-src --rosdistro humble -y 

RUN source /opt/ros/${ROS_DISTRO}/setup.bash && \
    colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release

ADD ros_entrypoint.sh /

ENTRYPOINT ["/ros_entrypoint.sh"]

CMD ["ros2", "run", "rmw_zenoh_cpp", "rmw_zenohd"]
