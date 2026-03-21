#!/usr/bin/env python3

"""
Generador de timestamp para experimentos multi-robot.
Útil para lanzamientos manuales con timestamp compartido.

Uso:
    timestamp=$(python3 generate_timestamp.py)
    ros2 launch launch_pkg robot_01_sim.launch.py experiment_timestamp:=$timestamp &
    sleep 2
    ros2 launch launch_pkg robot_02_sim.launch.py experiment_timestamp:=$timestamp &
"""

from datetime import datetime

if __name__ == "__main__":
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    print(timestamp, end='')  # Sin newline para facilitar captura