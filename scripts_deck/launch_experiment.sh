#!/bin/bash

# Script simplificado para lanzar robots con timestamp automático
# Uso: ./launch_experiment.sh [robots]
# Ejemplo: ./launch_experiment.sh 1 2 3

# Generar timestamp único (una línea simple)
timestamp=$(date +%Y%m%d_%H%M%S)

echo "🎯 Timestamp generado: $timestamp"
echo "📁 Datos en: csv/multi_robot_exp_$timestamp/"
echo ""

# Si no hay argumentos, solo mostrar el timestamp para uso manual
if [ $# -eq 0 ]; then
    echo "📝 Para usar manualmente:"
    echo "   ros2 launch launch_pkg robot_01_sim.launch.py experiment_timestamp:=$timestamp &"
    echo "   ros2 launch launch_pkg robot_02_sim.launch.py experiment_timestamp:=$timestamp &"
    echo "   ros2 launch launch_pkg robot_03_sim.launch.py experiment_timestamp:=$timestamp &"
    exit 0
fi

# Lanzar robots especificados
for robot in "$@"; do
    if [[ $robot =~ ^[1-3]$ ]]; then
        echo "🚀 Lanzando robot_0$robot..."
        ros2 launch launch_pkg robot_0${robot}_sim.launch.py experiment_timestamp:=$timestamp &
        sleep 2
    else
        echo "⚠️  Robot '$robot' inválido. Solo: 1, 2, 3"
        exit 1
    fi
done

echo "✅ Robots lanzados con timestamp: $timestamp"