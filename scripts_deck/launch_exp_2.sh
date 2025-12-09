#!/bin/bash
source /opt/ros/jazzy/setup.bash
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.bash"
cd "$HOME/Robotics_Cyber_Physical_LAB"


ros2 topic list
ros2 launch launch_pkg gazebo_sim_exp_2.launch.py

#echo "Presiona enter para salir..."
#read  # O puedes poner $SHELL para quedarte en la terminal