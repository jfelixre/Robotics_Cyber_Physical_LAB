#!/bin/zsh
source /opt/ros/jazzy/setup.zsh
source "$HOME/Robotics_Cyber_Physical_LAB/install/setup.zsh"
cd "$HOME/Robotics_Cyber_Physical_LAB"

set -e  # Exit on error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  🚀 CONVERSIÓN BAG → CSV → GRÁFICAS  ${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Pedir al usuario el nombre del experimento
echo -e "${YELLOW}📝 Ingresa el nombre del experimento a procesar:${NC}"
echo -e "   ${GREEN}Ejemplos disponibles:${NC}"

# Mostrar experimentos disponibles en bags/
if [ -d "bags" ]; then
    echo -e "   ${BLUE}En bags/:${NC}"
    for exp in bags/*/; do
        if [ -d "$exp" ]; then
            basename="$(basename "$exp")"
            echo -e "     • $basename"
        fi
    done
fi

echo ""
echo -ne "${YELLOW}Experimento: ${NC}"
read experiment_name

# Verificar que el experimento existe
bag_path="bags/$experiment_name"
if [ ! -d "$bag_path" ]; then
    echo -e "${RED}❌ Error: No se encontró el experimento '$experiment_name' en bags/${NC}"
    echo -e "${RED}   Verifica que el nombre sea correcto${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}✅ Experimento encontrado: $experiment_name${NC}"

# Definir rutas
csv_path="csv/$experiment_name"

echo ""

# Paso 1: Convertir BAG a CSV
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}📦 PASO 1: Convertir rosbag → CSV${NC}"
echo -e "${BLUE}============================================${NC}"

# Verificar si ya existen los CSVs
if [ -d "$csv_path" ] && [ "$(ls -A $csv_path)" ]; then
    echo -e "${YELLOW}📂 Los archivos CSV ya existen para '$experiment_name'${NC}"
    echo -e "${YELLOW}¿Quieres reconvertir? (s/N): ${NC}"
    read -t 10 -r reconvert || reconvert="N"  # Timeout de 10 segundos, default N
    if [[ ! "$reconvert" =~ ^[sS]$ ]]; then
        echo -e "${GREEN}✅ Usando archivos CSV existentes${NC}"
    else
        echo -e "${YELLOW}🔄 Reconvirtiendo archivos...${NC}"
        if python3 convert_experiment_to_csv.py "$experiment_name"; then
            echo -e "${GREEN}✅ Conversión BAG → CSV completada exitosamente${NC}"
        else
            echo -e "${RED}❌ Error en la conversión BAG → CSV${NC}"
            exit 1
        fi
    fi
else
    echo -e "${YELLOW}🔄 Ejecutando conversión...${NC}"
    if python3 convert_experiment_to_csv.py "$experiment_name"; then
        echo -e "${GREEN}✅ Conversión BAG → CSV completada exitosamente${NC}"
    else
        echo -e "${RED}❌ Error en la conversión BAG → CSV${NC}"
        echo -e "${RED}   Verifica que el rosbag esté completo y no corrupto${NC}"
        exit 1
    fi
fi

echo ""

# Paso 2: Verificar que existe el CSV
if [ ! -d "$csv_path" ]; then
    echo -e "${RED}❌ Error: No se generó directorio CSV en $csv_path${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Archivos CSV generados en: $csv_path${NC}"

# Debug: mostrar algunos archivos encontrados
echo -e "${YELLOW}📁 Archivos encontrados (primeros 5):${NC}"
ls "$csv_path" | head -5

echo ""

# Contar robots disponibles
echo -e "${YELLOW}🔍 Buscando archivos de robot en: $csv_path${NC}"
robot_count=$(find "$csv_path" -name "_robot_*_control_markers.csv" 2>/dev/null | wc -l)

if [ "$robot_count" -eq 0 ]; then
    echo -e "${YELLOW}⚠️  No se encontraron archivos control_markers${NC}"
    # Intentar contar de otra manera
    robot_count=$(find "$csv_path" -name "_robot_*" 2>/dev/null | head -1 | grep -o "_robot_[0-9]*_" | wc -l)
    if [ "$robot_count" -eq 0 ]; then
        echo -e "${YELLOW}   Usando valor por defecto: 3 robots${NC}"
        robot_count=3  # Default fallback
    fi
fi

# Verificar que robot_count no esté vacío
if [ -z "$robot_count" ] || [ "$robot_count" -eq 0 ]; then
    echo -e "${YELLOW}   Forzando valor por defecto: 3 robots${NC}"
    robot_count=3
fi

echo -e "${GREEN}📊 Robots detectados: $robot_count${NC}"
echo ""

# Paso 3: Generar gráficas
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}📈 PASO 2: Generar gráficas profesionales${NC}"
echo -e "${BLUE}============================================${NC}"

echo -e "${YELLOW}🎨 Generando gráficas de trayectorias y errores...${NC}"

# Debug: mostrar variables antes de ejecutar
echo -e "${YELLOW}🔍 Debug - Variables:${NC}"
echo -e "   Experimento: '$experiment_name'"  
echo -e "   Robot count: '$robot_count'"
echo -e "   CSV path: '$csv_path'"

if python3 plot_trajectories_corrected.py --experiment "$experiment_name" --robots "$robot_count" --errors --metrics; then
    echo -e "${GREEN}✅ Gráficas generadas exitosamente${NC}"
else
    echo -e "${RED}❌ Error generando gráficas${NC}"
    echo -e "${RED}   Verifica que los datos CSV estén completos${NC}"
    exit 1
fi

echo ""

# Paso 4: Mostrar resultados
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}🎯 RESULTADOS GENERADOS${NC}"
echo -e "${BLUE}============================================${NC}"

echo -e "${GREEN}📁 Archivos CSV:${NC} csv/$experiment_name/"
echo -e "${GREEN}📊 Gráficas EPS:${NC} trajectory_plots/"

# Listar archivos EPS generados
if [ -d "trajectory_plots" ]; then
    echo ""
    echo -e "${YELLOW}📈 Archivos EPS generados:${NC}"
    for eps_file in trajectory_plots/${experiment_name}_*.eps; do
        if [ -f "$eps_file" ]; then
            echo -e "   ${GREEN}✅ $(basename "$eps_file")${NC}"
        fi
    done
fi

echo ""
echo -e "${GREEN}🎉 ¡Proceso completado exitosamente!${NC}"
echo -e "${BLUE}========================================${NC}"

# Información adicional
echo ""
echo -e "${YELLOW}💡 Información:${NC}"
echo -e "   • Las gráficas están en formato EPS (vectorial para publicaciones)"
echo -e "   • El tiempo 0 corresponde al momento de la primera tarea asignada"
echo -e "   • Las líneas punteadas rojas marcan cambios de fase de los robots"
echo -e "   • Los datos de error son extraídos de posiciones reales del gripper"
echo ""