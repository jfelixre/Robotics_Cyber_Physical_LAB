#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/msg/arm_objective.hpp>
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/waiting_team.hpp>
#include <interfaces/msg/follower_robot.hpp>
#include <interfaces/msg/leader_robot.hpp>
#include <interfaces/srv/robot_status.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/empty.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <std_msgs/msg/int32.hpp> // NECESARIO PARA AVISAR AL MANAGER

// Visualización RVIZ
#include <visualization_msgs/msg/marker.hpp>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>

#include <memory>
#include <string>
#include <sstream>
#include <cmath>
#include <chrono>

using namespace std::chrono_literals;

class EventDrivenControl : public rclcpp::Node {
public:
    EventDrivenControl() : Node("event_driven_control") {
        this->declare_parameter<int>("robot_id", 0);
        robot_id = this->get_parameter("robot_id").as_int();
        RCLCPP_INFO(this->get_logger(), "Iniciando EventDrivenControl (Golden Master) para Robot_ID: %d", robot_id);

        setup_publishers_and_subscribers();
        setup_tf_components();
        
        // Inicializar Estado
        robot_state.robot_state = 0;
        publisher_robot_state->publish(robot_state);
        task.task_id = -1; 
        reset_phase_flags();

        // Timer Lógica (10 Hz)
        logic_timer_ = this->create_wall_timer(100ms, std::bind(&EventDrivenControl::logic_loop, this));
    }

private:
    int robot_id;
    
    // --- BANDERAS DE CONTROL ---
    bool team_ready_ = false;       
    bool phase_step_1_done_ = false; 
    
    // Timers
    rclcpp::Time wait_start_time_;   
    bool timer_active_ = false;
    
    // Timer para filtrar falsos positivos de llegada (Debounce)
    rclcpp::Time state_start_time_;

    // Cerrojo de envío único (Latch)
    bool objective_sent_ = false; 
    
    bool initial_position_saved_ = false;

    // VARIABLE CRÍTICA: Evita repetir tareas
    int last_completed_task_id_ = -1; 

    // Variables de Tarea y Posición
    float Xobj, Yobj, Zobj, Angobj;
    float angle_goal;
    float Z_saved = 0;
    std::string gripper_name;

    interfaces::msg::TaskDescription task;
    interfaces::msg::RobotState robot_state;
    interfaces::msg::RobotObjective objective;
    interfaces::msg::ArmObjective arm_objective;
    interfaces::msg::RobotObjective initial_position;
    interfaces::msg::WaitingTeam waiting_msg;
    
    geometry_msgs::msg::TransformStamped objective_transform_;

    // ROS Components
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr logic_timer_;

    rclcpp::Publisher<interfaces::msg::RobotState>::SharedPtr publisher_robot_state;
    rclcpp::Publisher<interfaces::msg::RobotObjective>::SharedPtr publisher_robot_objective;
    rclcpp::Publisher<interfaces::msg::ArmObjective>::SharedPtr publisher_arm_objective;
    rclcpp::Publisher<interfaces::msg::FollowerRobot>::SharedPtr publisher_follower_robot;
    rclcpp::Publisher<interfaces::msg::WaitingTeam>::SharedPtr publisher_waiting_robot;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_copy_control;
    rclcpp::Publisher<interfaces::msg::ControlFinish>::SharedPtr publisher_control_finish_team_robot;
    rclcpp::Publisher<interfaces::msg::LeaderRobot>::SharedPtr publisher_leader_robot;
    rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr publisher_attach;
    rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr publisher_detach;
    
    // Publisher Visualización
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr publisher_state_marker_;
    
    // Publisher Confirmación Tarea
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_task_finished_;

    rclcpp::Subscription<interfaces::msg::TaskDescription>::SharedPtr subscription_taskrobot_;
    rclcpp::Subscription<interfaces::msg::ControlFinish>::SharedPtr subscription_control_finish_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription_team_ready_;

