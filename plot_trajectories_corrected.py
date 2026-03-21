#!/usr/bin/env python3
"""
Script CORREGIDO para generar gráficas de trayectorias y errores desde datos CSV de ros2bag_to_csv.py
- Usa posiciones REALES del gripper desde control_markers (no trayectorias planificadas)
- Genera archivos EPS individuales por robot
- Sin títulos en las gráficas
- Gráfica combinada de errores con subplots
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import re
from pathlib import Path
import argparse

def extract_gripper_positions(csv_path, robot_id):
    """Extrae POSICIONES REALES del gripper desde control_markers"""
    
    markers_file = csv_path / f"_robot_{robot_id:02d}_control_markers.csv"
    
    if not markers_file.exists():
        print(f"❌ No se encontró archivo de markers: {markers_file}")
        return None
    
    gripper_positions = []
    timestamps = []
    
    print(f"📊 Extrayendo posiciones reales del gripper: {markers_file}")
    df = pd.read_csv(markers_file)
    
    for idx, row in df.iterrows():
        data_str = str(row['data'])
        # Buscar el primer punto del error_vector que es la posición actual del gripper
        error_vector_pattern = r"error_vector.*?points=\[geometry_msgs\.msg\.Point\(x=([-\d\.e\-]+), y=([-\d\.e\-]+), z=([-\d\.e\-]+)\)"
        
        match = re.search(error_vector_pattern, data_str)
        if match:
            try:
                x, y, z = float(match.group(1)), float(match.group(2)), float(match.group(3))
                gripper_positions.append((x, y))
                timestamps.append(row['timestamp'])
            except ValueError:
                continue
    
    if not gripper_positions:
        print(f"❌ No se encontraron posiciones del gripper para robot {robot_id:02d}")
        return None
    
    print(f"✅ Encontradas {len(gripper_positions)} posiciones reales del gripper")
    
    return pd.DataFrame({
        'x': [pos[0] for pos in gripper_positions],
        'y': [pos[1] for pos in gripper_positions], 
        'timestamp': timestamps
    })

def extract_error_data_from_objectives(csv_path, robot_id):
    """Extrae datos de error usando objetivos POR FASE: objetivo_fase - posición gripper
    OPTIMIZADO: Para cada fase, toma solo el ÚLTIMO PUNTO para medir precisión final
    Error Y en METROS (no milímetros)
    Retorna: tuple (dataframe_error_data, lista_comparaciones_detalladas)"""
    
    # Archivos necesarios - USAR OBJECTIVE PARA OBTENER OBJETIVOS POR FASE
    objective_file = csv_path / f"_robot_{robot_id:02d}_objective.csv" 
    markers_file = csv_path / f"_robot_{robot_id:02d}_control_markers.csv"
    state_file = csv_path / f"_robot_{robot_id:02d}_robot_state.csv"
    
    if not objective_file.exists():
        print(f"❌ No se encontró archivo de objetivos por fase: {objective_file}")
        return None, None
        
    if not markers_file.exists():
        print(f"❌ No se encontró archivo de markers: {markers_file}")
        return None, None
        
    if not state_file.exists():
        print(f"❌ No se encontró archivo de estados: {state_file}")
        return None, None
        
    # Cargar datos
    print(f"📊 Calculando errores objetivo_vigente - gripper: {objective_file.name}")
    df_obj = pd.read_csv(objective_file)
    df_markers = pd.read_csv(markers_file)
    
    # NUEVA LÓGICA: Agrupar por fases y tomar solo los últimos 5 puntos de cada fase
    phase_errors = {}  # Diccionario para agrupar errores por fase
    
    # Primero, identificar todas las fases y sus objetivos
    phases = {}  # {robot_state: [(timestamp, obj_data), ...]}
    
    for _, obj_row in df_obj.iterrows():
        obj_data = str(obj_row['data'])
        obj_state_match = re.search(r"robot_state=(\d+)", obj_data)
        if obj_state_match:
            robot_state = int(obj_state_match.group(1))
            if robot_state != 99:  # Excluir estados de movimiento
                if robot_state not in phases:
                    phases[robot_state] = []
                phases[robot_state].append((obj_row['timestamp'], obj_data))
    
    print(f"✅ Identificadas {len(phases)} fases: {list(phases.keys())}")
    
    error_data = []
    timestamps = []
    detailed_comparisons = []  # Nueva lista para datos detallados
    
    # Para cada fase, procesar solo los últimos 5 markers
    for phase_state, phase_objectives in phases.items():
        if not phase_objectives:
            continue
            
        # Obtener el objetivo más reciente de esta fase
        phase_objectives.sort(key=lambda x: x[0])  # Ordenar por timestamp
        latest_obj_timestamp, latest_obj_data = phase_objectives[-1]
        
        # Extraer objetivo de esta fase
        obj_pattern = r"point=geometry_msgs\.msg\.Point\(x=([-\d\.e\-\+]+), y=([-\d\.e\-\+]+), z=([-\d\.e\-\+]+)\)"
        obj_match = re.search(obj_pattern, latest_obj_data)
        
        if not obj_match:
            continue
            
        try:
            obj_x, obj_y = float(obj_match.group(1)), float(obj_match.group(2))
        except ValueError:
            continue
        
        # Encontrar markers durante esta fase y tomar los últimos 5
        phase_markers = []
        for _, marker_row in df_markers.iterrows():
            marker_timestamp = marker_row['timestamp']
            
            # Determinar si este marker pertenece a esta fase
            # (buscar el objetivo más reciente para este timestamp)
            current_objective = None
            for _, obj_row in df_obj.iterrows():
                if obj_row['timestamp'] <= marker_timestamp:
                    current_objective = obj_row
                else:
                    break
                    
            if current_objective is not None:
                current_obj_data = str(current_objective['data'])
                current_state_match = re.search(r"robot_state=(\d+)", current_obj_data)
                if current_state_match and int(current_state_match.group(1)) == phase_state:
                    marker_data = str(marker_row['data'])
                    # Extraer posición del gripper desde target_point (posición real)
                    gripper_pattern = r"ns='target_point'.*?position=geometry_msgs\.msg\.Point\(x=([-\d\.e\-]+), y=([-\d\.e\-]+), z=([-\d\.e\-]+)\)"
                    gripper_match = re.search(gripper_pattern, marker_data)
                    
                    if gripper_match:
                        try:
                            gripper_x, gripper_y = float(gripper_match.group(1)), float(gripper_match.group(2))
                            phase_markers.append({
                                'timestamp': marker_timestamp,
                                'gripper_x': gripper_x,
                                'gripper_y': gripper_y
                            })
                        except ValueError:
                            continue
        
        # Tomar solo el ÚLTIMO punto de esta fase
        if phase_markers:
            phase_markers.sort(key=lambda x: x['timestamp'])
            last_marker = phase_markers[-1]  # Solo el último punto
            
            print(f"  Fase {phase_state}: {len(phase_markers)} puntos total, usando último 1")
            
            # Calcular error: objetivo_vigente - posición_actual  
            error_x = obj_x - last_marker['gripper_x']
            error_y = obj_y - last_marker['gripper_y']
            error_total = np.sqrt(error_x**2 + error_y**2)
            
            # 🔍 IMPRIMIR DETALLES DE CADA COMPARACIÓN
            print(f"    📍 Comparación Fase {phase_state}:")
            print(f"      Objetivo:  X={obj_x:.6f}m, Y={obj_y:.6f}m")
            print(f"      Real:      X={last_marker['gripper_x']:.6f}m, Y={last_marker['gripper_y']:.6f}m")
            print(f"      Error:     X={error_x:.6f}m ({error_x*1000:.1f}mm), Y={error_y:.6f}m ({error_y*1000:.1f}mm)")
            print(f"      Error Total: {error_total:.6f}m ({error_total*1000:.1f}mm)")
            print(f"      Timestamp: {last_marker['timestamp']}")
            
            # Guardar datos detallados para archivo
            detailed_comparisons.append({
                'fase': phase_state,
                'obj_x': obj_x,
                'obj_y': obj_y,
                'real_x': last_marker['gripper_x'],
                'real_y': last_marker['gripper_y'],
                'error_x': error_x,
                'error_y': error_y,
                'error_total': error_total,
                'timestamp': last_marker['timestamp']
            })
            
            error_data.append({'error_x': error_x, 'error_y': error_y, 'error_total': error_total})
            timestamps.append(last_marker['timestamp'])
    
    if not error_data:
        print(f"❌ No se pudieron calcular errores para robot {robot_id:02d}")
        return None, None
        
    print(f"✅ Calculados {len(error_data)} puntos de error objetivo_vigente-gripper")
    
    error_df = pd.DataFrame({
        'error_x': [e['error_x'] for e in error_data],
        'error_y': [e['error_y'] for e in error_data], 
        'error_total': [e['error_total'] for e in error_data],
        'timestamp': timestamps
    })
    
    return error_df, detailed_comparisons

def extract_continuous_error_data(csv_path, robot_id):
    """Extrae datos de error CONTINUO para gráficas: todos los puntos vs tiempo
    Calcula error = objetivo_vigente - posición_real para cada timestamp
    Para gráficas de error vs tiempo (NO para MSE/RMSE)"""
    
    # Archivos necesarios
    objective_file = csv_path / f"_robot_{robot_id:02d}_objective.csv" 
    markers_file = csv_path / f"_robot_{robot_id:02d}_control_markers.csv"
    
    if not objective_file.exists() or not markers_file.exists():
        print(f"❌ Archivos no encontrados para robot {robot_id:02d}")
        return None
        
    print(f"📊 Calculando errores continuos (gráficas): robot {robot_id:02d}")
    df_obj = pd.read_csv(objective_file)
    df_markers = pd.read_csv(markers_file)
    
    error_points = []
    
    # Para cada marker, encontrar el objetivo vigente y calcular error
    for _, marker_row in df_markers.iterrows():
        marker_timestamp = marker_row['timestamp']
        marker_data = str(marker_row['data'])
        
        # Extraer posición real del gripper
        gripper_pattern = r"ns='target_point'.*?position=geometry_msgs\.msg\.Point\(x=([-\d\.e\-]+), y=([-\d\.e\-]+), z=([-\d\.e\-]+)\)"
        gripper_match = re.search(gripper_pattern, marker_data)
        
        if not gripper_match:
            continue
            
        try:
            gripper_x, gripper_y = float(gripper_match.group(1)), float(gripper_match.group(2))
        except ValueError:
            continue
            
        # Encontrar objetivo vigente para este timestamp (más reciente)
        current_objective = None
        for _, obj_row in df_obj.iterrows():
            if obj_row['timestamp'] <= marker_timestamp:
                current_objective = obj_row
            else:
                break
                
        if current_objective is None:
            continue
            
        obj_data = str(current_objective['data'])
        obj_pattern = r"point=geometry_msgs\.msg\.Point\(x=([-\d\.e\-\+]+), y=([-\d\.e\-\+]+), z=([-\d\.e\-\+]+)\)"
        obj_match = re.search(obj_pattern, obj_data)
        
        if obj_match:
            try:
                obj_x, obj_y = float(obj_match.group(1)), float(obj_match.group(2))
                
                # Calcular error continuo
                error_x = obj_x - gripper_x
                error_y = obj_y - gripper_y
                error_total = np.sqrt(error_x**2 + error_y**2)
                
                error_points.append({
                    'timestamp': marker_timestamp,
                    'error_x': error_x,
                    'error_y': error_y,
                    'error_total': error_total
                })
            except ValueError:
                continue
    
    if not error_points:
        print(f"❌ No se pudieron calcular errores continuos para robot {robot_id:02d}")
        return None
        
    print(f"✅ Calculados {len(error_points)} puntos de error continuo")
    
    return pd.DataFrame(error_points)

def extract_phase_changes(csv_path, robot_id):
    """Extrae timestamps de cambios de fase desde robot_state.csv
    
    Estados de fase soportados: 0-9 (IDLE, APPROACH, TARGET, MANIPULATION, etc.)
    """
    
    state_file = csv_path / f"_robot_{robot_id:02d}_robot_state.csv"
    
    if not state_file.exists():
        print(f"❌ No se encontró archivo de estado: {state_file}")
        return []
    
    phase_changes = []
    
    print(f"📊 Extrayendo cambios de fase: {state_file}")
    df = pd.read_csv(state_file)
    
    current_phase = None
    
    for idx, row in df.iterrows():
        data_str = str(row['data'])
        # Buscar robot_state=X en el mensaje
        state_pattern = r"robot_state=(\d+)"
        
        match = re.search(state_pattern, data_str)
        if match:
            phase_state = int(match.group(1))
            
            # Detectar cambio de fase
            if current_phase != phase_state:
                if current_phase is not None:  # No registrar el primer estado
                    phase_changes.append({
                        'timestamp': row['timestamp'],
                        'from_phase': current_phase,
                        'to_phase': phase_state,
                        'phase_name': f'ST{phase_state}'
                    })
                current_phase = phase_state
    
    print(f"✅ Encontrados {len(phase_changes)} cambios de fase para robot {robot_id:02d}")
    
    return phase_changes

def extract_objectives_by_phase(csv_path, robot_id):
    """Extrae objetivos únicos por fase para mostrar en gráficas de trayectoria"""
    
    objective_file = csv_path / f"_robot_{robot_id:02d}_objective.csv"
    
    if not objective_file.exists():
        print(f"⚠️ No se encontró archivo de objetivos: {objective_file}")
        return []
    
    objectives = []
    seen_objectives = set()  # Para evitar objetivos duplicados
    
    print(f"📊 Extrayendo objetivos por fase: {objective_file}")
    df = pd.read_csv(objective_file)
    
    for _, row in df.iterrows():
        data_str = str(row['data'])
        
        # Extraer coordenadas del objetivo
        obj_pattern = r"point=geometry_msgs\.msg\.Point\(x=([-\d\.e\-\+]+), y=([-\d\.e\-\+]+), z=([-\d\.e\-\+]+)\)"
        obj_match = re.search(obj_pattern, data_str)
        
        # Extraer robot_state del objetivo (para identificar la fase)
        state_pattern = r"robot_state=(\d+)"
        state_match = re.search(state_pattern, data_str)
        
        if obj_match and state_match:
            try:
                obj_x = float(obj_match.group(1))
                obj_y = float(obj_match.group(2))
                robot_state = int(state_match.group(1))
                
                # Crear clave única para evitar duplicados
                obj_key = (round(obj_x, 3), round(obj_y, 3), robot_state)
                
                if obj_key not in seen_objectives:
                    objectives.append({
                        'x': obj_x,
                        'y': obj_y,
                        'robot_state': robot_state,
                        'timestamp': row['timestamp']
                    })
                    seen_objectives.add(obj_key)
            except ValueError:
                continue
    
    print(f"✅ Encontrados {len(objectives)} objetivos únicos para robot {robot_id:02d}")
    
    return objectives

def get_first_task_timestamp(csv_path, num_robots):
    """Encuentra el timestamp GLOBAL de la primera tarea asignada en todo el experimento"""
    
    first_timestamp = None
    
    for robot_id in range(1, num_robots + 1):
        task_file = csv_path / f"_robot_{robot_id:02d}_task_assigned.csv"
        
        if task_file.exists():
            try:
                df = pd.read_csv(task_file)
                if len(df) > 0:
                    robot_first_task = df['timestamp'].iloc[0]
                    if first_timestamp is None or robot_first_task < first_timestamp:
                        first_timestamp = robot_first_task
            except:
                continue
    
    if first_timestamp is None:
        print("⚠️ No se encontraron tareas asignadas")
        return None
    else:
        print(f"✅ TIEMPO 0 GLOBAL: {first_timestamp} nanoseconds")
        print(f"📅 Todos los robots usarán este tiempo como referencia")
    
    return first_timestamp

def get_global_error_range(csv_path, num_robots):
    """Calcula el rango global de errores de todos los robots para escalas uniformes"""
    
    all_error_x = []
    all_error_y = []
    
    for robot_id in range(1, num_robots + 1):
        error_data = extract_continuous_error_data(csv_path, robot_id)  # ERRORES CONTINUOS para gráficas
        if error_data is not None:
            all_error_x.extend(error_data['error_x'].tolist())
            all_error_y.extend(error_data['error_y'].tolist())
    
    if all_error_x and all_error_y:
        # Calcular rango con margen del 5%
        min_x, max_x = min(all_error_x), max(all_error_x) 
        min_y, max_y = min(all_error_y), max(all_error_y)
        
        range_x = max_x - min_x
        range_y = max_y - min_y
        
        # Usar el rango más amplio para ambos ejes (mantener proporción)
        max_range = max(range_x, range_y)
        margin = max_range * 0.05  # 5% de margen
        
        global_min = min(min_x, min_y) - margin
        global_max = max(max_x, max_y) + margin
        
        print(f"📏 Rango global de errores: [{global_min:.4f}, {global_max:.4f}] metros")
        return global_min, global_max
    
    return None, None

def plot_trajectories(experiment_name, num_robots):
    """Genera plots de trayectorias INDIVIDUALES para cada robot"""
    
    csv_path = Path("csv") / experiment_name
    if not csv_path.exists():
        print(f"❌ No se encontró la carpeta: {csv_path}")
        return 0
    
    # Configuración de colores profesionales
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b']
    
    successful_robots = 0
    output_dir = Path("trajectory_plots")
    output_dir.mkdir(exist_ok=True)
    
    for robot_id in range(1, num_robots + 1):
        print(f"\\n🤖 Procesando Robot {robot_id:02d}...")
        
        traj_data = extract_gripper_positions(csv_path, robot_id)
        if traj_data is not None:
            # Crear figura INDIVIDUAL para este robot
            plt.style.use('seaborn-v0_8-whitegrid')
            fig, ax = plt.subplots(figsize=(10, 8))
            
            color = colors[(robot_id - 1) % len(colors)]
            
            # Plot trayectoria INDIVIDUAL
            ax.plot(traj_data['x'], traj_data['y'], 
                   color=color, linewidth=2.5, alpha=0.8,
                   label=f'Robot {robot_id:02d}')
            
            # 🎯 AGREGAR OBJETIVOS POR FASE como marcas X
            objectives = extract_objectives_by_phase(csv_path, robot_id)
            if objectives:
                for i, obj in enumerate(objectives):
                    ax.scatter(obj['x'], obj['y'], 
                             color='red', s=150, marker='x', linewidths=3,
                             zorder=15, 
                             label='Phase objectives' if i == 0 else "")
            
            # Marcar inicio y fin
            ax.scatter(traj_data['x'].iloc[0], traj_data['y'].iloc[0], 
                      color=color, s=100, marker='o', 
                      edgecolors='black', linewidths=2, zorder=10, label='Start')
            
            ax.scatter(traj_data['x'].iloc[-1], traj_data['y'].iloc[-1], 
                      color=color, s=100, marker='s', 
                      edgecolors='black', linewidths=2, zorder=10, label='End')
            
            # Configurar plot INDIVIDUAL (sin título)
            ax.set_xlabel('Position X [m]', fontsize=14)
            ax.set_ylabel('Position Y [m]', fontsize=14)
            # NO TÍTULO como solicitado
            
            # Grid y aspectos profesionales
            ax.grid(True, alpha=0.3)
            ax.set_aspect('equal')
            ax.legend(fontsize=12, loc='upper right')  # Agregar leyenda
            
            # 🎯 Fijar ejes X e Y con tamaño del workspace
            workspace_xlim = [-3.0, 3.0]  # Workspace X limits [m]
            workspace_ylim = [-2.0, 2.0]  # Workspace Y limits [m]
            ax.set_xlim(workspace_xlim)
            ax.set_ylim(workspace_ylim)
            
            plt.tight_layout()
            
            # Guardar archivo EPS INDIVIDUAL
            output_file = output_dir / f"{experiment_name}_robot_{robot_id:02d}_trajectory.eps"
            
            plt.savefig(output_file, format='eps', dpi=300, bbox_inches='tight', 
                        facecolor='white', edgecolor='none')
            plt.close()
            
            print(f"🎯 Robot {robot_id:02d} trayectoria guardada en: {output_file}")
            successful_robots += 1
        else:
            print(f"⚠️  Robot {robot_id:02d} sin datos de trayectoria")
    
    return successful_robots

def plot_errors(experiment_name, num_robots):
    """Genera plots de errores lineales INDIVIDUALES para cada robot"""
    
    csv_path = Path("csv") / experiment_name
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b']
    
    # Obtener timestamp de la primera tarea para usar como tiempo 0
    first_task_timestamp = get_first_task_timestamp(csv_path, num_robots)
    
    # 📏 Calcular rango global para escalas uniformes
    global_min, global_max = get_global_error_range(csv_path, num_robots)
    
    successful_robots = 0
    output_dir = Path("trajectory_plots")
    output_dir.mkdir(exist_ok=True)
    
    for robot_id in range(1, num_robots + 1):
        print(f"\\n📈 Analizando errores Robot {robot_id:02d}...")
        
        error_data = extract_continuous_error_data(csv_path, robot_id)  # ERRORES CONTINUOS para gráficas
        if error_data is not None:
            # Crear figura INDIVIDUAL para este robot (format delgado)
            plt.style.use('seaborn-v0_8-whitegrid')
            fig, ax = plt.subplots(figsize=(12, 4))
            
            color = colors[(robot_id - 1) % len(colors)]
            
            # Convertir timestamp a tiempo relativo desde primera tarea GLOBAL (tiempo 0)
            if first_task_timestamp is not None:
                time_rel = (error_data['timestamp'] - first_task_timestamp) / 1e9
            else:
                print(f"⚠️ Sin timestamp global, usando timestamp del primer error del robot {robot_id:02d}")
                time_rel = (error_data['timestamp'] - error_data['timestamp'].iloc[0]) / 1e9
            
            ax.plot(time_rel, error_data['error_x'],  # Error X en metros
                   color='red', linewidth=2, alpha=0.8,
                   label=f'Robot {robot_id:02d} - Error X [m]')
            
            ax.plot(time_rel, error_data['error_y'],  # Error Y en metros
                   color='blue', linewidth=2, alpha=0.8,
                   label=f'Robot {robot_id:02d} - Error Y [m]')
            
            # 🔄 AGREGAR LÍNEAS VERTICALES de cambios de fase (sin etiquetas)
            phase_changes = extract_phase_changes(csv_path, robot_id)
            for i, change in enumerate(phase_changes):
                if first_task_timestamp is not None:
                    change_time = (change['timestamp'] - first_task_timestamp) / 1e9
                else:
                    change_time = (change['timestamp'] - error_data['timestamp'].iloc[0]) / 1e9
                ax.axvline(x=change_time, linestyle='--', linewidth=1.5, 
                          alpha=0.7, color='black',
                          label=f'Phase changes' if i == 0 else "")
            
            # Configurar plot de errores INDIVIDUAL (sin título)
            ax.set_xlabel('Time [s]', fontsize=14)
            ax.set_ylabel('Linear Error [m]', fontsize=14)
            # NO TÍTULO como solicitado
            
            ax.grid(True, alpha=0.3)
            ax.legend(fontsize=12)
            
            # Asegurar que el eje X comience en 0
            ax.set_xlim(left=0)
            
            # 📏 Aplicar escala uniforme si está disponible
            if global_min is not None and global_max is not None:
                ax.set_ylim(global_min, global_max)
            
            plt.tight_layout()
            
            # Guardar archivo EPS INDIVIDUAL
            error_file = output_dir / f"{experiment_name}_robot_{robot_id:02d}_errors.eps"
            
            plt.savefig(error_file, format='eps', dpi=300, bbox_inches='tight',
                        facecolor='white', edgecolor='none')
            plt.close()
            
            print(f"📊 Robot {robot_id:02d} errores guardados en: {error_file}")
            successful_robots += 1
        else:
            print(f"⚠️  Robot {robot_id:02d} sin datos de error")
    
    return successful_robots

def plot_errors_combined(experiment_name, num_robots):
    """Genera UNA gráfica con subplots de errores de todos los robots"""
    
    csv_path = Path("csv") / experiment_name
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b']
    
    # Obtener timestamp de la primera tarea para usar como tiempo 0
    first_task_timestamp = get_first_task_timestamp(csv_path, num_robots)
    
    # 📏 Calcular rango global para escalas uniformes
    global_min, global_max = get_global_error_range(csv_path, num_robots)
    
    # Crear figura con subplots verticales
    fig, axes = plt.subplots(num_robots, 1, figsize=(12, 3*num_robots), sharex=True)
    if num_robots == 1:
        axes = [axes]  # Hacer iterable si solo hay 1 robot
    
    plt.style.use('seaborn-v0_8-whitegrid') 
    
    successful_robots = 0
    letters = ['a)', 'b)', 'c)', 'd)', 'e)', 'f)']
    
    for robot_id in range(1, num_robots + 1):
        ax = axes[robot_id - 1]
        
        print(f"📈 Analizando errores Robot {robot_id:02d} para subplot...")
        
        error_data = extract_continuous_error_data(csv_path, robot_id)  # ERRORES CONTINUOS para gráficas
        if error_data is not None:
            color = colors[(robot_id - 1) % len(colors)]
            
            # Convertir timestamp a tiempo relativo desde primera tarea GLOBAL (tiempo 0)
            if first_task_timestamp is not None:
                time_rel = (error_data['timestamp'] - first_task_timestamp) / 1e9
            else:
                print(f"⚠️ Sin timestamp global para robot {robot_id:02d}")
                time_rel = (error_data['timestamp'] - error_data['timestamp'].iloc[0]) / 1e9
            
            ax.plot(time_rel, error_data['error_x'],  # Error X en metros
                   color='red', linewidth=2, alpha=0.8, label='Error X [m]')
            
            ax.plot(time_rel, error_data['error_y'],  # Error Y en metros
                   color='blue', linewidth=2, alpha=0.8, label='Error Y [m]')
            
            # 🔄 AGREGAR LÍNEAS VERTICALES de cambios de fase (sin etiquetas)
            phase_changes = extract_phase_changes(csv_path, robot_id)
            for change in phase_changes:
                if first_task_timestamp is not None:
                    change_time = (change['timestamp'] - first_task_timestamp) / 1e9
                else:
                    change_time = (change['timestamp'] - error_data['timestamp'].iloc[0]) / 1e9
                ax.axvline(x=change_time, linestyle='--', linewidth=1.5, 
                          alpha=0.7, color='black')
            
            # Título DEBAJO del subplot con eje X apropiado
            if robot_id == num_robots:  # Solo el último subplot
                ax.set_xlabel(f"{letters[robot_id-1]} Linear Errors of Robot {robot_id}\nTime [s]", fontsize=12, fontweight='bold')
            else:  # Los demás subplots
                ax.set_xlabel(f"{letters[robot_id-1]} Linear Errors of Robot {robot_id}", fontsize=12, fontweight='bold')
            ax.set_ylabel('Error [m]', fontsize=12)
            ax.grid(True, alpha=0.3)
            ax.legend(loc='upper right', fontsize=10)
            
            successful_robots += 1
        else:
            # Si no hay datos, subplot vacío con mensaje
            ax.text(0.5, 0.5, f'No data for Robot {robot_id:02d}', 
                   transform=ax.transAxes, ha='center', va='center', fontsize=12)
            if robot_id == num_robots:  # Solo el último subplot
                ax.set_xlabel(f"{letters[robot_id-1]} Linear Errors of Robot {robot_id}\nTime [s]", fontsize=12, fontweight='bold')
            else:  # Los demás subplots
                ax.set_xlabel(f"{letters[robot_id-1]} Linear Errors of Robot {robot_id}", fontsize=12, fontweight='bold')
            ax.set_ylabel('Error [m]', fontsize=12)
    
    # Asegurar que todos los ejes X comiencen en 0
    for ax in axes:
        ax.set_xlim(left=0)
        
    # 📏 Aplicar escalas uniformes Y a todos los subplots
    if global_min is not None and global_max is not None:
        for ax in axes:
            ax.set_ylim(global_min, global_max)
    
    plt.tight_layout()
    
    # Guardar archivo EPS combinado
    output_dir = Path("trajectory_plots")
    output_dir.mkdir(exist_ok=True)
    combined_file = output_dir / f"{experiment_name}_errors_combined.eps"
    
    plt.savefig(combined_file, format='eps', dpi=300, bbox_inches='tight',
                facecolor='white', edgecolor='none')
    plt.close()
    
    print(f"📊 Errores combinados guardados en: {combined_file}")
    return successful_robots

def calculate_error_metrics(experiment_name, num_robots):
    """Calcula MSE y RMSE para errores X e Y de cada robot"""
    
    csv_path = Path("csv") / experiment_name
    
    print("=" * 60)
    print("📊 CALCULANDO MÉTRICAS DE ERROR (MSE y RMSE)")
    print("=" * 60)
    
    results = []
    all_detailed_comparisons = {}  # Para guardar comparaciones de todos los robots
    
    for robot_id in range(1, num_robots + 1):
        print(f"\n🤖 Analizando métricas Robot {robot_id:02d}...")
        
        error_data, detailed_comparisons = extract_error_data_from_objectives(csv_path, robot_id)
        if error_data is not None:
            # Calcular MSE (Mean Squared Error)
            mse_x = np.mean(error_data['error_x']**2)
            mse_y = np.mean(error_data['error_y']**2)
            mse_total = np.mean(error_data['error_total']**2)
            
            # Calcular RMSE (Root Mean Squared Error)
            rmse_x = np.sqrt(mse_x)
            rmse_y = np.sqrt(mse_y)
            rmse_total = np.sqrt(mse_total)
            
            # Otras métricas útiles
            mean_abs_error_x = np.mean(np.abs(error_data['error_x']))
            mean_abs_error_y = np.mean(np.abs(error_data['error_y']))
            max_abs_error_x = np.max(np.abs(error_data['error_x']))
            max_abs_error_y = np.max(np.abs(error_data['error_y']))
            
            results.append({
                'robot_id': robot_id,
                'mse_x': mse_x,
                'mse_y': mse_y,
                'mse_total': mse_total,
                'rmse_x': rmse_x,
                'rmse_y': rmse_y,
                'rmse_total': rmse_total,
                'mae_x': mean_abs_error_x,
                'mae_y': mean_abs_error_y,
                'max_error_x': max_abs_error_x,
                'max_error_y': max_abs_error_y,
                'num_points': len(error_data)
            })
            
            # Guardar comparaciones detalladas
            all_detailed_comparisons[robot_id] = detailed_comparisons
            
            print(f"✅ Robot {robot_id:02d} métricas calculadas ({len(error_data)} puntos)")
        else:
            print(f"⚠️ Robot {robot_id:02d} sin datos de error")
    
    if results:
        print("\n" + "=" * 80)
        print("📊 RESUMEN DE MÉTRICAS DE ERROR")
        print("=" * 80)
        print(f"{'Robot':<8} {'MSE_X':<12} {'MSE_Y':<12} {'RMSE_X':<12} {'RMSE_Y':<12} {'Puntos':<8}")
        print("-" * 80)
        
        for result in results:
            print(f"{result['robot_id']:02d}       "
                  f"{result['mse_x']:<12.6f} "
                  f"{result['mse_y']:<12.6f} "
                  f"{result['rmse_x']:<12.6f} "
                  f"{result['rmse_y']:<12.6f} "
                  f"{result['num_points']:<8d}")
        
        print("\n" + "=" * 80)
        print("📊 MÉTRICAS ADICIONALES")
        print("=" * 80)
        print(f"{'Robot':<8} {'MAE_X':<12} {'MAE_Y':<12} {'MaxErr_X':<12} {'MaxErr_Y':<12} {'RMSE_Total':<12}")
        print("-" * 80)
        
        for result in results:
            print(f"{result['robot_id']:02d}       "
                  f"{result['mae_x']:<12.6f} "
                  f"{result['mae_y']:<12.6f} "
                  f"{result['max_error_x']:<12.6f} "
                  f"{result['max_error_y']:<12.6f} "
                  f"{result['rmse_total']:<12.6f}")
        
        # Guardar resultados en CSV
        output_dir = Path("trajectory_plots")
        output_dir.mkdir(exist_ok=True)
        metrics_file = output_dir / f"{experiment_name}_error_metrics.csv"
        
        import csv
        with open(metrics_file, 'w', newline='') as csvfile:
            fieldnames = ['robot_id', 'mse_x', 'mse_y', 'mse_total', 'rmse_x', 'rmse_y', 'rmse_total',
                         'mae_x', 'mae_y', 'max_error_x', 'max_error_y', 'num_points']
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            for result in results:
                writer.writerow(result)
        
        print(f"\n📂 Métricas guardadas en: {metrics_file}")
        
        # Generar archivo TXT detallado con todas las comparaciones
        from datetime import datetime
        detailed_file = output_dir / f"{experiment_name}_detailed_comparisons.txt"
        
        with open(detailed_file, 'w', encoding='utf-8') as f:
            f.write(f"ANÁLISIS DETALLADO DE ERRORES\n")
            f.write(f"Experimento: {experiment_name}\n")
            f.write(f"Fecha: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"="*80 + "\n\n")
            
            for robot_id, comparisons in all_detailed_comparisons.items():
                f.write(f"🤖 ROBOT {robot_id:02d}\n")
                f.write(f"-"*40 + "\n")
                
                # Escribir comparaciones detalladas
                if comparisons:
                    for comparison in comparisons:
                        f.write(f"📍 Comparación Fase {comparison['fase']}:\n")
                        f.write(f"  Objetivo:     X={comparison['obj_x']:.6f}m, Y={comparison['obj_y']:.6f}m\n")
                        f.write(f"  Real:         X={comparison['real_x']:.6f}m, Y={comparison['real_y']:.6f}m\n")
                        f.write(f"  Error:        X={comparison['error_x']:.6f}m ({comparison['error_x']*1000:.1f}mm)\n")
                        f.write(f"                Y={comparison['error_y']:.6f}m ({comparison['error_y']*1000:.1f}mm)\n")
                        f.write(f"  Error Total:  {comparison['error_total']:.6f}m ({comparison['error_total']*1000:.1f}mm)\n")
                        f.write(f"  Timestamp:    {comparison['timestamp']}\n\n")
                else:
                    f.write("  Sin comparaciones disponibles\n\n")
                
                # Escribir métricas finales
                robot_metrics = next((r for r in results if r['robot_id'] == robot_id), None)
                if robot_metrics:
                    f.write(f"📊 MÉTRICAS FINALES:\n")
                    f.write(f"  MSE_X:        {robot_metrics['mse_x']:.6f}\n")
                    f.write(f"  MSE_Y:        {robot_metrics['mse_y']:.6f}\n")
                    f.write(f"  RMSE_X:       {robot_metrics['rmse_x']:.6f} ({robot_metrics['rmse_x']*1000:.1f}mm)\n")
                    f.write(f"  RMSE_Y:       {robot_metrics['rmse_y']:.6f} ({robot_metrics['rmse_y']*1000:.1f}mm)\n")
                    f.write(f"  RMSE_Total:   {robot_metrics['rmse_total']:.6f} ({robot_metrics['rmse_total']*1000:.1f}mm)\n")
                    f.write(f"  MAE_X:        {robot_metrics['mae_x']:.6f} ({robot_metrics['mae_x']*1000:.1f}mm)\n")
                    f.write(f"  MAE_Y:        {robot_metrics['mae_y']:.6f} ({robot_metrics['mae_y']*1000:.1f}mm)\n")
                    f.write(f"  Max_Error_X:  {robot_metrics['max_error_x']:.6f} ({robot_metrics['max_error_x']*1000:.1f}mm)\n")
                    f.write(f"  Max_Error_Y:  {robot_metrics['max_error_y']:.6f} ({robot_metrics['max_error_y']*1000:.1f}mm)\n")
                    f.write(f"  Puntos:       {robot_metrics['num_points']}\n\n")
                
                f.write(f"="*80 + "\n\n")
        
        print(f"📄 Comparaciones detalladas guardadas en: {detailed_file}")
        
    else:
        print("❌ No se pudieron calcular métricas para ningún robot")
    
    return results

def main():
    parser = argparse.ArgumentParser(description='Generate trajectory and error plots from rosbag CSV data')
    parser.add_argument('--experiment', type=str, default='experiment_3',
                        help='Experiment name (default: experiment_3)')
    parser.add_argument('--robots', type=int, default=3,
                        help='Number of robots (default: 3)')
    parser.add_argument('--errors', action='store_true',
                        help='Generate error plots in addition to trajectories')
    parser.add_argument('--metrics', action='store_true',
                        help='Calculate MSE and RMSE metrics for each robot')
    
    args = parser.parse_args()
    
    print("🚀 Generador de Gráficas Profesionales - POSICIONES REALES DEL GRIPPER")
    print(f"📊 Experimento: {args.experiment}")
    print(f"🤖 Robots: {args.robots}")
    print(f"📈 Generar errores: {'Sí' if args.errors else 'No'}")
    print(f"📊 Calcular métricas: {'Sí' if args.metrics else 'No'}")
    print("")
    
    # Generar trayectorias
    print("=" * 60)
    print("🎯 GENERANDO GRÁFICAS DE TRAYECTORIAS REALES")  
    print("=" * 60)
    traj_success = plot_trajectories(args.experiment, args.robots)
    
    # Generar errores si se solicita
    if args.errors:
        print("\\n" + "=" * 60)
        print("📈 GENERANDO GRÁFICAS DE ERRORES INDIVIDUALES")
        print("=" * 60)
        error_success = plot_errors(args.experiment, args.robots)
        
        print("\\n" + "=" * 60)
        print("📈 GENERANDO GRÁFICA COMBINADA DE ERRORES")
        print("=" * 60)
        combined_success = plot_errors_combined(args.experiment, args.robots)
    
    # Calcular métricas si se solicita
    if args.metrics:
        metrics_results = calculate_error_metrics(args.experiment, args.robots)
        
    # Resumen final
    if args.errors and args.metrics:
        print(f"\\n🎯 ¡Generación completa!")
        print(f"📊 Trayectorias: {traj_success} archivos EPS individuales generados")
        print(f"📈 Errores individuales: {error_success} archivos EPS generados") 
        print(f"📈 Errores combinados: 1 archivo EPS con {combined_success} robots"
              f"\\n📊 Métricas: Calculadas para {len(metrics_results)} robots")
    elif args.errors:
        print(f"\\n🎯 ¡Generación completa!")
        print(f"📊 Trayectorias: {traj_success} archivos EPS individuales generados")
        print(f"📈 Errores individuales: {error_success} archivos EPS generados") 
        print(f"📈 Errores combinados: 1 archivo EPS con {combined_success} robots")
    elif args.metrics:
        print(f"\\n🎯 ¡Análisis completo!")
        print(f"📊 Trayectorias: {traj_success} archivos EPS individuales generados")
        print(f"📊 Métricas: Calculadas para {len(metrics_results)} robots")
    else:
        print(f"\\n🎯 ¡Trayectorias generadas!")
        print(f"📊 Procesados: {traj_success} archivos EPS individuales")
    
    print(f"📁 Formato: EPS vectorial (escalable para publicación)")
    print(f"📂 Ubicación: trajectory_plots/")
    if not args.errors and not args.metrics:
        print(f"💡 Usa --errors para generar también gráficas de error individuales")
        print(f"💡 Usa --metrics para calcular MSE y RMSE de cada robot")

if __name__ == "__main__":
    main()
