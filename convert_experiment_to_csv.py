#!/usr/bin/env python3
"""
Script wrapper para convertir rosbags de experimento a CSV usando ros2bag_to_csv.py

Usage:
    python3 convert_experiment_to_csv.py multi_robot_exp_20260317_101906
    
    # O con autocompletado:
    python3 convert_experiment_to_csv.py [TAB] 

Este script:
1. Convierte el rosbag a CSV usando ros2bag_to_csv.py
2. Organiza los CSVs en formato compatible con plot_trajectories_professional.py
3. Extrae datos de errores lineales de los topics robot_state

Autor: Generated for Robotics_Cyber_Physical_LAB
"""

import argparse
import os
import sys
import subprocess
from pathlib import Path
import pandas as pd

def main():
    parser = argparse.ArgumentParser(description='Convertir rosbag de experimento a CSV')
    parser.add_argument('experiment', help='Nombre del experimento (ej: multi_robot_exp_20260317_101906)')
    
    args = parser.parse_args()
    
    # Rutas
    workspace_dir = Path.home() / "Robotics_Cyber_Physical_LAB" 
    bags_dir = workspace_dir / "bags" / args.experiment
    csv_dir = workspace_dir / "csv" / args.experiment
    
    # Verificar que existe el rosbag
    if not bags_dir.exists():
        print(f"❌ Error: No existe el rosbag en {bags_dir}")
        print(f"📁 Rosbags disponibles:")
        bags_parent = workspace_dir / "bags"
        if bags_parent.exists():
            for bag in sorted(bags_parent.glob("multi_robot_exp_*")):
                print(f"   - {bag.name}")
        sys.exit(1)
    
    print(f"🚀 Convirtiendo experimento: {args.experiment}")
    print(f"📂 Rosbag: {bags_dir}")
    print(f"📁 CSV output: {csv_dir}")
    
    # Crear directorio CSV
    csv_dir.mkdir(parents=True, exist_ok=True)
    
    # Convertir rosbag a CSV usando ros2bag_to_csv.py
    print("\n📋 Ejecutando ros2bag_to_csv.py...")
    cmd = [
        "python3", 
        str(workspace_dir / "ros2bag_to_csv.py"), 
        str(bags_dir), 
        str(csv_dir)
    ]
    
    try:
        result = subprocess.run(cmd, cwd=workspace_dir, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"❌ Error ejecutando ros2bag_to_csv.py:")
            print(result.stderr)
            sys.exit(1)
        print("✅ Conversión completada!")
        
    except Exception as e:
        print(f"❌ Error: {e}")
        sys.exit(1)
    
    # Listar archivos CSV generados
    csv_files = list(csv_dir.glob("*.csv"))
    print(f"\n📊 Archivos CSV generados ({len(csv_files)}):")
    for csv_file in sorted(csv_files):
        file_size = csv_file.stat().st_size
        print(f"   📄 {csv_file.name} ({file_size} bytes)")
    
    # Instrucciones para generar gráficas
    print(f"\n🎨 Para generar gráficas:")
    print(f"python3 plot_trajectories_professional.py --experiment {args.experiment}")
    print(f"python3 plot_trajectories_professional.py --experiment {args.experiment} --errors")
    
    # Información adicional
    tf_files = list(csv_dir.glob("*tf*.csv"))
    robot_state_files = list(csv_dir.glob("*robot_state*.csv"))
    
    print(f"\n📈 Datos disponibles:")
    print(f"   🤖 TF data: {len(tf_files)} archivo(s)")
    print(f"   📊 Robot states: {len(robot_state_files)} archivo(s)")
    
    if tf_files:
        print("   ✅ Datos de posición disponibles para gráficas de trayectorias")
    if robot_state_files:
        print("   ✅ Datos de estado disponibles para gráficas de errores")

if __name__ == "__main__":
    main()