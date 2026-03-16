#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/task_msg.hpp>
#include <interfaces/msg/task_report.hpp>
#include <interfaces/msg/new_task_msg.hpp>
#include <interfaces/srv/robot_status.hpp>

#include <chrono>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std::chrono_literals;


class HeartbeatChecker : public rclcpp::Node {
public:
    HeartbeatChecker() : Node("heartbeat_checker") {
        // Suscribirse a la lista de tareas
        task_list_sub_ = this->create_subscription<interfaces::msg::TaskMsg>(
            "/task_scheduler/task_queue", 10,
            std::bind(&HeartbeatChecker::task_list_callback, this, std::placeholders::_1));

        // Publisher para actualizar tareas
        update_task_pub_ = this->create_publisher<interfaces::msg::TaskReport>(
            "/task_scheduler/update_task", 10);
        // Publisher para crear nuevas tareas
        new_task_pub_ = this->create_publisher<interfaces::msg::NewTaskMsg>(
            "/task_scheduler/new_task", 10);

        // Timer para chequear heartbeats periódicamente
        timer_ = this->create_wall_timer(3s, std::bind(&HeartbeatChecker::check_heartbeats, this));
    }

private:
    struct TaskInfo {
        interfaces::msg::TaskDescription task;
        rclcpp::Time last_check;
    };

    std::vector<TaskInfo> active_tasks_;
    std::unordered_map<int, rclcpp::Client<interfaces::srv::RobotStatus>::SharedPtr> robot_clients_;
    // Nuevo: contador de fallos por robot
    std::unordered_map<int, int> robot_fail_count_;

    rclcpp::Subscription<interfaces::msg::TaskMsg>::SharedPtr task_list_sub_;
    rclcpp::Publisher<interfaces::msg::TaskReport>::SharedPtr update_task_pub_;
    rclcpp::Publisher<interfaces::msg::NewTaskMsg>::SharedPtr new_task_pub_;
    rclcpp::TimerBase::SharedPtr timer_;

    void task_list_callback(const interfaces::msg::TaskMsg::SharedPtr msg) {
        active_tasks_.clear();
        for (const auto& task : msg->task_queue) {
            // Solo tareas asignadas (in progress)
            if (task.state == 1 && task.robot_id > 0) {
                TaskInfo info{task, this->now()};
                active_tasks_.push_back(info);
                // Crear cliente si no existe
                if (robot_clients_.find(task.robot_id) == robot_clients_.end()) {
                    std::stringstream ss;
                    ss << "/robot_0" << task.robot_id << "/robot_status";
                    robot_clients_[task.robot_id] = this->create_client<interfaces::srv::RobotStatus>(ss.str());
                }
                // Inicializar contador si no existe
                if (robot_fail_count_.find(task.robot_id) == robot_fail_count_.end()) {
                    robot_fail_count_[task.robot_id] = 0;
                }
            }
        }
    }

    void check_heartbeats() {
        for (auto& info : active_tasks_) {
            int robot_id = info.task.robot_id;
            auto client = robot_clients_[robot_id];
            bool failed = false;

            if (!client->wait_for_service(500ms)) {
                RCLCPP_WARN(this->get_logger(), "Robot %d no responde al servicio robot_status", robot_id);
                failed = true;
            } else {
                auto request = std::make_shared<interfaces::srv::RobotStatus::Request>();
                auto future = client->async_send_request(request);
                
                // Usar wait_for con timeout en lugar de spin_until_future_complete
                // para evitar conflicto de executor
                auto status = future.wait_for(500ms);
                if (status != std::future_status::ready) {
                    RCLCPP_WARN(this->get_logger(), "Robot %d no responde al heartbeat", robot_id);
                    failed = true;
                } else {
                    auto response = future.get();
                    // Si el robot está en estado de error, también se puede manejar aquí
                    if (response->robot_state == 99) { // Asumiendo 99 es estado de error
                        RCLCPP_WARN(this->get_logger(), "Robot %d reporta estado de error", robot_id);
                        failed = true;
                    } else {
                        // Si responde bien, reiniciar contador
                        robot_fail_count_[robot_id] = 0;
                    }
                }
            }

            if (failed) {
                robot_fail_count_[robot_id]++;
                RCLCPP_WARN(this->get_logger(), "Robot %d fallo %d/200 veces", robot_id, robot_fail_count_[robot_id]);
                if (robot_fail_count_[robot_id] >= 200) {
                    handle_robot_failure(info.task);
                    robot_fail_count_[robot_id] = 0; // Reiniciar contador tras marcar fallo
                }
            }
        }
    }

    void handle_robot_failure(const interfaces::msg::TaskDescription& task) {
        // 1. Actualizar tarea a estado de error
        interfaces::msg::TaskReport report;
        report.task_id = task.task_id;
        report.state = 99; // Estado 99 = error de robot
        report.robot_id = task.robot_id;
        update_task_pub_->publish(report);
        RCLCPP_INFO(this->get_logger(), "Tarea %d marcada como error (99)", task.task_id);
        
    }
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<HeartbeatChecker>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