    // --- SETUP ---
    void setup_publishers_and_subscribers() {
        std::string prefix = "/robot_0" + std::to_string(robot_id);

        subscription_taskrobot_ = this->create_subscription<interfaces::msg::TaskDescription>(
            prefix + "/task_assigned", 10, std::bind(&EventDrivenControl::task_robot_callback, this, std::placeholders::_1));

        subscription_control_finish_ = this->create_subscription<interfaces::msg::ControlFinish>(
            prefix + "/control_finish", 10, std::bind(&EventDrivenControl::control_finish_callback, this, std::placeholders::_1));

        subscription_team_ready_ = this->create_subscription<std_msgs::msg::Bool>(
            "/team_ready", 10, std::bind(&EventDrivenControl::team_ready_callback, this, std::placeholders::_1));

        publisher_robot_state = this->create_publisher<interfaces::msg::RobotState>(prefix + "/robot_state", 1);
        publisher_robot_objective = this->create_publisher<interfaces::msg::RobotObjective>(prefix + "/objective", 1);
        publisher_arm_objective = this->create_publisher<interfaces::msg::ArmObjective>(prefix + "/arm_objective", 1);
        publisher_waiting_robot = this->create_publisher<interfaces::msg::WaitingTeam>(prefix + "/waiting_team", 10);
        publisher_copy_control = this->create_publisher<std_msgs::msg::Bool>(prefix + "/copy_control", 10);
        publisher_leader_robot = this->create_publisher<interfaces::msg::LeaderRobot>("/leader_robot_id", 10);
        
        publisher_state_marker_ = this->create_publisher<visualization_msgs::msg::Marker>(prefix + "/system_status_marker", 10);
        
        // Publisher para confirmar finalización al Manager
        publisher_task_finished_ = this->create_publisher<std_msgs::msg::Int32>(prefix + "/task_completed", 10);

        if (robot_id == 1) { 
             publisher_follower_robot = this->create_publisher<interfaces::msg::FollowerRobot>("/robot_02/follower_robot", 10);
        } else {
             publisher_follower_robot = this->create_publisher<interfaces::msg::FollowerRobot>("/robot_01/follower_robot", 10);
        }

        std::string partner_finish_topic = (robot_id == 1) ? "/robot_02/control_finish" : "/robot_01/control_finish";
        publisher_control_finish_team_robot = this->create_publisher<interfaces::msg::ControlFinish>(partner_finish_topic, 10);

        publisher_attach = this->create_publisher<std_msgs::msg::Empty>("/robot_01/robot_02/attach", 10);
        publisher_detach = this->create_publisher<std_msgs::msg::Empty>("/robot_01/robot_02/detach", 10);
    }

    void setup_tf_components() {
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
        
        objective_transform_.header.frame_id = "marker_id_00";
        objective_transform_.child_frame_id = "objective_" + std::to_string(robot_id);
        
        // --- AGREGA ESTAS LÍNEAS ---
        // Inicializamos una rotación válida (Identidad)
        objective_transform_.transform.rotation.x = 0.0;
        objective_transform_.transform.rotation.y = 0.0;
        objective_transform_.transform.rotation.z = 0.0;
        objective_transform_.transform.rotation.w = 1.0; // <--- ESTO ES LA CLAVE
    }

    // --- CALLBACKS ---
    void task_robot_callback(const interfaces::msg::TaskDescription::SharedPtr msg) {
        // EVITAR REPETICIÓN: Si es la misma tarea que acabamos de terminar, ignorar.
        if (msg->task_id == last_completed_task_id_) {
            return; 
        }

        task = *msg;
        RCLCPP_INFO(this->get_logger(), "--> NUEVA TAREA ACEPTADA: ID %d", task.task_id);
        
        reset_phase_flags();
        robot_state.robot_state = 1; 
        publisher_robot_state->publish(robot_state);
        
        angle_goal = task.angle_goal;
        std::stringstream ss_gripper;
        ss_gripper << "robot_0" << robot_id << "/gr_ref_link";
        gripper_name = ss_gripper.str();
    }

