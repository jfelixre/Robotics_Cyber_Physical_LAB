Scripts .ino uses micro-ros library on arduino ide.


# BUILD FIRMWARE BEFORE UPLOAD FILES WITH ARDUINO   (Talvez no es necesario)
ros2 run micro_ros_setup create_firmware_ws.sh freertos esp32
ros2 run micro_ros_setup configure_firmware.sh int32_publisher -t udp -i [your local machine IP] -p 8888
ros2 run micro_ros_setup build_firmware.sh menuconfig

# Now go to the micro-ROS Transport Settings → WiFi Configuration menu and fill your WiFi SSID and password. Save your changes, exit the interactive menu, and run:
ros2 run micro_ros_setup build_firmware.sh

# Connect your ESP32 to the computer with a micro-USB cable, and run:
ros2 run micro_ros_setup flash_firmware.sh


# CREATE MICRO ROS AGENT

# Download micro-ROS-Agent packages
ros2 run micro_ros_setup create_agent_ws.sh

# Build step
ros2 run micro_ros_setup build_agent.sh
source install/local_setup.bash



To start micro-ros on esp32 run this:
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888^C   (Checar pasos intermedios)