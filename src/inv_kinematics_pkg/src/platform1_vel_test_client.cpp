#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> Platform1VelClient_node = rclcpp::Node::make_shared("platform1_vel_client");

  rclcpp::Client<interfaces::srv::PlatformVel>::SharedPtr client =
    Platform1VelClient_node->create_client<interfaces::srv::PlatformVel>("platform1_vel");

  auto request = std::make_shared<interfaces::srv::PlatformVel::Request>();
  request->x_vel = atof(argv[1]);
  request->y_vel = atof(argv[2]);
  request->ang_vel = atof(argv[3]);

  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  auto result = client->async_send_request(request);
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(Platform1VelClient_node, result) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to platform1");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service platform1_vel");
  }

  rclcpp::shutdown();
  return 0;
}