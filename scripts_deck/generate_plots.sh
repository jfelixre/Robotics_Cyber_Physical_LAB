#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

python3 plot_trajectories_corrected.py --experiment experiment_3 --robots 3 --errors --metrics
