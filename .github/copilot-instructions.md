# GitHub Copilot Instructions for Robotics_Cyber_Physical_LAB

This is a ROS 2 (Foxy/Humble compatible) workspace for a multi-robot cyber-physical system. The project involves simulation (Gazebo), real hardware control (Jetson), and complex coordination between robots.

## 🏗 Project Architecture

- **Core Logic**: `control_pkg` contains the main control algorithms (PID, Trajectory, A*).
- **Communication**: `interfaces` defines all custom `.msg` and `.srv` files. **CRITICAL**: This package is a dependency for almost everything else.
- **Simulation**: `gazebo_plugin_sim` and `robot_custom_description` handle the Gazebo simulation environment and robot models (SDF/URDF).
- **Launch**: `launch_pkg` centralizes launch files for experiments and robot bring-up.
- **Image Processing**: `img_proc_pkg` handles vision tasks.

### Key Data Flows
- **Task Assignment**: `task_pkg` -> `/robot_0X/task_assigned` -> `control_pkg` (Event Driven Control).
- **State Management**: Robots publish state to `/robot_0X/robot_state`.
- **Multi-Robot Naming**: Resources are typically namespaced or suffixed with `_01`, `_02` or `_r1`, `_r2`.

## 🛠 Build & Development Workflow

### Build System
- Use `colcon` for building.
- **Always** use `--symlink-install` to avoid rebuilding for Python changes and config updates.
- **Build Command**:
  ```bash
  colcon build --symlink-install
  ```
- **Dependency Chain**: If you modify `interfaces`, you MUST rebuild it before building dependent packages (`control_pkg`, etc.).
  ```bash
  colcon build --packages-select interfaces
  colcon build --packages-select control_pkg
  ```

### Environment
- Source the workspace after building:
  ```bash
  source install/setup.zsh  # (or setup.bash)
  ```

### Running Code
- Prefer `ros2 launch` over `ros2 run`.
- Main simulation entry points:
  ```bash
  ros2 launch launch_pkg gazebo_sim_exp_1.launch.py
  ros2 launch launch_pkg gazebo_sim_exp_2.launch.py
  ```

## 🧩 Coding Conventions & Patterns

### C++ (ROS 2 Nodes)
- **Headers**: Include standard C++ headers, then ROS 2 headers, then local headers.
- **Namespaces**: Use `std::` explicitly or `using namespace std;` (common in this codebase, though generally discouraged in strict C++).
- **Parameters**: heavily used for `robot_id` configuration.
  ```cpp
  this->declare_parameter<int>("robot_id", 0);
  robot_id = this->get_parameter("robot_id").as_int();
  ```
- **TF2**: Use `tf2_ros::Buffer` and `tf2_ros::TransformListener` for coordinate transforms.

### Custom Interfaces
- Located in `src/interfaces`.
- Include format: `#include <interfaces/msg/your_message.hpp>`
- When adding a new message, update `src/interfaces/CMakeLists.txt` and `package.xml`.

### Event Driven Control Pattern
- The `Event_Driven_Control` class in `control_pkg` is a central pattern.
- It subscribes to tasks and manages a state machine (`robot_state`).
- Use `std::stringstream` to construct dynamic topic names based on `robot_id`.
  ```cpp
  std::stringstream ss;
  ss << "/robot_0" << robot_id << "/topic_name";
  ```

## ⚠️ Common Pitfalls
- **Omitted Dependencies**: If a node crashes with "symbol lookup error" or similar, check `package.xml` and `CMakeLists.txt` for missing dependencies (especially `interfaces`).
- **Sourcing**: Always source `install/setup.zsh` in **every** new terminal.
- **Gazebo Models**: If models don't spawn, check `GAZEBO_MODEL_PATH` or the paths in `robot_custom_description`.
