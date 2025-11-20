# PlatformIO for Microcontroller Interface (ESP32)

## Directory Tree view
This section describes the structure of the Firmware directory, which contains all microcontroller programs used in the robot system.
There is one PlatformIO project for each firmware.
```bash
├── GNSS                                # SparkFun ZED-F9P GNSS module
│   ├── zedf9p_esp32                    # microcontroller program for ESP32 micromod
│   │   ├── src                         # source folder
│   │   │   └── main.cpp                # main program for ESP32 micromod
│   │   ├── include                     # include folder
│   │   │   ├── main.hpp                # main header for ESP32 micromod
│   │   │   └── secret.h                # wifi credencial file. DO NOT COMMIT
│   │   ├── APK3.log                    # log file for accuracy testing
│   │   └── platformio.ini              # PlatformIO project configuration file 
│   └── zedf9p_ucenter                  # python program for ucentral port serial data
│       ├── gnss_publisher.py           # python program
│       └── requirements.txt            # requirement dependencies for pip 
├── power_pcb_datas                     # Microcontroller programs to publish datas from power PCB
│   ├── src                             # source folder
│   │   └── main.cpp                    # main program for power data publisher
│   ├── include                         # include folder
│   │   ├── main.hpp                    # main header for power data publisher
│   │   └── wifi_secrets.h              # wifi credencial file. DO NOT COMMIT
│   ├── extra_packages/power_pcb_msgs   # custom ROS message type to publish all datas at once
│   └── platformio.ini                  # PlatformIO project configuration file 
├── Robots                              # main Microcontroller programs to control robots
│   ├── MiniATV                         # main ESP32 program of the MiniATV
│   └── Snower                          # Main ESP32 program of the Snower
│       ├── src                         # source folder
│       │   ├── main.cpp                # main program for Snower
│       │   └── RobotDriveControl.c     # diff drive source file 
│       └── include                     # include folder
│           ├── main.hpp                # main header
│           ├── wifi_secrets.h          # wifi credencial file. DO NOT COMMIT
│           └── RobotDriveControl.h     # diff drive header file 
├── Trigger                             # trigger button for the dataset project
│   ├── src                             # source folder
│   │   └── main.cpp                    # main program for the button
│   ├── include                         # include folder
│   │   ├── main.hpp                    # main header for the button
│   │   └── wifi_secrets.h              # wifi credencial file. DO NOT COMMIT
│   └── platformio.ini                  # PlatformIO project configuration file
└── Ultrasonic                          # Ultrasonic sensor array module 
    ├── src                             # source folder
    │   └── main.cpp                    # main program for Ultrasonic module
    └── platformio.ini                  # PlatformIO project configuration file
```

## Prevent Committing Credentials
To ensure sensitive files are not accidentally committed, run the following commands to ignore modifications locally:
```bash
git update-index --assume-unchanged Firmware/GNSS/zedf9p_esp32/include/secrets.h
git update-index --assume-unchanged Firmware/Robots/Snower/include/wifi_secrets.h
git update-index --assume-unchanged Firmware/Robots/power_pcb_datas/include/wifi_secrets.h
git update-index --assume-unchanged Firmware/Robots/Trigger/include/wifi_secrets.h
```

## Install PlatformIO
Follow this link for a tutorial on how to install PLatformIO IDE on VSCode :
[https://platformio.org/install/ide?install=vscode](https://platformio.org/install/ide?install=vscode)

If you only want to install PlatformIO Core, you can follow this link :
[https://docs.platformio.org/en/latest/core/installation/index.html](https://docs.platformio.org/en/latest/core/installation/index.html)

For Linux users, follow this link to install udev rules for PlatformIO :
[https://docs.platformio.org/en/latest/core/installation/udev-rules.html](https://docs.platformio.org/en/latest/core/installation/udev-rules.html)

## Requirements 
For `micro_ros_platformio` library (from [https://github.com/micro-ROS/micro_ros_platformio](https://github.com/micro-ROS/micro_ros_platformio)) :

- PlatformIO Core version 6.1.0 or greater
- PlatformIO needs  `git`, `cmake` and `pip3` to handle micro-ROS internal dependencies:

  ```bash
  apt install -y git cmake python3-pip
  ```

## Dependencies
The `platformio.ini` files should list all dependencies to automatically setup the projects and flash the microcontrollers. Please remember that the first setup of a project can take a long time when the `micro_ros_platformio` librairy is used.

For the _power_pcb_datas_ project, the ROS package in the _extra_packages_ folder must be built and sourced on your local machine in order to echo the `/power_pcb/state` topic.

## Use PlatformIO
Follow this link for a tutorial on how to use PlatformIO IDE :
[https://docs.platformio.org/en/stable/integration/ide/vscode.html](https://docs.platformio.org/en/stable/integration/ide/vscode.html)

Alternatively, you can use CLI commands. Follow this link for CLI documentation :
[https://docs.platformio.org/en/latest/core/userguide/cmd_run.html](https://docs.platformio.org/en/latest/core/userguide/cmd_run.html)