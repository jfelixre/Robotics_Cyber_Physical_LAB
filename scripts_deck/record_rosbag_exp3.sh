#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Script to record EXPERIMENT 3 rosbag data
# Usage: ./record_rosbag_exp3.sh
# 
# Run this AFTER launching experiment 3 with:
#   ros2 launch launch_pkg gazebo_sim_exp_3.launch.py

set -e  # Exit on error

EXPERIMENT_NAME="experiment_3"

echo "🎬 Starting rosbag recording for EXPERIMENT 3"
echo "🤖 Expected robots: robot_01, robot_02, robot_03" 
echo "📊 Recording Name: ${EXPERIMENT_NAME}"
echo ""

# Create bags directory if it doesn't exist
mkdir -p bags

echo "📋 Listing all available topics..."
TOPICS=$(ros2 topic list)
NUM_TOPICS=$(echo "$TOPICS" | wc -l)

echo "Found ${NUM_TOPICS} topics:"
echo "$TOPICS"
echo ""

if [ $NUM_TOPICS -eq 0 ]; then
    echo "❌ No topics found! Make sure experiment 3 is running."
    echo "💡 Launch with: ros2 launch launch_pkg gazebo_sim_exp_3.launch.py"
    exit 1
fi

echo "🎥 Starting rosbag recording..."
echo "📁 Saving to: bags/${EXPERIMENT_NAME}"
echo ""

# Test the command first
echo "🔧 Testing rosbag command..."

# Record ALL topics EXCEPT camera and image topics (SQLite3 = more compact, MCAP = faster access)
# For less disk space: use sqlite3  
# For faster analysis: use mcap
# Use regex pattern to exclude topics containing 'cam' or 'image'
echo "🚀 Executing: ros2 bag record -o bags/${EXPERIMENT_NAME} --all --storage sqlite3 --compression-mode file --exclude-regex '.*cam.*|.*image.*'"
ros2 bag record -o bags/${EXPERIMENT_NAME} --all --storage sqlite3 --compression-mode file --exclude-regex '.*cam.*|.*image.*' || {
    echo "❌ Error with exclude-regex, trying without exclusion..."
    ros2 bag record -o bags/${EXPERIMENT_NAME} --all --storage sqlite3 --compression-mode file
}
# &
# ROSBAG_PID=$!

# echo "✅ Rosbag recording started (PID: ${ROSBAG_PID})"
# echo ""
# echo "🎯 Recording ALL topics EXCEPT:"
# echo "   - Topics containing 'cam' (camera data)"
# echo "   - Topics containing 'image' (image data)"
# echo "📊 Expected key topics:"
# echo "   - /tf"
# echo "   - /robot_01/robot_state"
# echo "   - /robot_02/robot_state"
# echo "   - /robot_03/robot_state"
# echo "   - /robot_01/cmd_vel"
# echo "   - /robot_02/cmd_vel"
# echo "   - /robot_03/cmd_vel"
# echo ""
# echo "📋 To stop recording:"
# echo "   Press Ctrl+C"
# echo ""
# echo "📊 To generate plots after recording:"
# echo "   python3 convert_experiment_to_csv.py ${EXPERIMENT_NAME}"
# echo "   python3 plot_trajectories_professional.py"
# echo ""

# # Function to cleanup on exit
# cleanup() {
#     echo ""
#     echo "🛑 Stopping rosbag recording..."
#     if kill -0 $ROSBAG_PID 2>/dev/null; then
#         kill -SIGINT $ROSBAG_PID
#         sleep 2
#         if kill -0 $ROSBAG_PID 2>/dev/null; then
#             kill -SIGTERM $ROSBAG_PID
#         fi
#         echo "✅ Rosbag recording stopped"
#     fi
#     echo "🏁 Experiment 3 recording completed!"
#     echo "📁 Rosbag saved in: bags/${EXPERIMENT_NAME}"
#     echo "📊 Next steps:"
#     echo "   python3 convert_experiment_to_csv.py ${EXPERIMENT_NAME}"
#     echo "   python3 plot_trajectories_professional.py"
#     exit 0
# }

# # Trap Ctrl+C and cleanup
# trap cleanup INT TERM

# # Wait for user to stop with a simple loop
# echo "Press Ctrl+C to stop recording..."
# while kill -0 $ROSBAG_PID 2>/dev/null; do
#     sleep 1
# done

# # If we reach here, rosbag stopped by itself
# echo "📊 Rosbag recording finished"
# cleanup


echo "Presiona enter para salir..."
read 