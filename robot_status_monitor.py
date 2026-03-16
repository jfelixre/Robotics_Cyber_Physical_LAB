#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from interfaces.msg import RobotState
import os
import time
from tabulate import tabulate
from datetime import datetime

class RobotStatusMonitor(Node):
    def __init__(self):
        super().__init__('robot_status_monitor')
        
        # Diccionario para almacenar estado de cada robot
        self.robot_states = {}
        
        # Crear suscripciones para múltiples robots (ajusta según necesites)
        self.max_robots = 3  # Monitoreará robot_01 hasta robot_03
        self.robot_subscriptions = []  # Cambio de nombre para evitar conflicto
        
        for robot_id in range(1, self.max_robots + 1):
            topic_name = f'/robot_0{robot_id}/robot_state'
            subscription = self.create_subscription(
                RobotState,
                topic_name,
                lambda msg, rid=robot_id: self.robot_state_callback(msg, rid),
                10
            )
            self.robot_subscriptions.append(subscription)
            
            # Inicializar estado del robot
            self.robot_states[robot_id] = {
                'state': -1,
                'last_update': None,
                'active': False
            }
        
        # Timer para actualizar pantalla cada 1 segundo
        self.timer = self.create_timer(1.0, self.display_update)
        
        self.get_logger().info(f'Robot Status Monitor started. Monitoring {self.max_robots} robots.')
        
    def robot_state_callback(self, msg, robot_id):
        """Callback para recibir estado de robot específico"""
        self.robot_states[robot_id] = {
            'state': msg.robot_state,
            'last_update': datetime.now(),
            'active': True
        }
        
    def get_state_description(self, state):
        """Obtener descripción del estado"""
        state_descriptions = {
            -1: "UNKNOWN",
            0: "IDLE",
            1: "APPROACH", 
            2: "PRE-GRASP",
            3: "TAKE_OBJECT",
            4: "TRANSPORT",
            5: "PLACE_OBJECT", 
            6: "RELEASE",
            7: "RETURN",
            8: "COMPLETE",
            9: "RESET",
            99: "ERROR"
        }
        return state_descriptions.get(state, f"UNKNOWN({state})")
        
    def get_status_emoji(self, state):
        """Obtener emoji según el estado"""
        if state == -1:
            return "❓"  # Desconocido
        elif state == 0:
            return "⏸️"  # Idle
        elif 1 <= state <= 7:
            return "🔄"  # En progreso
        elif state == 8:
            return "✅"  # Completado
        elif state == 9:
            return "🔄"  # Reset
        elif state == 99:
            return "🚨"  # Error
        else:
            return "❓"
            
    def display_update(self):
        """Actualizar la pantalla con información de robots"""
        # Limpiar pantalla
        os.system('clear')
        
        print("=" * 90)
        print(f"🤖 ROBOT STATUS MONITOR - {time.strftime('%H:%M:%S')}")
        print("=" * 90)
        
        # Preparar datos para tabla
        headers = ["Robot ID", "Status", "State", "Description", "Last Update", "Connection"]
        table_data = []
        
        for robot_id in sorted(self.robot_states.keys()):
            robot_info = self.robot_states[robot_id]
            state = robot_info['state']
            last_update = robot_info['last_update']
            active = robot_info['active']
            
            # Determinar estado de conexión
            if not active or last_update is None:
                connection = "❌ OFFLINE"
                last_update_str = "Never"
            else:
                # Verificar si la última actualización fue hace más de 5 segundos
                time_diff = (datetime.now() - last_update).total_seconds()
                if time_diff > 5:
                    connection = "⚠️ STALE"
                else:
                    connection = "✅ ONLINE"
                last_update_str = last_update.strftime("%H:%M:%S")
            
            row = [
                f"Robot_{robot_id:02d}",
                self.get_status_emoji(state),
                state,
                self.get_state_description(state),
                last_update_str,
                connection
            ]
            table_data.append(row)
            
        print(tabulate(table_data, headers=headers, tablefmt="grid"))
        
        # Estadísticas generales
        print(f"\n📊 Total robots monitored: {len(self.robot_states)}")
        
        # Estadísticas por estado
        state_counts = {}
        connection_counts = {"ONLINE": 0, "OFFLINE": 0, "STALE": 0}
        
        for robot_info in self.robot_states.values():
            state = robot_info['state']
            last_update = robot_info['last_update']
            active = robot_info['active']
            
            # Conteo por estado
            description = self.get_state_description(state)
            state_counts[description] = state_counts.get(description, 0) + 1
            
            # Conteo por conexión
            if not active or last_update is None:
                connection_counts["OFFLINE"] += 1
            else:
                time_diff = (datetime.now() - last_update).total_seconds()
                if time_diff > 5:
                    connection_counts["STALE"] += 1
                else:
                    connection_counts["ONLINE"] += 1
        
        print("\n📈 States:")
        for state, count in sorted(state_counts.items()):
            print(f"   {state}: {count}")
            
        print("\n🔗 Connections:")
        for conn_type, count in connection_counts.items():
            if count > 0:
                print(f"   {conn_type}: {count}")

def main(args=None):
    rclpy.init(args=args)
    monitor = RobotStatusMonitor()
    
    try:
        rclpy.spin(monitor)
    except KeyboardInterrupt:
        print("\n👋 Closing robot status monitor...")
    finally:
        monitor.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()