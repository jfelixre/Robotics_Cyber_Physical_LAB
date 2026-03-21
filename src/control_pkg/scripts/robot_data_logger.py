#!/usr/bin/env python3
"""
Robot Data Logger Node
Graba datos de trayectorias, estados y poses de robots en tiempo real a archivos CSV
para generar gráficas profesionales.

Este nodo se suscribe a múltiples topics y genera archivos CSV organizados
por robot y tipo de dato.

Usage:
    ros2 run control_pkg robot_data_logger --ros-args -p experiment_name:=exp1 -p robots:=2

Topics suscritos por robot X:
    - /tf (poses de base_link y gripper)
    - /robot_0X/robot_state (estados FSM)
    
Datos generados:
    - csv/{experiment_name}/robot_0X_positions.csv
    - csv/{experiment_name}/robot_0X_states.csv
    - csv/{experiment_name}/robot_0X_gripper.csv
"""

import rclpy
from rclpy.node import Node
from interfaces.msg import RobotState
import tf2_ros
import tf2_py
import numpy as np
import pandas as pd
from pathlib import Path
import time
from datetime import datetime
import csv
import threading
from tf2_ros import TransformException
from geometry_msgs.msg import TransformStamped

class RobotDataLogger(Node):
    def __init__(self):
        super().__init__('robot_data_logger')
        
        # Parámetros
        self.declare_parameter('experiment_name', 'default_exp')
        self.declare_parameter('robots', 1)  # Número de robots a monitorear desde este logger
        self.declare_parameter('robot_offset', 1)  # Offset para empezar (robot_01 = offset 1)
        self.declare_parameter('output_dir', 'csv')
        self.declare_parameter('log_rate', 10.0)  # Hz
        self.declare_parameter('use_timestamp', True)  # Agregar timestamp automático
        self.declare_parameter('shared_timestamp', '')  # Timestamp compartido (opcional)
        
        experiment_base = self.get_parameter('experiment_name').value
        self.num_robots = self.get_parameter('robots').value  
        self.robot_offset = self.get_parameter('robot_offset').value
        self.output_dir = Path(self.get_parameter('output_dir').value)
        self.log_rate = self.get_parameter('log_rate').value
        use_timestamp = self.get_parameter('use_timestamp').value
        shared_timestamp = self.get_parameter('shared_timestamp').value
        
        # Crear nombre de experimento con timestamp
        if use_timestamp:
            if shared_timestamp and shared_timestamp.strip():
                # Usar timestamp compartido si está disponible
                self.experiment_name = f"{experiment_base}_{shared_timestamp}"
                self.get_logger().info(f'Usando timestamp compartido: {shared_timestamp}')
            else:
                # Generar timestamp propio
                timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
                self.experiment_name = f"{experiment_base}_{timestamp}"
                self.get_logger().info(f'Generando timestamp propio: {timestamp}')
        else:
            self.experiment_name = experiment_base
        
        # Crear directorio de salida
        self.experiment_dir = self.output_dir / self.experiment_name
        self.experiment_dir.mkdir(parents=True, exist_ok=True)
        
        self.get_logger().info(f'Iniciando Robot Data Logger para experimento: {self.experiment_name}')
        self.get_logger().info(f'Monitoreando {self.num_robots} robot(s) desde robot_{self.robot_offset:02d} a {self.log_rate} Hz')
        
        # TF Buffer y Listener
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)
        
        # Estados de robots
        self.robot_states = {}
        
        # Archivos CSV y writers
        self.csv_files = {}
        self.csv_writers = {}
        self._init_csv_files()
        
        # Suscripciones a estados de robots - usar offset
        self.state_subscriptions = []
        for i in range(self.num_robots):
            robot_id = self.robot_offset + i
            topic = f'/robot_{robot_id:02d}/robot_state'
            sub = self.create_subscription(
                RobotState, topic, 
                lambda msg, rid=robot_id: self.robot_state_callback(msg, rid), 
                10
            )
            self.state_subscriptions.append(sub)
            self.robot_states[robot_id] = {'state': -1, 'timestamp': 0}
        
        # Timer para logging de posiciones
        self.timer = self.create_timer(1.0/self.log_rate, self.log_positions_callback)
        
        # Lock para acceso thread-safe a los archivos
        self.file_lock = threading.Lock()
        
        self.get_logger().info('Robot Data Logger listo!')

    def _init_csv_files(self):
        """Inicializar archivos CSV y writers"""
        for i in range(self.num_robots):
            robot_id = self.robot_offset + i
            robot_files = {}
            robot_writers = {}
            
            # Archivo de posiciones (base_link)
            pos_file = self.experiment_dir / f'robot_{robot_id:02d}_positions.csv'  
            pos_f = open(pos_file, 'w', newline='')
            pos_writer = csv.writer(pos_f)
            pos_writer.writerow(['timestamp', 'x', 'y', 'z', 'roll', 'pitch', 'yaw'])
            robot_files['positions'] = pos_f
            robot_writers['positions'] = pos_writer
            
            # Archivo de gripper (gr_ref_link)
            grip_file = self.experiment_dir / f'robot_{robot_id:02d}_gripper.csv'
            grip_f = open(grip_file, 'w', newline='')
            grip_writer = csv.writer(grip_f)
            grip_writer.writerow(['timestamp', 'x', 'y', 'z', 'roll', 'pitch', 'yaw'])
            robot_files['gripper'] = grip_f
            robot_writers['gripper'] = grip_writer
            
            # Archivo de estados
            state_file = self.experiment_dir / f'robot_{robot_id:02d}_states.csv'
            state_f = open(state_file, 'w', newline='') 
            state_writer = csv.writer(state_f)
            state_writer.writerow(['timestamp', 'state'])
            robot_files['states'] = state_f
            robot_writers['states'] = state_writer
            
            self.csv_files[robot_id] = robot_files
            self.csv_writers[robot_id] = robot_writers

    def robot_state_callback(self, msg, robot_id):
        """Callback para estados de robots"""
        timestamp_ns = self.get_clock().now().nanoseconds
        
        # Verificar cambio de estado
        if self.robot_states[robot_id]['state'] != msg.robot_state:
            with self.file_lock:
                self.csv_writers[robot_id]['states'].writerow([timestamp_ns, msg.robot_state])
                self.csv_files[robot_id]['states'].flush()
            
            self.robot_states[robot_id] = {
                'state': msg.robot_state, 
                'timestamp': timestamp_ns
            }
            
            self.get_logger().info(f'Robot {robot_id:02d}: Estado -> {msg.robot_state}')

    def log_positions_callback(self):
        """Callback del timer para logging de posiciones"""
        timestamp_ns = self.get_clock().now().nanoseconds
        
        for i in range(self.num_robots):
            robot_id = self.robot_offset + i
            # Log base_link position
            self._log_transform(robot_id, 'positions', f'robot_{robot_id:02d}/base_link', timestamp_ns)
            
            # Log gripper position  
            self._log_transform(robot_id, 'gripper', f'robot_{robot_id:02d}/gr_ref_link', timestamp_ns)

    def _log_transform(self, robot_id, data_type, frame_id, timestamp_ns):
        """Obtener y loggear una transformación específica"""
        try:
            # Obtener transform desde world (origen global de Gazebo)
            transform = self.tf_buffer.lookup_transform(
                'world', frame_id, 
                rclpy.time.Time(), timeout=rclpy.duration.Duration(seconds=0.1)
            )
            
            # Extraer posición
            x = transform.transform.translation.x
            y = transform.transform.translation.y  
            z = transform.transform.translation.z
            
            # Extraer orientación (convertir quaternion a Euler)
            qx = transform.transform.rotation.x
            qy = transform.transform.rotation.y
            qz = transform.transform.rotation.z
            qw = transform.transform.rotation.w
            
            # Conversión a ángulos de Euler
            roll, pitch, yaw = self._quaternion_to_euler(qx, qy, qz, qw)
            
            # Escribir a CSV
            with self.file_lock:
                self.csv_writers[robot_id][data_type].writerow([
                    timestamp_ns, x, y, z, roll, pitch, yaw
                ])
                self.csv_files[robot_id][data_type].flush()
                
            # Log de debug (solo ocasionalmente)
            if timestamp_ns % (5 * 1e9) < 1e8:  # Cada 5 segundos aprox
                self.get_logger().info(f'✅ Datos guardados: Robot {robot_id:02d} {data_type} -> ({x:.3f}, {y:.3f})')
                
        except (TransformException, Exception) as e:
            # Log de error solo ocasionalmente para no spam
            if timestamp_ns % (2 * 1e9) < 1e8:  # Cada 2 segundos aprox
                self.get_logger().warn(f'⚠️ No se pudo obtener TF para {frame_id}: {str(e)}')

    def _quaternion_to_euler(self, x, y, z, w):
        """Convertir quaternion a ángulos de Euler (roll, pitch, yaw)"""
        # Roll (x-axis rotation)
        sinr_cosp = 2 * (w * x + y * z)
        cosr_cosp = 1 - 2 * (x * x + y * y)
        roll = np.arctan2(sinr_cosp, cosr_cosp)

        # Pitch (y-axis rotation)
        sinp = 2 * (w * y - z * x)
        if abs(sinp) >= 1:
            pitch = np.copysign(np.pi / 2, sinp)  # use 90 degrees if out of range
        else:
            pitch = np.arcsin(sinp)

        # Yaw (z-axis rotation)
        siny_cosp = 2 * (w * z + x * y)
        cosy_cosp = 1 - 2 * (y * y + z * z)
        yaw = np.arctan2(siny_cosp, cosy_cosp)

        return roll, pitch, yaw

    def __del__(self):
        """Cerrar archivos al destruir el nodo"""
        self.close_files()
    
    def close_files(self):
        """Cerrar explícitamente todos los archivos CSV"""
        self.get_logger().info('📁 Cerrando archivos CSV...')
        for robot_id in self.csv_files:
            for file_type in self.csv_files[robot_id]:
                try:
                    self.csv_files[robot_id][file_type].close()
                    self.get_logger().info(f'✅ Cerrado: robot_{robot_id:02d}_{file_type}.csv')
                except Exception as e:
                    self.get_logger().error(f'❌ Error cerrando archivo: {e}')

def main(args=None):
    rclpy.init(args=args)
    
    logger = RobotDataLogger()
    
    try:
        rclpy.spin(logger)
    except KeyboardInterrupt:
        logger.get_logger().info('🛑 Interrumpido por usuario - Cerrando archivos...')
    finally:
        logger.close_files()  # Cerrar archivos explícitamente
        logger.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()