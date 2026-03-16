#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;🎯 Task Manager 2 - Robot Control\007"



ros2 launch launch_pkg create_task2.launch.py

#echo "Presiona enter para salir..."
#read  # O puedes poner $SHELL para quedarte en la terminal