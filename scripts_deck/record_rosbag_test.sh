#!/bin/bash

# 🎬 Rosbag Recording Script - TEST VERSION
# Records ROS2 topics for multi-robot experiments with proper signal handling

source /opt/ros/jazzy/setup.bash
source install/setup.zsh

cd /home/javierfr/Robotics_Cyber_Physical_LAB

# Configuration
EXPERIMENT_NAME="experiment_test"
EXPERIMENT_DIR="bags/${EXPERIMENT_NAME}"

# Create experiment directory
mkdir -p $EXPERIMENT_DIR

echo "🚀 Starting Rosbag recording - TEST"
echo "📁 Recording to: $EXPERIMENT_DIR"
echo "⏱️  This is a short test recording"
echo ""

# Start recording with SQLite3 format and compression
# Exclude topics containing 'cam' or 'image' to save space and improve performance
ros2 bag record \
    --storage sqlite3 \
    --exclude ".*cam.*|.*image.*" \
    /robot_01/robot_state \
    /robot_02/robot_state \
    /robot_03/robot_state \
    /robot_01/robot_platform_vel \
    /robot_02/robot_platform_vel \
    /robot_03/robot_platform_vel \
    /tf \
    /tf_static \
    -o $EXPERIMENT_DIR &

# Store the PID
ROSBAG_PID=$!

echo "📊 Recording started with PID: $ROSBAG_PID"
echo "🎯 Selected topics:"
echo "   - Robot states (/robot_0X/robot_state)"
echo "   - Velocities (/robot_0X/robot_platform_vel)"  
echo "   - Transforms (/tf, /tf_static)"
echo ""

# Function to cleanup on exit
cleanup() {
    echo ""
    echo "🛑 Stopping rosbag recording..."
    if kill -0 $ROSBAG_PID 2>/dev/null; then
        kill -SIGINT $ROSBAG_PID
        sleep 2
        if kill -0 $ROSBAG_PID 2>/dev/null; then
            kill -SIGTERM $ROSBAG_PID
        fi
        echo "✅ Rosbag recording stopped"
    fi
    echo "🏁 TEST recording completed!"
    echo "📁 Rosbag saved in: bags/${EXPERIMENT_NAME}"
    echo "📊 Next steps:"
    echo "   python3 convert_experiment_to_csv.py ${EXPERIMENT_NAME}"
    echo "   python3 plot_trajectories_professional.py"
    exit 0
}

# Trap Ctrl+C and cleanup
trap cleanup INT TERM

# Wait for user to stop with a simple loop
echo "Press Ctrl+C to stop recording..."
while kill -0 $ROSBAG_PID 2>/dev/null; do
    sleep 1
done

# If we reach here, rosbag stopped by itself
echo "📊 Rosbag recording finished"
cleanup