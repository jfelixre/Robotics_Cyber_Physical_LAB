#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;🧪 Test Launch - Development Testing\007"



ros2 launch launch_pkg test.launch.py

echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal