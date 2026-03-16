#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from interfaces.msg import TaskMsg
import os
import time
from tabulate import tabulate

class TaskQueueMonitor(Node):
    def __init__(self):
        super().__init__('task_queue_monitor')
        self.subscription = self.create_subscription(
            TaskMsg,
            '/task_scheduler/task_queue',
            self.task_queue_callback,
            10)
        self.get_logger().info('Task Queue Monitor iniciado. Monitoreando /task_scheduler/task_queue')
        
    def task_queue_callback(self, msg):
        # Limpiar pantalla
        os.system('clear')
        
        print("=" * 80)
        print(f"🤖 TASK QUEUE MONITOR - {time.strftime('%H:%M:%S')}")
        print("=" * 80)
        
        if not msg.task_queue:
            print("📭 Cola de tareas VACÍA")
            return
            
        # Preparar datos para tabla
        headers = ["ID", "Priority", "State", "Description", "Robot", "Leader", "Obj.ID", "Goal X", "Goal Y", "Angle"]
        table_data = []
        
        for task in msg.task_queue:
            # Determinar descripción basada en el estado
            if task.state == 0:
                description = "PENDING"
            elif 1 <= task.state <= 7:
                description = "IN_PROGRESS"
            elif task.state == 8 or task.state == 9:
                description = "COMPLETED"
            elif task.state == 99:
                description = "FAILED"
            else:
                description = f"UNKNOWN({task.state})"
            
            row = [
                task.task_id,
                task.priority,
                task.state,  # Mostrar el número del state
                description,  # Nueva columna con descripción
                task.robot_id if task.robot_id > 0 else "-",
                task.leader_robot_id,
                task.obj_id,
                f"{task.goal.x:.2f}",
                f"{task.goal.y:.2f}", 
                f"{task.angle_goal:.2f}"
            ]
            table_data.append(row)
            
        print(tabulate(table_data, headers=headers, tablefmt="grid"))
        print(f"\n📊 Total tareas: {len(msg.task_queue)}")
        
        # Estadísticas por estado
        stats = {}
        for task in msg.task_queue:
            # Usar la misma lógica de descripción
            if task.state == 0:
                state = "PENDING"
            elif 1 <= task.state <= 7:
                state = "IN_PROGRESS"
            elif task.state == 8 or task.state == 9:
                state = "COMPLETED"
            elif task.state == 99:
                state = "FAILED"
            else:
                state = f"UNKNOWN({task.state})"
            stats[state] = stats.get(state, 0) + 1
            
        print("\n📈 Estados:")
        for state, count in stats.items():
            print(f"   {state}: {count}")
            
def main(args=None):
    rclpy.init(args=args)
    monitor = TaskQueueMonitor()
    
    try:
        rclpy.spin(monitor)
    except KeyboardInterrupt:
        print("\n👋 Cerrando monitor de task queue...")
    finally:
        monitor.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()