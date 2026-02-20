#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

ros2 launch launch_pkg gazebo_sim_exp_3.launch.py

echo "Presiona enter para salir..."
read
