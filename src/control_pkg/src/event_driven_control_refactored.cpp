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

    // Función auxiliar para calcular la distancia real al objetivo (Desde el GRIPPER)
    double get_distance_to_objective() {
        try {
            // --- CORRECCIÓN: USAR FRAME DEL GRIPPER, NO DE LA BASE ---
            // El objetivo es para la herramienta, así que medimos desde la herramienta.
            std::string gripper_frame = "robot_0" + std::to_string(robot_id) + "/gr_ref_link";
            
            // Preguntar a TF dónde está el GRIPPER realmente respecto al Origen (Marker 00)
            auto tf = tf_buffer_->lookupTransform("marker_id_00", gripper_frame, tf2::TimePointZero);
            double gx = tf.transform.translation.x; // Gripper X
            double gy = tf.transform.translation.y; // Gripper Y

            // Comparar con el objetivo actual
            double tx = objective.point.x;
            double ty = objective.point.y;

            // Distancia Euclidiana
            return std::hypot(tx - gx, ty - gy);

        } catch (tf2::TransformException &ex) {
            RCLCPP_ERROR(this->get_logger(), "Error TF en verificacion: %s", ex.what());
            return 999.9; // Ante la duda, asumimos que estamos lejos
        }
    }

    // --- CALLBACKS ---
    void task_robot_callback(const interfaces::msg::TaskDescription::SharedPtr msg) {
        // 1. Si es una tarea vieja que ya terminamos, la ignoramos.
        if (static_cast<int>(msg->task_id) == last_completed_task_id_) {
            return; 
        }

        // 2. --- EL FIX QUE TE FALTA ---
        // Si me mandan la MISMA tarea que ya estoy haciendo (task_id igual)
        // Y NO estoy en reposo (State 0) ni reseteando (State 9)...
        // ¡IGNORAR EL MENSAJE PARA NO REINICIAR EL TIMER!
        if (msg->task_id == task.task_id && robot_state.robot_state != 0 && robot_state.robot_state != 9) {
            return; 
        }

        if (static_cast<int>(task.task_id) != -1 && robot_state.robot_state != 0 && robot_state.robot_state != 9) {
             return;
        }

        // Si pasa los filtros, aceptamos la nueva tarea
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

    // --- FILTRO INTELIGENTE + VERIFICACIÓN DE DISTANCIA ---
    void control_finish_callback(const interfaces::msg::ControlFinish::SharedPtr msg) {
        if (msg->finish_confirm == true) {
            
            // 1. LISTA NEGRA (Fases de Brazo)
            if (robot_state.robot_state == 0 ||
                robot_state.robot_state == 3 ||
                robot_state.robot_state == 6 ||
                robot_state.robot_state == 9) { 
                return; 
            }

            double seconds_since_start = (this->now() - state_start_time_).seconds();

            // 2. FILTRO DE TIEMPO (Regla de los 3 segundos)
            if (robot_state.robot_state == 4 || 
                robot_state.robot_state == 5 || 
                robot_state.robot_state == 7 || 
                robot_state.robot_state == 8) {
                
                if (seconds_since_start < 3.0) {
                    return; 
                }
            }

            // 3. --- NUEVO: FILTRO DE DISTANCIA REAL ---
            // "Confiar pero Verificar". El controlador dice que llegó, pero... ¿es verdad?
            if (robot_state.robot_state == 1 ||
                robot_state.robot_state == 2 ||
                robot_state.robot_state == 4 || 
                robot_state.robot_state == 5 || 
                robot_state.robot_state == 7 || // Retroceso
                robot_state.robot_state == 8) { // Home
                
                double real_dist = get_distance_to_objective();
                
                // Tolerancia de 15cm (0.15m). 
                // Si falta más de esto, el robot NO ha llegado, es un falso positivo.
                if (real_dist > 0.15) {
                    RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 1000, 
                        "Falso positivo detectado en Fase %d. Distancia restante: %.2f m. Ignorando...", 
                        robot_state.robot_state, real_dist);
                    return; // ¡NO CAMBIAR DE FASE!
                }
            }

            // Si pasamos todos los filtros, es una llegada real y verificada.
            RCLCPP_INFO(this->get_logger(), "LLEGADA VERIFICADA en Fase %d. Avanzando...", robot_state.robot_state);
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
        
        // --- ESTA LÍNEA ES CRÍTICA PARA EL FILTRO DE TIEMPO ---
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
                // Calcula el tiempo actual
                double seconds = (this->now() - wait_start_time_).seconds();
                
                // IMPRIMIR CADA SEGUNDO (Throttle)
                RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, 
                    "Fase 3: Cerrando Gripper... Tiempo: %.1f / 5.0", seconds);

                // Republish para asegurar que el gripper no se suelte
                arm_objective.gripper = true;
                arm_objective.take_pos = true; // O false, segun tu logica obj_size
                publisher_arm_objective->publish(arm_objective); 

                if (seconds >= 5.0) {
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
        // 1. Definir Objetivo (El punto final de entrega)
        objective.point.x = task.goal.x;
        objective.point.y = task.goal.y;
        objective.angle = angle_goal; 
        
        // Mantenemos la altura Z guardada
        objective_transform_.transform.translation.z = Z_saved; 

        // 2. Enviar comandos (Solo una vez)
        if (!objective_sent_) {
            
            // A. Publicar objetivo a la Base (Para que se acomode fino en el punto exacto)
            objective.robot_state = robot_state.robot_state;
            objective.obj_id = task.obj_id;
            publisher_robot_objective->publish(objective);

            // B. Publicar objetivo al Brazo
            if (task.obj_size == 1) {
                // Objeto chico: Mantenemos gripper cerrado y posición de transporte
                arm_objective.gripper = true; 
                arm_objective.transport_pos = false;
                arm_objective.take_pos = true;
            }
            else if (task.obj_size == 2) {
                // Objeto grande: Lógica de equipo
                if (task.robot_id == task.leader_robot_id) {
                    publisher_detach->publish(std_msgs::msg::Empty());
                    interfaces::msg::ControlFinish finish_msg;
                    finish_msg.finish_confirm = true;
                    publisher_control_finish_team_robot->publish(finish_msg);
                }
                arm_objective.take_pos = true;
                arm_objective.send_finish = true; // El brazo confirma su parte
                arm_objective.transport_pos = false;
            }
            publisher_arm_objective->publish(arm_objective);

            RCLCPP_INFO(this->get_logger(), "Fase 5: Acomodando en punto de entrega (Esperando confirmación de base)...");
            objective_sent_ = true;
        }

        // 3. ACTUALIZAR TF (Para visualización y referencia del brazo)
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        
        // NOTA: Ya no hay Timer. El cambio de fase lo hace control_finish_callback
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

            RCLCPP_INFO(this->get_logger(), "Fase 6: Soltando (6s)...");
            wait_start_time_ = this->now();
            timer_active_ = true;
        } else {
            if ((this->now() - wait_start_time_).seconds() >= 6.0) {
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
        // 1. Calcular el punto de destino (Retroceder)
        if (task.obj_size == 1) {
            // Retroceder 0.5m desde el punto de entrega en la dirección contraria al ángulo de llegada
            objective.point.x = task.goal.x - (1 * cos(angle_goal));
            objective.point.y = task.goal.y - (1 * sin(angle_goal));
            objective.angle = angle_goal; // Mantener orientación o girar, según prefieras
        }
        else if (task.obj_size == 2) {
            // Lógica para objeto grande (Separación de robots)
            if (task.robot_id == task.leader_robot_id) {
                objective.point.x = task.goal.x + (1.0 * cos(-M_PI));
                objective.point.y = task.goal.y + (1.0 * sin(-M_PI));
                objective.angle = 0;
            } else {
                objective.point.x = task.goal.x - (1.0 * cos(M_PI));
                objective.point.y = task.goal.y - (1.0 * sin(M_PI));
                objective.angle = M_PI;
            }
        }

        // Mantener Z
        objective.point.z = Z_saved;
        objective.obj_id = task.obj_id;
        objective.robot_state = robot_state.robot_state;

        // 2. Enviar comandos (Solo una vez)
        if (!objective_sent_) {
            
            // Configurar brazo para el viaje de regreso (Take Pos o Home)
            arm_objective.take_pos = true; 
            arm_objective.gripper = false; // Asegurar gripper abierto
            arm_objective.transport_pos = false;
            
            publisher_arm_objective->publish(arm_objective);
            publisher_robot_objective->publish(objective);
            
            RCLCPP_INFO(this->get_logger(), "Fase 7: Retrocediendo (Esperando confirmación de base)...");
            objective_sent_ = true;
        }

        // 3. Actualizar TF para visualización
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        
        // SIN TIMER: El cambio a Fase 8 lo hará control_finish_callback 
        // cuando el robot llegue al punto calculado.
    }

    void phase_eight_finalize_task() {
        // 1. Recuperar la posición inicial (Guardada en Fase 1)
        objective.point.x = initial_position.point.x;
        objective.point.y = initial_position.point.y;
        objective.point.z = 0.0; // Altura de base (piso)
        
        // Asumimos ángulo 0 o el que tenía al inicio (si lo guardaste)
        // Por lo general volver a 0 o mirar al frente es lo estándar.
        objective.angle = 0.0; 

        objective.robot_state = robot_state.robot_state;
        objective.obj_id = -1; // Ya no hay objeto de interés

        // 2. Enviar comandos (Solo una vez)
        if (!objective_sent_) {
            
            // Configurar Brazo: Ir a HOME, pero NO enviar 'send_finish' todavía.
            // Queremos que el brazo se pliegue mientras la base viaja.
            arm_objective.home_pos = true;
            arm_objective.gripper = false;
            arm_objective.take_pos = false;
            arm_objective.transport_pos = false;
            arm_objective.send_finish = false; // <-- IMPORTANTE: No finalizar por brazo

            publisher_arm_objective->publish(arm_objective);
            publisher_robot_objective->publish(objective);
            
            RCLCPP_INFO(this->get_logger(), "Fase 8: Volviendo a la Posición Inicial (Home)...");
            objective_sent_ = true;
        }

        // 3. Actualizar TF para visualización
        objective_transform_.transform.translation.x = objective.point.x;
        objective_transform_.transform.translation.y = objective.point.y;
        
        // El cambio a Fase 9 ocurrirá cuando 'control_finish_callback'
        // reciba la señal de que el robot llegó a la posicion inicial
    }

    void phase_nine_reset_state() {
        RCLCPP_INFO_ONCE(this->get_logger(), "TAREA FINALIZADA. Reseteando memoria...");

        // 1. AVISAR AL MANAGER Y GUARDAR EN HISTORIAL
        // Usamos el cast (int) para evitar los warnings que vimos antes
        if (static_cast<int>(task.task_id) != -1) {
             std_msgs::msg::Int32 finished_msg;
             finished_msg.data = task.task_id;
             publisher_task_finished_->publish(finished_msg);
             
             // GUARDAR EL ID PARA NO REPETIRLO (Anti-Replay)
             last_completed_task_id_ = static_cast<int>(task.task_id);
        }
        
        // 2. --- BORRADO TOTAL DE LA TAREA (LO QUE PEDISTE) ---
        // Al poner esto en -1, el robot "olvida" qué estaba haciendo.
        task.task_id = -1; 
        task.obj_id = -1;
        
        // 3. LIMPIEZA DE BANDERAS INTERNAS
        reset_phase_flags();
        objective_sent_ = false; // Asegurar que el candado se cierre

        // 4. PASAR A MODO ESPERA (IDLE)
        robot_state.robot_state = 0;
        publisher_robot_state->publish(robot_state);
        
        RCLCPP_INFO(this->get_logger(), "Robot en Espera (State 0). Memoria limpia.");
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