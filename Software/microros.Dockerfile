FROM microros/micro-ros-agent:humble

# install ros package
RUN apt-get update
RUN apt-get install -y ros-$ROS_DISTRO-rmw-fastrtps-cpp
RUN rm -rf /var/lib/apt/lists/*

CMD ["serial", "--dev", "/dev/ttyUSB0"]
