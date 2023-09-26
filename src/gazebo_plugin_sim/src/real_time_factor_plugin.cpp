#include <gazebo/common/Time.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>
#include <gazebo_plugin_sim/real_time_factor_plugin.hpp>
#include <gazebo_ros/conversions/builtin_interfaces.hpp>
#include <gazebo_ros/conversions/geometry_msgs.hpp>
#include <gazebo_ros/node.hpp>
#include <geometry_msgs/msg/pose2_d.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sdf/sdf.hh>
#include <std_msgs/msg/float32.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo_ros/executor.hpp>
#include <interfaces/msg/motor_platform_vels.hpp>
#include <interfaces/msg/motor_vels_w_arm.hpp>
#include <interfaces/msg/motor_arm_vels.hpp>
#include <interfaces/msg/limit_switch.hpp>

#include <memory>
#include <string>
#include <vector>
#include <chrono>
namespace gazebo
{
  class RealTimeFactorPlugin : public WorldPlugin
  {
  public:
    RealTimeFactorPlugin() : WorldPlugin() {}

        void Load(physics::WorldPtr _world, sdf::ElementPtr /*_sdf*/) override
        {
            // Connect to the World Update event
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                std::bind(&RealTimeFactorPlugin::OnUpdate, this));

            world = _world;
        }

		

		 void OnUpdate()
        {
            // Get the current simulation time and real-time factor
            common::Time simTime = world->SimTime();
            double realTimeFactor = world->Physics()->GetRealTimeUpdateRate();

            // Print the real-time factor to the console
            std::cout << "Simulation Time: " << simTime.Double() << " sec | Real-Time Factor: " << realTimeFactor << std::endl;
        }

    private:
        physics::WorldPtr world;
        event::ConnectionPtr updateConnection;


	};

	
	GZ_REGISTER_WORLD_PLUGIN(RealTimeFactorPlugin)
}	//namespace gazebo_plugin_sim