    // --- FILTRO INTELIGENTE DE FINALIZACIÓN ---
    void control_finish_callback(const interfaces::msg::ControlFinish::SharedPtr msg) {
        if (msg->finish_confirm == 1) {
            
            // 1. FILTRO DE TIPO DE FASE:
            // En fases estáticas (brazo), ignoramos señal de base.
            if (robot_state.robot_state == 3 || 
                robot_state.robot_state == 5 || 
                robot_state.robot_state == 6) {
                return; 
            }

            // 2. FILTRO DE TIEMPO (DEBOUNCE):
            // En fases de viaje largo (4 y 7), ignorar "llegada" los primeros 3 segundos.
            if (robot_state.robot_state == 4 || robot_state.robot_state == 7) {
                double seconds_since_start = (this->now() - state_start_time_).seconds();
                if (seconds_since_start < 3.0) {
                    return; 
                }
            }

            RCLCPP_INFO(this->get_logger(), "Fase %d completada (Base Confirmada). Avanzando...", robot_state.robot_state);
            robot_state.robot_state++;
            publisher_robot_state->publish(robot_state);
            reset_phase_flags(); 
        }
    }

    void team_ready_callback(const std_msgs::msg::Bool::SharedPtr msg) {
        team_ready_ = msg->data;
    }

    // --- LÓGICA PRINCIPAL ---
    void logic_loop() {
        publish_state_marker();
        
        // 1. TIMESTAMP (Actualizar hora)
        objective_transform_.header.stamp = this->now();

        // 2. ASEGURAR COORDENADAS (Copiarlas de la variable de lógica)
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        objective_transform_.transform.translation.z = objective.point.z;

        // 3. FIX CRÍTICO: CALCULAR ROTACIÓN VÁLIDA
        // Si enviamos w=0, el brazo ignorará el objetivo.
        tf2::Quaternion q;
        // Protección por si el ángulo no está definido aún
        double angle_safe = std::isfinite(objective.angle) ? objective.angle : 0.0;
        
        q.setRPY(0, 0, angle_safe); // Convertir ángulo Euler a Cuaternión
        
        objective_transform_.transform.rotation.x = q.x();
        objective_transform_.transform.rotation.y = q.y();
        objective_transform_.transform.rotation.z = q.z();
        objective_transform_.transform.rotation.w = q.w(); // Esto será 1.0 (Válido)

        // 4. PUBLICAR EL TF (Ahora sí el brazo lo verá)
        tf_broadcaster_->sendTransform(objective_transform_);

        // 5. ACTUALIZAR SENSORES DE POSICIÓN
        if (robot_state.robot_state > 0 && robot_state.robot_state < 9) {
             update_transforms(); 
        }

        // 6. MÁQUINA DE ESTADOS
        switch (robot_state.robot_state) {
            case 0: phase_zero_initialization(); break;
            case 1: phase_one_approach(); break;
            case 2: phase_two_last_approach(); break;
            case 3: phase_three_take_object(); break;
            case 4: phase_four_transport(); break;
            case 5: phase_five_place_object(); break; 
            case 6: phase_six_release_object(); break;
            case 7: phase_seven_return_to_start(); break;
            case 8: phase_eight_finalize_task(); break;
            case 9: phase_nine_reset_state(); break;
            default: break;
        }
    }

