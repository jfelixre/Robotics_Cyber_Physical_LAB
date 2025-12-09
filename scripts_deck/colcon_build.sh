#!/bin/bash
source /opt/ros/jazzy/setup.bash
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.bash"
cd "$HOME/Robotics_Cyber_Physical_LAB"

colcon build --symlink-install

echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal