#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/int32.hpp>
#include <memory>
#include <cinttypes>
#include <sstream>

class RobotEmergencyStopNode : public rclcpp::Node
{
public:
    RobotEmergencyStopNode() : Node("robot_emergency_stop_node")
    {
        this->declare_parameter<int>("robot_id", 1);
        robot_id_ = this->get_parameter("robot_id").as_int();
        RCLCPP_INFO(this->get_logger(), "Iniciando parada de emergencia para Robot_ID: %d", robot_id_);

        // Crear publishers para los 4 motores del robot especificado
        std::stringstream ss_m1, ss_m2, ss_m3, ss_m4;
        ss_m1 << "/robot_0" << robot_id_ << "/m1_joint/controller";
        ss_m2 << "/robot_0" << robot_id_ << "/m2_joint/controller";
        ss_m3 << "/robot_0" << robot_id_ << "/m3_joint/controller";
        ss_m4 << "/robot_0" << robot_id_ << "/m4_joint/controller";

        publisher_M1_ = this->create_publisher<std_msgs::msg::Float64>(ss_m1.str(), 1);
        publisher_M2_ = this->create_publisher<std_msgs::msg::Float64>(ss_m2.str(), 1);
        publisher_M3_ = this->create_publisher<std_msgs::msg::Float64>(ss_m3.str(), 1);
        publisher_M4_ = this->create_publisher<std_msgs::msg::Float64>(ss_m4.str(), 1);

        // Publisher para enviar señal de emergencia al event_driven_control
        std::stringstream ss_emergency;
        ss_emergency << "/robot_0" << robot_id_ << "/emergency_stop";
        publisher_emergency_ = this->create_publisher<std_msgs::msg::Int32>(ss_emergency.str(), 1);

        // Timer para enviar velocidades 0 cada 100ms indefinidamente
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&RobotEmergencyStopNode::send_zero_velocities, this));

        RCLCPP_INFO(this->get_logger(), "Enviando velocidades 0 al robot %d indefinidamente...", robot_id_);
        RCLCPP_INFO(this->get_logger(), "Presiona Ctrl+C para detener el nodo de emergencia");
    }

private:
    void send_emergency_signal()
    {
        std_msgs::msg::Int32 emergency_msg;
        emergency_msg.data = 99; // Estado de emergencia
        publisher_emergency_->publish(emergency_msg);
        RCLCPP_INFO(this->get_logger(), "Señal de emergencia enviada al robot %d", robot_id_);
    }

    void send_zero_velocities()
    {
        // Enviar señal de emergencia cada 1 segundo para asegurar que llegue
        static auto last_emergency_signal = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed_emergency = std::chrono::duration_cast<std::chrono::seconds>(now - last_emergency_signal).count();
        
        if (elapsed_emergency >= 1) {
            send_emergency_signal();
            last_emergency_signal = now;
        }
        
        std_msgs::msg::Float64 zero;
        zero.data = 0.0;

        publisher_M1_->publish(zero);
        publisher_M2_->publish(zero);
        publisher_M3_->publish(zero);
        publisher_M4_->publish(zero);

        // Log cada 10 segundos para no saturar la consola
        static auto last_log = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_log).count();
        
        if (elapsed >= 10) {
            RCLCPP_INFO(this->get_logger(), "Manteniendo velocidades en 0 para robot %d (cada 100ms)", robot_id_);
            last_log = now;
        }
    }

    int robot_id_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M1_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M2_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M3_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_M4_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_emergency_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotEmergencyStopNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}