    // --- VISUALIZACIÓN ---
    void publish_state_marker() {
        visualization_msgs::msg::Marker marker;
        std::string ref_frame = (robot_id < 10) ? 
            "robot_0" + std::to_string(robot_id) + "/base_link" : 
            "robot_" + std::to_string(robot_id) + "/base_link";

        marker.header.frame_id = ref_frame; 
        marker.header.stamp = rclcpp::Time(0); 
        marker.ns = "robot_status_info";
        marker.id = 0; 
        marker.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.pose.position.x = 0.0; marker.pose.position.y = 0.0; marker.pose.position.z = 0.8; 
        marker.scale.z = 0.15; 
        marker.color.r = 0.0; marker.color.g = 1.0; marker.color.b = 1.0; marker.color.a = 1.0; 

        std::stringstream ss;
        ss << "R" << robot_id << " | ST:" << robot_state.robot_state << "\n";
        
        std::string state_desc;
        switch(robot_state.robot_state) {
            case 0: state_desc = "IDLE"; break;
            case 1: state_desc = "APPROACH"; break;
            case 2: state_desc = "PRE-GRASP"; break;
            case 3: state_desc = "GRASPING"; break;
            case 4: state_desc = "TRANSPORT"; break;
            case 5: state_desc = "PLACING"; break;
            case 6: state_desc = "RELEASING"; break;
            case 7: state_desc = "RETURNING"; break;
            case 8: state_desc = "FINISH"; break;
            default: state_desc = "UNKNOWN"; break;
        }
        ss << state_desc;
        marker.text = ss.str();
        publisher_state_marker_->publish(marker);
    }

    void reset_phase_flags() {
        phase_step_1_done_ = false;
        timer_active_ = false;
        objective_sent_ = false; 
        
        // Guardar tiempo inicio para debounce
        state_start_time_ = this->now();
    }

    bool update_transforms() {
        try {
            std::string obj_frame = (task.obj_id < 10) ? "marker_id_0" + std::to_string(task.obj_id) : "marker_id_" + std::to_string(task.obj_id);
            auto transform = tf_buffer_->lookupTransform("marker_id_00", obj_frame, tf2::TimePointZero);
            Xobj = transform.transform.translation.x;
            Yobj = transform.transform.translation.y;
            Zobj = transform.transform.translation.z;

            tf2::Quaternion q(transform.transform.rotation.x, transform.transform.rotation.y, transform.transform.rotation.z, transform.transform.rotation.w);
            tf2::Matrix3x3 m(q); double r, p, y; m.getRPY(r, p, y);
            Angobj = y;
            
            std::string cube_frame = "cube_id_" + std::to_string(task.obj_id) + "/cube_link";
            try {
                auto tf_cube = tf_buffer_->lookupTransform("marker_id_00", cube_frame, tf2::TimePointZero);
                Zobj = tf_cube.transform.translation.z;
            } catch (...) {}

            return true;
        } catch (tf2::TransformException &ex) {
            return false;
        }
    }

    // --- FASES ---

    void phase_zero_initialization() {
        // Publicar estado constantemente mientras esperamos.
        // Si no hacemos esto, el TaskManager podría perderse el mensaje inicial
        // y creer que el robot no está listo.
        
        // Publicar cada 1 segundo aprox (para no saturar)
        // Usamos la macro THROTTLE que ejecuta esto solo cada 1000ms
        RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 2000, 
            "Estado IDLE (0). Esperando asignación de tareas...");

