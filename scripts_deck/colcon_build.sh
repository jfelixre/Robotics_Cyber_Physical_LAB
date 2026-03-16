#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;🔨 Colcon Build - Workspace Compilation\007"

colcon build --symlink-install

echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal