#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

# Cambiar título de la terminal
echo -ne "\033]0;📊 Task Queue Monitor - Live View\007"



## Monitor de Task Queue en tiempo real
## Muestra el estado de todas las tareas y estadísticas del sistema

# Función simple para maximizar ventana
maximize_window() {
    echo "📺 Maximizando ventana..."
    
    # Método más confiable: F11 para fullscreen
    if command -v xdotool >/dev/null 2>&1; then
        sleep 0.5
        xdotool key F11
        echo "✨ Ventana maximizada con F11"
    else
        echo "📦 Para maximización automática, instala: sudo apt install xdotool"
        echo "💡 Puedes maximizar manualmente con F11"
    fi
}

# Ejecutar maximización
maximize_window

python3 task_queue_monitor.py

echo "Presiona enter para salir..."
read  # O puedes poner $SHELL para quedarte en la terminal