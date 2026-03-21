#!/bin/bash

# Script to record ALL active ROS topics to rosbag
# Usage: ./record_rosbag.sh [EXPERIMENT_NAME]
# 
# 🎯 RECOMMENDED: Use specialized scripts instead:
#   ./record_rosbag_exp1.sh     # For experiment 1 (2 robots)
#   ./record_rosbag_exp2.sh     # For experiment 2 (2 robots)
#   ./record_rosbag_exp3.sh     # For experiment 3 (3 robots)
# 
# Generic usage:
#   ./record_rosbag.sh experiment_1        # For experiment 1  
#   ./record_rosbag.sh experiment_2        # For experiment 2
#   ./record_rosbag.sh experiment_3        # For experiment 3
#   ./record_rosbag.sh                     # Auto-generate timestamp (backup)
# 
# Run this AFTER launching your experiment and robots

set -e  # Exit on error

# Get experiment name from argument or generate timestamp as fallback
if [ $# -eq 1 ]; then
    EXPERIMENT_NAME="$1"
    echo "🎬 Starting rosbag recording with specified name"
    echo "📊 Recording Name: ${EXPERIMENT_NAME}"
else
    TIMESTAMP=$(date +"%Y%m%d_%H%M%S") 
    EXPERIMENT_NAME="experiment_${TIMESTAMP}"
    echo "🎬 Starting rosbag recording with auto-generated name"
    echo "📊 Recording Name: ${EXPERIMENT_NAME}"
    echo "⏰ Timestamp: ${TIMESTAMP}"
fi

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
    echo "❌ No topics found! Make sure your experiment is running."
    exit 1
fi

echo "🎥 Starting rosbag recording..."
echo "📁 Saving to: bags/${EXPERIMENT_NAME}"
echo ""

# Record ALL topics (SQLite3 = more compact, MCAP = faster access)
# For less disk space: use sqlite3
# For faster analysis: use mcap  
ros2 bag record -o bags/${EXPERIMENT_NAME} --all --storage sqlite3 &
ROSBAG_PID=$!

echo "✅ Rosbag recording started (PID: ${ROSBAG_PID})"
echo ""
echo "🎯 Recording ALL topics!"
echo ""
echo "📋 To stop recording:"
echo "   Press Ctrl+C"
echo ""
echo "📊 To generate plots after recording:"
echo "   python3 convert_experiment_to_csv.py ${EXPERIMENT_NAME}"
echo "   python3 plot_trajectories_professional.py --experiment ${EXPERIMENT_NAME} --legacy --errors --robots NUM_ROBOTS"
echo ""
echo "💡 Recommended specialized scripts:"
echo "   ./scripts_deck/record_rosbag_exp1.sh    # Experiment 1 (2 robots)"
echo "   ./scripts_deck/record_rosbag_exp2.sh    # Experiment 2 (2 robots)"
echo "   ./scripts_deck/record_rosbag_exp3.sh    # Experiment 3 (3 robots)"

# Function to cleanup on exit
cleanup() {
    echo ""
    echo "🛑 Stopping rosbag recording..."
    if kill -0 $ROSBAG_PID 2>/dev/null; then
        kill $ROSBAG_PID
        echo "✅ Rosbag recording stopped"
    fi
    echo "🏁 Recording completed: ${EXPERIMENT_NAME}"
    echo "📁 Rosbag saved in: bags/${EXPERIMENT_NAME}"
}

# Trap Ctrl+C and cleanup
trap cleanup INT TERM

# Wait for user to stop
wait