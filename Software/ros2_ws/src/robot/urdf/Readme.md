# Robot package — Rviz2 simulation
> [!NOTE]
> Commands to load the robot package which allows you to retrieve the 3D model of the snower for simulation on Rviz2

## Install the package

```bash
cd ~/YOUR_OWN_PATH/AI.R-Autonomous_Robot/Software/ros2_ws

# Build only the robot package with symbolic links (avoids rebuilding for each source modification)
colcon build --packages-select robot --symlink-install

# Load the workspace environment so that ROS2 recognizes the freshly compiled package
source install/setup.bash

# Check that the package is loaded
ros2 pkg prefix robot 

# Check that the 3D model is here
find -name "Snower_whole_3d.stl"
```

## Delete the package

```bash
cd ~/YOUR_OWN_PATH/AI.R-Autonomous_Robot/Software/ros2_ws

rm -rf build/robot install/robot log/
```