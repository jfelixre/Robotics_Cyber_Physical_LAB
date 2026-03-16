#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;🚨 Emergency Stop - Robot 2\007"

echo "Activando parada de emergencia para Robot 2..."
ros2 run inv_kinematics_pkg robot_emergency_stop_node --ros-args -p robot_id:=2

echo "Parada de emergencia completada para Robot 2"
echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal