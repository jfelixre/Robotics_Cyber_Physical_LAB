#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"



ros2 launch launch_pkg robot_01_sim.launch.py planner:=astar
#ros2 launch launch_pkg robot_01_sim.launch.py planner:=reverse
#ros2 launch launch_pkg robot_01_sim.launch.py planner:=greedy

echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal