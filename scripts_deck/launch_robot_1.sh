#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

##planner puede ser astar, reverse o greedy
#Control puede ser pid o geometric

ros2 launch launch_pkg robot_01_sim.launch.py planner:=reverse controller:=geometric


echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal