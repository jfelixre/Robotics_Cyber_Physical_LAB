#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>

#include <interfaces/msg/new_task_msg.hpp>
#include <interfaces/msg/task_report.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/task_msg.hpp>
#include <interfaces/srv/task_list_service.hpp>
#include <interfaces/msg/positions.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <std_msgs/msg/int32.hpp> // <--- NUEVO: Necesario para leer la confirmación
#include <rclcpp/executors/single_threaded_executor.hpp>


using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;

int robot_id = 0;
bool busy = false;
int robot_state = -1;
int leader_robot_id = 0;

interfaces::msg::TaskMsg task_list;
interfaces::msg::TaskDescription selected_task;


class Task_Manager_Node_Client : public rclcpp::Node
{
    public:
        Task_Manager_Node_Client() : Node("task_manager_node_client")
        {
            selected_task.priority = 15;
            selected_task.task_id = 0;

            publisher_task_update = this->create_publisher<interfaces::msg::TaskReport>("/task_scheduler/update_task",10);
            publisher_new_task = this->create_publisher<interfaces::msg::NewTaskMsg>("/task_scheduler/new_task",10);

            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            // 1. Suscripción al Estado (Solo para monitoreo visual o lógica 'busy')
            // USO TOPIC RELATIVO - El namespace del launch se aplicará automáticamente
            subscription_robot_state = this->create_subscription<interfaces::msg::RobotState>(
                "robot_state", 10, std::bind(&Task_Manager_Node_Client::robot_state_callback, this, _1));

            // 2. NUEVA SUSCRIPCIÓN: Confirmación explícita de tarea terminada
            subscription_task_completed = this->create_subscription<std_msgs::msg::Int32>(
                "task_completed", 10, std::bind(&Task_Manager_Node_Client::task_completed_callback, this, _1));

             timer_ = this->create_wall_timer(
             2000ms, std::bind(&Task_Manager_Node_Client::timer_callback, this)); // Bajé el tiempo a 2s para mayor reacción

            // USO TOPIC RELATIVO - El namespace del launch se aplicará automáticamente
            publisher_task_robot = this->create_publisher<interfaces::msg::TaskDescription>("task_assigned", 10);

            task_request_client = this->create_client<interfaces::srv::TaskListService>("/task_scheduler/assign_task");
        }

    private:

     // Callback de Estado: Solo gestiona la bandera 'busy' localmente
     void robot_state_callback(const interfaces::msg::RobotState::SharedPtr msg)
        {
            int previous_robot_state = robot_state;
            robot_state = msg->robot_state;

            // DEBUG: Log siempre los cambios de estado recibidos
            if (previous_robot_state != robot_state) {
                RCLCPP_INFO(this->get_logger(), "[STATE_DEBUG] Robot %d: %d → %d | Tarea activa: %d | Busy: %s", 
                           robot_id, previous_robot_state, robot_state, 
                           static_cast<int>(selected_task.task_id), busy ? "true" : "false");
            }

            // Si el robot está en IDLE (0), lo marcamos como libre.
            if (robot_state == 0){
                busy = false;
            }

            // NUEVA LÓGICA: Enviar updates al scheduler cuando hay tarea activa
            if (previous_robot_state != robot_state && selected_task.task_id > 0) {
                interfaces::msg::TaskReport msg_update;
                msg_update.robot_id = robot_id;
                msg_update.task_id = selected_task.task_id;
                
                // ENVÍO DIRECTO: usar robot_state directamente
                msg_update.state = robot_state;  
                
                publisher_task_update->publish(msg_update);
                RCLCPP_INFO(this->get_logger(), "[UPDATE_SENT] Robot %d cambió de estado %d → %d | Tarea %d | Estado de tarea: %d", 
                           robot_id, previous_robot_state, robot_state, selected_task.task_id, msg_update.state);
            }
            else if (previous_robot_state != robot_state && selected_task.task_id <= 0) {
                RCLCPP_WARN(this->get_logger(), "[NO_UPDATE] Robot %d cambió de estado %d → %d | Sin tarea activa (task_id=%d)", 
                           robot_id, previous_robot_state, robot_state, static_cast<int>(selected_task.task_id));
            }
        }

    // NUEVO CALLBACK: Recibe la confirmación segura desde EventDrivenControl
    void task_completed_callback(const std_msgs::msg::Int32::SharedPtr msg) {
        int completed_task_id = msg->data;
        
        RCLCPP_INFO(this->get_logger(), "CONFIRMATION RECEIVED: Task ID %d completed by Robot %d", completed_task_id, robot_id);

        // 1. Avisar al Scheduler
        interfaces::msg::TaskReport msg_update;
        msg_update.robot_id = robot_id;
        msg_update.task_id = completed_task_id; // Usamos el ID real que viene del mensaje
        msg_update.state = 2; // Estado: Finished
        publisher_task_update->publish(msg_update);

        // 2. Limpieza Local
        leader_robot_id = 0;
        selected_task.priority = 15;
        selected_task.task_id = 0;
        busy = false; // Aseguramos que se libere
    }

    void request_task_from_scheduler() {
        if (!task_request_client->wait_for_service(1s)) {
            // RCLCPP_WARN(this->get_logger(), "Task scheduler service not available");
            return;
        }

        auto request = std::make_shared<interfaces::srv::TaskListService::Request>();
        request->robot_id = robot_id;

        auto future = task_request_client->async_send_request(request,
            [this](rclcpp::Client<interfaces::srv::TaskListService>::SharedFuture response) {
                try {
                    if (!response.get()->task_list.empty()) {
                        selected_task = response.get()->task_list[0];
                        
                        // Validar que no estemos reasignando la tarea 0 por error
                        if (selected_task.task_id == 0) return;

                        // Avisar al Scheduler que la tomamos (En Progreso)
                        interfaces::msg::TaskReport msg_update;
                        msg_update.robot_id = robot_id;
                        msg_update.task_id = selected_task.task_id;
                        msg_update.state = 1; // Estado: In Progress

                        RCLCPP_INFO(this->get_logger(), ">>> Task ID %d ASSIGNED to Robot %d", selected_task.task_id, robot_id);

                        busy = true;

                        // Lógica de Robots Colaborativos
                        if (selected_task.obj_size == 2) {
                            if (selected_task.leader_robot_id == 0) {
                                interfaces::msg::NewTaskMsg msg_new_task;
                                msg_new_task.priority = selected_task.priority;
                                msg_new_task.obj_id = selected_task.obj_id;
                                msg_new_task.goal = selected_task.goal;
                                msg_new_task.angle_goal = selected_task.angle_goal; // No olvidar el ángulo
                                msg_new_task.leader_robot_id = robot_id;
                                selected_task.leader_robot_id = robot_id;

                                publisher_new_task->publish(msg_new_task);
                            } else {
                                RCLCPP_INFO(this->get_logger(), "Joining as Follower Robot");
                            }
                        }

                        publisher_task_update->publish(msg_update);
                        publisher_task_robot->publish(selected_task); // Enviar al robot controller
                    } 
                } catch (const std::exception &e) {
                    RCLCPP_ERROR(this->get_logger(), "Error en scheduler response: %s", e.what());
                }
            });
    }

     void timer_callback()
    {  
        // Solo pedimos tarea si NO estamos ocupados y el robot está en estado 0 (IDLE)
        if (!busy && robot_state == 0) {
            request_task_from_scheduler();
        }
    }

    rclcpp::Publisher<interfaces::msg::TaskReport>::SharedPtr publisher_task_update;
    rclcpp::Publisher<interfaces::msg::NewTaskMsg>::SharedPtr publisher_new_task;
    rclcpp::Publisher<interfaces::msg::TaskDescription>::SharedPtr publisher_task_robot;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<interfaces::msg::RobotState>::SharedPtr subscription_robot_state;
    
    // NUEVA SUSCRIPCIÓN
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_task_completed;
    
    rclcpp::Client<interfaces::srv::TaskListService>::SharedPtr task_request_client;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto client_node = std::make_shared<Task_Manager_Node_Client>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(client_node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}