#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;🤖 Robot 3 - Individual Launch\007"

##planner puede ser astar, reverse o greedy
#Control puede ser pid o geometric

ros2 launch launch_pkg robot_03_sim.launch.py planner:=reverse controller:=geometric

echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal
