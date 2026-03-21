# 🚀 Sistema de Gráficas Profesionales para Trayectorias de Robots

## 📋 Resumen

Ahora tienes un sistema completo para generar gráficas profesionales de las trayectorias de tus robots con datos reales capturados durante los experimentos.

## 🔧 Componentes Nuevos

### 1. **robot_data_logger.py** - Captura datos en tiempo real
- 📍 **Ubicación**: `src/control_pkg/scripts/robot_data_logger.py`
- 🎯 **Función**: Graba posiciones, estados y poses de robots durante experimentos
- 💾 **Salida**: Archivos CSV organizados por robot y tipo de dato

### 2. **plot_trajectories_professional.py** - Genera gráficas
- 📍 **Ubicación**: `plot_trajectories_professional.py` (raíz del proyecto)
- 🎯 **Función**: Crea gráficas profesionales de trayectorias segmentadas por fases
- 🖼️ **Salida**: Imágenes PDf/PNG de alta calidad para publicación

## 🚀 Cómo Usar

### Paso 1: Configurar Logging (Una sola vez)

En el launch file donde quieras capturar datos, des-comenta la línea del data logger:

```python
# En robot_01_sim.launch.py (línea ~280):
# data_logger,  ← Quita el comentario (#)
```

### Paso 2: Ejecutar Experimento con Captura de Datos

1. **Lanzar experimento con logging activo**:
```bash
source install/setup.zsh
ros2 launch launch_pkg gazebo_sim_exp_1.launch.py
```

2. **En otra terminal, activar manualmente el logger** (alternativo):
```bash
ros2 run control_pkg robot_data_logger.py --ros-args -p experiment_name:=mi_experimento -p robots:=2
```

### Paso 3: Ejecutar tu Experimento Normalmente

- Los robots ejecutarán sus tareas (Fases 1-9)
- El logger capturará automáticamente:
  - ✅ Posiciones (x,y,z) del base_link
  - ✅ Posiciones del gripper
  - ✅ Estados de la máquina de estados
  - ✅ Timestamps precisos

### Paso 4: Generar Gráficas Profesionales

Cuando el experimento termine, generar las gráficas:

```bash
# Gráficas para todos los robots
python3 plot_trajectories_professional.py --experiment mi_experimento --robots 2 --show

# Solo Robot 1
python3 plot_trajectories_professional.py --experiment mi_experimento --robot 1 --show

# Sin mostrar en pantalla (solo guardar)
python3 plot_trajectories_professional.py --experiment mi_experimento --robots 2
```

## 📁 Estructura de Archivos Generados

```
csv/
└── mi_experimento/
    ├── robot_01_positions.csv    # Posiciones base_link
    ├── robot_01_gripper.csv      # Posiciones gripper  
    ├── robot_01_states.csv       # Estados FSM
    ├── robot_02_positions.csv
    ├── robot_02_gripper.csv
    └── robot_02_states.csv

trajectory_plots/
├── mi_experimento_robot_01_trajectory.png
├── mi_experimento_robot_02_trajectory.png
└── mi_experimento_comparison_trajectory.png
```

## 🎨 Características de las Gráficas

### ✨ Profesionales para Publicación
- **Alta resolución**: 300 DPI
- **Colores distinguibles**: Cada fase tiene color único
- **Leyendas limpias**: Fases numeradas (Phase 1-9)
- **Texto en inglés**: Listo para publicación internacional
- **Marcadores**: Inicio (verde), fin (rojo), direcciones

### 📊 Segmentación por Fases
- **Phase 1**: Navegación inicial (azul)
- **Phase 2**: Aproximación a objeto (naranja)
- **Phase 3**: Posicionamiento (verde)
- **Phase 4**: Recolección (rojo)
- **Phase 5**: Transporte (púrpura)
- **Phase 6**: Navegación con objeto (marrón)
- **Phase 7**: Posicionamiento destino (rosa)
- **Phase 8**: Entrega (gris)
- **Phase 9**: Retorno a base (verde oliva)

## 🔧 Configuración Avanzada

### Cambiar Rate de Logging
```bash
ros2 run control_pkg robot_data_logger.py --ros-args -p log_rate:=20.0  # 20 Hz
```

### Logging para 3 Robots
```bash
ros2 run control_pkg robot_data_logger.py --ros-args -p robots:=3
```

### Usar Datos Legacy (bags_exp2_test3)
```bash
python3 plot_trajectories_professional.py --experiment bags_exp2_test3 --legacy --robots 2
```

## 🐛 Troubleshooting

### ❌ "No se encontraron datos"
- Verifica que el logger esté corriendo durante el experimento
- Confirma que los robots publican `/robot_0X/robot_state`

### ❌ "Archivo no encontrado"
- Asegúrate de que el experimento haya terminado
- Verifica nombres: `--experiment` debe coincidir con el parámetro usado en el logger

### ❌ "No se encontraron estados válidos"
- Los robots deben pasar por estados 1-9 para generar segmentos
- Revisa que la máquina de estados esté funcionando correctamente

## 📈 Próximos Pasos

1. **Ejecuta un experimento** con logging activado
2. **Genera tus primeras gráficas** con el script
3. **Personaliza colores/estilos** si necesitas
4. **¡Publícalas!** - Son de calidad profesional

---

🎉 **¡Sistema listo para usar!** Tus gráficas de trayectorias ahora serán profesionales y basadas en datos reales.