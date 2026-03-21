# 🚀 Script de Automatización: BAG → CSV → GRÁFICAS

## 📋 Descripción

Script automatizado que convierte rosbags de experimentos a CSV y genera gráficas profesionales en formato EPS.

## 🎯 Uso

```bash
# Ejecutar desde la raíz del workspace
./scripts_deck/bag_to_plots.sh
```

El script te pedirá el nombre del experimento y:
1. **Convierte** rosbag → CSV usando `convert_experiment_to_csv.py`
2. **Genera** gráficas de trayectorias y errores usando `plot_trajectories_corrected.py`
3. **Optimiza** el proceso (no reconvierte CSVs existentes)

## 📊 Resultados Generados

### 🎯 Trayectorias (posiciones reales del gripper):
- `experiment_X_robot_01_trajectory.eps`
- `experiment_X_robot_02_trajectory.eps`  
- `experiment_X_robot_03_trajectory.eps`

### 📈 Errores individuales (con líneas de cambio de fase):
- `experiment_X_robot_01_errors.eps`
- `experiment_X_robot_02_errors.eps`
- `experiment_X_robot_03_errors.eps`

### 📊 Errores combinados (subplots a), b), c)):
- `experiment_X_errors_combined.eps`

## ✨ Características

- ✅ **Automatización completa**: Un solo comando
- ✅ **Detección inteligente**: Evita reconversiones innecesarias  
- ✅ **Gráficas profesionales**: Formato EPS vectorial para publicaciones
- ✅ **Tiempo real**: Tiempo 0 = primera tarea asignada
- ✅ **Cambios de fase**: Líneas punteadas rojas para transiciones de estado
- ✅ **Posiciones reales**: Extrae datos del gripper, no trayectorias planificadas

## 🗂️ Estructura

```
workspace/
├── bags/experiment_X/          ← Rosbags de entrada
├── csv/experiment_X/           ← CSVs generados
├── trajectory_plots/           ← Gráficas EPS generadas
└── scripts_deck/bag_to_plots.sh ← Script de automatización
```

## 🔧 Requisitos

- `convert_experiment_to_csv.py`
- `plot_trajectories_corrected.py`  
- Python 3 con pandas, matplotlib, numpy

---
**Autor**: Generated for Robotics_Cyber_Physical_LAB