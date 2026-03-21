#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;🚀 Experiment 3 - Advanced Simulation\007"

# Set experiment name for rosbag recording
export EXPERIMENT_TIMESTAMP="experiment_3"
echo "📊 Experiment Name: ${EXPERIMENT_TIMESTAMP}"
echo "💡 To record rosbag: ./scripts_deck/record_rosbag.sh experiment_3"
echo ""

ros2 launch launch_pkg gazebo_sim_exp_3.launch.py

echo "Presiona enter para salir..."
read
