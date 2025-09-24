#!/bin/bash

# Add or update AIR_AR_PATH in ~/.bashrc
if ! grep -q "export AIR_AR_PATH=" ~/.bashrc; 
then
    echo "export AIR_AR_PATH=\"$(pwd)\"" >> ~/.bashrc
    echo "AIR_AR_PATH added to ~/.bashrc"
else
    sed -i "s|export AIR_AR_PATH=.*|export AIR_AR_PATH=\"$(pwd)\"|" ~/.bashrc
    echo "AIR_AR_PATH updated in ~/.bashrc"
fi
export AIR_AR_PATH="$(pwd)"

# Add or update AIR_AR_PATH and USER in /etc/default/air_ar
if [ ! -f /etc/default/air_ar ]; 
then
    echo "AIR_AR_PATH=\"$(pwd)\"" | sudo tee /etc/default/air_ar > /dev/null
    echo "USER=\"$(whoami)\"" | sudo tee -a /etc/default/air_ar > /dev/null
    echo "/etc/default/air_ar created with AIR_AR_PATH and USER"
else
    sudo sed -i "s|^AIR_AR_PATH=.*|AIR_AR_PATH=\"$(pwd)\"|" /etc/default/air_ar
    sudo sed -i "s|^USER=.*|USER=\"$(whoami)\"|" /etc/default/air_ar
    echo "AIR_AR_PATH and USER updated in /etc/default/air_ar"
fi

# Copy the systemd service file to the system directory
sudo cp -r Software/systemctls/services/* /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable snower-ros2-docker.service
sudo systemctl enable snower-data-collector.service
sudo systemctl enable snower-iot-monitor.service
sudo systemctl enable snower-ouster-lidar.service
sudo systemctl enable snower-zed2-camara.service

echo "Snower services installed and enabled."