        // Reforzar que estamos en estado 0
        robot_state.robot_state = 0;
        publisher_robot_state->publish(robot_state);

    }
    
    void phase_one_approach() {
        if (!initial_position_saved_){
            try {
                auto tf_base = tf_buffer_->lookupTransform("marker_id_00", "robot_0" + std::to_string(robot_id) + "/base_link", tf2::TimePointZero);
                initial_position.point.x = tf_base.transform.translation.x;
                initial_position.point.y = tf_base.transform.translation.y;
                initial_position.point.z = 0;
                initial_position_saved_ = true;
            } catch(...) {}
        }

        if (task.obj_size == 1) {
            objective.point.x = Xobj - (0.5 * cos(Angobj));
            objective.point.y = Yobj - (0.5 * sin(Angobj));
            objective.angle = Angobj;
        } 
        else if (task.obj_size == 2) {
             if (task.robot_id == task.leader_robot_id) {
                objective.point.x = Xobj + (0.8 * cos(Angobj));
                objective.point.y = Yobj + (0.8 * sin(Angobj));
                objective.angle = (Angobj + M_PI) - static_cast<int>((Angobj + M_PI) / (2*M_PI)) * 2*M_PI;
            } else {
                if(!phase_step_1_done_) {
                    std::string topic_follower = "/robot_0" + std::to_string(task.leader_robot_id) + "/follower_robot";
                    interfaces::msg::FollowerRobot follower_msg;
                    follower_msg.follower_robot_id = robot_id;
                    publisher_follower_robot->publish(follower_msg);
                    phase_step_1_done_ = true;
                }
                objective.point.x = Xobj - (0.8 * cos(Angobj));
                objective.point.y = Yobj - (0.8 * sin(Angobj));
                objective.angle = Angobj;
            }
        }
        objective.point.z = Zobj;
        objective.obj_id = task.obj_id;
        objective.robot_state = robot_state.robot_state;
        
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        objective_transform_.transform.translation.z = objective.point.z;

        if (!objective_sent_) {
            publisher_robot_objective->publish(objective);

            arm_objective.home_pos = false;
            arm_objective.gripper = false;
            arm_objective.transport_pos = false;
            arm_objective.take_pos = (task.obj_size == 2); 
            arm_objective.obj_id = task.obj_id;
            publisher_arm_objective->publish(arm_objective);
            
            RCLCPP_INFO(this->get_logger(), "Fase 1: Moviendo a Pre-Approach...");
            objective_sent_ = true;
        }
    }

    void phase_two_last_approach() {
        if (task.obj_size == 1) {
            objective.point.x = Xobj;
            objective.point.y = Yobj; 
            objective.angle = Angobj;
        } 
        else if (task.obj_size == 2) {
             if (task.robot_id == task.leader_robot_id) {
                objective.point.x = Xobj + (0.19 * cos(Angobj));
                objective.point.y = Yobj + (0.19 * sin(Angobj));
                objective.angle = (Angobj + M_PI) - static_cast<int>((Angobj + M_PI) / (2*M_PI)) * 2*M_PI;
            } else {
                objective.point.x = Xobj - (0.27 * cos(Angobj));
                objective.point.y = Yobj - (0.27 * sin(Angobj));
                objective.angle = Angobj;
            }
        }
        
        objective.point.z = Zobj;
        Z_saved = Zobj; 
        objective.obj_id = task.obj_id;
        objective.robot_state = robot_state.robot_state;

        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        objective_transform_.transform.translation.z = objective.point.z;

        if (!objective_sent_) {
            publisher_robot_objective->publish(objective);
            arm_objective.take_pos = true;
            publisher_arm_objective->publish(arm_objective);
            RCLCPP_INFO(this->get_logger(), "Fase 2: Aproximación Final...");
            objective_sent_ = true;
        }
    }

    void phase_three_take_object() {
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        objective_transform_.transform.translation.z = objective.point.z;

        if (task.obj_size == 1) {
            if (!timer_active_) {
                arm_objective.gripper = true; 
                arm_objective.transport_pos = false;
                publisher_arm_objective->publish(arm_objective); 
                
                RCLCPP_INFO(this->get_logger(), "Fase 3: Cerrando Gripper (15s)...");
                wait_start_time_ = this->now();
                timer_active_ = true; 
            } else {
                if ((this->now() - wait_start_time_).seconds() >= 15.0) {
                     RCLCPP_INFO(this->get_logger(), "Fase 3: Tiempo completado.");
                     robot_state.robot_state++;
                     publisher_robot_state->publish(robot_state);
                     reset_phase_flags();
                }
            }
        }
        else if (task.obj_size == 2) {
             if (!team_ready_) {
                waiting_msg.waiting_team = true;
                publisher_waiting_robot->publish(waiting_msg);
                return;
            }
             if (!timer_active_) {
                arm_objective.gripper = true;
                arm_objective.take_pos = true;
                publisher_arm_objective->publish(arm_objective); 
                wait_start_time_ = this->now();
                timer_active_ = true;
            } else {
                if ((this->now() - wait_start_time_).seconds() >= 15.0) {
                     robot_state.robot_state++;
                     publisher_robot_state->publish(robot_state);
                     reset_phase_flags();
                }
            }
        }
    }

    void phase_four_transport() {
        interfaces::msg::LeaderRobot msg_leader;
        msg_leader.leader_robot_id = task.leader_robot_id;
        publisher_leader_robot->publish(msg_leader);

        if (task.obj_size == 1) {
            objective.point.x = task.goal.x - (0.5 * cos(angle_goal));
            objective.point.y = task.goal.y - (0.5 * sin(angle_goal));
            objective.angle = angle_goal;
        } 
        else if (task.obj_size == 2) {
             if (task.robot_id == task.leader_robot_id) {
                if(!phase_step_1_done_) {
                    publisher_attach->publish(std_msgs::msg::Empty());
                    phase_step_1_done_ = true;
                }
                objective.point.x = task.goal.x + (0.5 * cos(-M_PI));
                objective.point.y = task.goal.y + (0.5 * sin(-M_PI));
                objective.angle = 0;
            } else {
                std_msgs::msg::Bool copy_msg; copy_msg.data = true;
                publisher_copy_control->publish(copy_msg);
                objective.angle = M_PI;
            }
        }

        objective.obj_id = task.obj_id;
        objective.robot_state = robot_state.robot_state;
        
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;

        if (!objective_sent_) {
            arm_objective.take_pos = false;
            arm_objective.transport_pos = true;
            publisher_arm_objective->publish(arm_objective);
            publisher_robot_objective->publish(objective);
            
            RCLCPP_INFO(this->get_logger(), "Fase 4: Transporte iniciado...");
            objective_sent_ = true;
        }
    }

    void phase_five_place_object() {
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        objective_transform_.transform.translation.z = Z_saved;

        // FASE ESTÁTICA: Usamos Timer y Avance Manual
        if (!timer_active_) {
            if (task.obj_size == 1) {
                objective.point.x = task.goal.x;
                objective.point.y = task.goal.y;
                objective.angle = angle_goal;
                
                if (!objective_sent_) {
                     publisher_robot_objective->publish(objective);
                     objective_sent_ = true;
                }
                arm_objective.gripper = true;
                arm_objective.transport_pos = true; 
                publisher_arm_objective->publish(arm_objective);
            }
            else if (task.obj_size == 2) {
                if (!objective_sent_) {
                    if (task.robot_id == task.leader_robot_id) {
                        publisher_detach->publish(std_msgs::msg::Empty());
                        interfaces::msg::ControlFinish finish_msg;
                        finish_msg.finish_confirm = true;
                        publisher_control_finish_team_robot->publish(finish_msg);
                    }
                    objective_sent_ = true;
                }
                arm_objective.take_pos = true;
                arm_objective.send_finish = true;
                arm_objective.transport_pos = false;
                publisher_arm_objective->publish(arm_objective);
            }

            RCLCPP_INFO(this->get_logger(), "Fase 5: Colocando (5s)...");
            wait_start_time_ = this->now();
            timer_active_ = true; 
        } 
        else {
            if ((this->now() - wait_start_time_).seconds() >= 5.0) {
                RCLCPP_INFO(this->get_logger(), "Fase 5: Completada.");
                robot_state.robot_state++;
                publisher_robot_state->publish(robot_state);
                reset_phase_flags();
            }
        }
    }

    void phase_six_release_object() {
        if (task.obj_size == 2) {
             std_msgs::msg::Bool copy_msg; copy_msg.data = false;
            publisher_copy_control->publish(copy_msg);
            arm_objective.take_pos = true;
            arm_objective.send_finish = true;
            arm_objective.gripper = false;
            publisher_arm_objective->publish(arm_objective);
            return;
        }

        objective_transform_.transform.translation.z = Z_saved;

        if (!timer_active_) {
            arm_objective.gripper = false; // ABRIR
            arm_objective.transport_pos = false;
            arm_objective.take_pos = true;
            publisher_arm_objective->publish(arm_objective);

            RCLCPP_INFO(this->get_logger(), "Fase 6: Soltando (5s)...");
            wait_start_time_ = this->now();
            timer_active_ = true;
        } else {
            if ((this->now() - wait_start_time_).seconds() >= 5.0) {
                RCLCPP_INFO(this->get_logger(), "Fase 6: Completada.");
                arm_objective.send_finish = true; 
                publisher_arm_objective->publish(arm_objective);
                
                robot_state.robot_state++;
                publisher_robot_state->publish(robot_state);
                reset_phase_flags();
            }
        }
    }

    void phase_seven_return_to_start() {
        if (task.obj_size == 1) {
            if(!timer_active_) {
                wait_start_time_ = this->now();
                timer_active_ = true;
            } else {
                if ((this->now() - wait_start_time_).seconds() >= 3.0) {
                    if (!objective_sent_) {
                        arm_objective.take_pos = true;
                        publisher_arm_objective->publish(arm_objective);

                        objective.point.x = task.goal.x - (0.5 * cos(angle_goal));
                        objective.point.y = task.goal.y - (0.5 * sin(angle_goal));
                        objective.angle = angle_goal;
                        objective.robot_state = robot_state.robot_state;
                        publisher_robot_objective->publish(objective);
                        
                        RCLCPP_INFO(this->get_logger(), "Fase 7: Retrocediendo...");
                        objective_sent_ = true;
                    }
                }
            }
        }
        else if (task.obj_size == 2) {
              if (!objective_sent_) {
                arm_objective.take_pos = true;
                arm_objective.gripper = false;
                publisher_arm_objective->publish(arm_objective);
                if (task.robot_id == task.leader_robot_id) {
                    objective.point.x = task.goal.x + (1.0 * cos(-M_PI));
                    objective.point.y = task.goal.y + (1.0 * sin(-M_PI));
                    objective.angle = 0;
                } else {
                    objective.point.x = task.goal.x - (1.0 * cos(M_PI));
                    objective.point.y = task.goal.y - (1.0 * sin(M_PI));
                    objective.angle = M_PI;
                }
                objective.robot_state = robot_state.robot_state;
                publisher_robot_objective->publish(objective);
                objective_sent_ = true;
             }
        }
    }

    void phase_eight_finalize_task() {
        if (!objective_sent_) {
            arm_objective.home_pos = true;
            arm_objective.gripper = false;
            arm_objective.take_pos = false;
            arm_objective.transport_pos = false;
            arm_objective.send_finish = true; 
            publisher_arm_objective->publish(arm_objective);

            interfaces::msg::RobotObjective end_objective = initial_position;
            end_objective.robot_state = robot_state.robot_state;
            publisher_robot_objective->publish(end_objective);
            
            RCLCPP_INFO(this->get_logger(), "Fase 8: Volviendo a Home...");
            objective_sent_ = true;
        }
    }

    void phase_nine_reset_state() {
        RCLCPP_INFO_ONCE(this->get_logger(), "TAREA FINALIZADA. Avisando a Manager y esperando.");
        
        // 1. CONFIRMACIÓN A TASK MANAGER
        if (task.task_id != -1) {
             std_msgs::msg::Int32 finished_msg;
             finished_msg.data = task.task_id;
             publisher_task_finished_->publish(finished_msg);
             
             // 2. GUARDAR MEMORIA PARA NO REPETIR
             last_completed_task_id_ = task.task_id;
        }
        
        task.task_id = -1; 
        reset_phase_flags();
        robot_state.robot_state = 0;
        publisher_robot_state->publish(robot_state);
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<EventDrivenControl>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}