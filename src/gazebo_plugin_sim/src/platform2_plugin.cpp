//Example set velocity 
//ros2 topic pub --once  /robot_platform_2/vel_Motors interfaces/msg/MotorVels "{vel_m1: 20}"
//Example get velocity
//ros2 topic echo /robot_platform_2/sim_vel_Motors 


#include <gazebo/common/Time.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>
#include <gazebo_plugin_sim/platform2_plugin.hpp>
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
#include <interfaces/msg/motor_vels.hpp>

#include <memory>
#include <string>
#include <vector>
#include <chrono>

namespace gazebo_plugin_sim
{
	class Platform2PluginPrivate
	{
	public:
		//Callbacks for subscriptions
		void OnMsgVelM(const interfaces::msg::MotorVels::SharedPtr _msgM);

		//Callback setvelocity
		void SetVelocityM1(const double &_velM1);
		void SetVelocityM2(const double &_velM2);
		void SetVelocityM3(const double &_velM3);
		void SetVelocityM4(const double &_velM4);

		//Callback event update
		void OnUpdate(const gazebo::common::UpdateInfo & /*_info*/);

		/// A pointer to the GazeboROS node.
  		gazebo_ros::Node::SharedPtr ros_node_;

  		//Subscribers
		rclcpp::Subscription<interfaces::msg::MotorVels>::SharedPtr vel_M_;

		//Publishers
		rclcpp::Publisher<interfaces::msg::MotorVels>::SharedPtr sim_vel_M_;


		/// Pointers to wheel joints.
		std::vector<gazebo::physics::JointPtr> joints_;

		/// Pointer to model.
		gazebo::physics::ModelPtr model_;

		//Pointer to the joint
		gazebo::physics::JointPtr jointM1;
		gazebo::physics::JointPtr jointM2;
		gazebo::physics::JointPtr jointM3;
		gazebo::physics::JointPtr jointM4;

		//PID controller for the joint
		gazebo::common::PID pidM1;
		gazebo::common::PID pidM2;
		gazebo::common::PID pidM3;
		gazebo::common::PID pidM4;

		//Event update for publisher
		gazebo::event::ConnectionPtr updateConnection;
	};

	Platform2Plugin::Platform2Plugin()
	: impl_(std::make_unique<Platform2PluginPrivate>())
	{

	} 

	Platform2Plugin::~Platform2Plugin()
	{

	}

	void Platform2Plugin::Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf)
	{
		impl_->model_ = _model;
		
		// Initialize ROS node
  		impl_->ros_node_ = gazebo_ros::Node::Get(_sdf);

  		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Plugin attached to Platform 2");

  		// Get QoS profiles
  		const gazebo_ros::QoS & qos = impl_->ros_node_->get_qos();

  		//Get joints
  		impl_->jointM1 = _model->GetJoints()[0];
  		impl_->jointM2 = _model->GetJoints()[10];
  		impl_->jointM3 = _model->GetJoints()[20];
  		impl_->jointM4 = _model->GetJoints()[30];

  		//Setup PID controller
  		impl_->pidM1 = gazebo::common::PID(0.1,0,0);
  		impl_->pidM2 = gazebo::common::PID(0.1,0,0);
  		impl_->pidM3 = gazebo::common::PID(0.1,0,0);
  		impl_->pidM4 = gazebo::common::PID(0.1,0,0);

  		//Apply PID controller to the joint
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM1->GetScopedName(), impl_->pidM1);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM2->GetScopedName(), impl_->pidM2);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM3->GetScopedName(), impl_->pidM3);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM4->GetScopedName(), impl_->pidM4);

      	// Default to zero velocity
      	double velocityM1 = 0;
      	double velocityM2 = 0;
      	double velocityM3 = 0;
      	double velocityM4 = 0;

      	// Check that the velocity element exists, then read the value
      	if (_sdf->HasElement("velocityM1"))
        	velocityM1 = _sdf->Get<double>("velocityM1");
        if (_sdf->HasElement("velocityM2"))
        	velocityM2 = _sdf->Get<double>("velocityM2");
        if (_sdf->HasElement("velocityM3"))
        	velocityM3 = _sdf->Get<double>("velocityM3");
        if (_sdf->HasElement("velocityM4"))
        	velocityM4 = _sdf->Get<double>("velocityM4");


    	//Set initial velocity
    	impl_->SetVelocityM1(velocityM1);
    	impl_->SetVelocityM2(velocityM2);
    	impl_->SetVelocityM3(velocityM3);
    	impl_->SetVelocityM4(velocityM4);





      	//subscribers
      	impl_->vel_M_ = impl_->ros_node_->create_subscription<interfaces::msg::MotorVels>(
    		"~/vel_Motors", qos.get_subscription_qos("~/vel_Motors", rclcpp::QoS(1)),
    		std::bind(&Platform2PluginPrivate::OnMsgVelM, impl_.get(), std::placeholders::_1));

      	//publishers
      	impl_->sim_vel_M_ = impl_->ros_node_->create_publisher<interfaces::msg::MotorVels>(
      		"~/sim_vel_Motors", qos.get_publisher_qos("~/sim_vel_Motors", rclcpp::QoS(1)));



      	//event update for publisher
      	impl_->updateConnection = gazebo::event::Events::ConnectWorldUpdateBegin(
      		std::bind(&Platform2PluginPrivate::OnUpdate, impl_.get(), std::placeholders::_1));

	}

	//Void set velocity
	void Platform2PluginPrivate::SetVelocityM1(const double &_velM1)
	{
		Platform2PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Platform2PluginPrivate::jointM1->GetScopedName(),_velM1);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform2_M1= %f", _velM1);
	}

	void Platform2PluginPrivate::SetVelocityM2(const double &_velM2)
	{
		Platform2PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Platform2PluginPrivate::jointM2->GetScopedName(),_velM2);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform2_M2= %f", _velM2);
	}

	void Platform2PluginPrivate::SetVelocityM3(const double &_velM3)
	{
		Platform2PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Platform2PluginPrivate::jointM3->GetScopedName(),_velM3);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform2_M3= %f", _velM3);
	}

	void Platform2PluginPrivate::SetVelocityM4(const double &_velM4)
	{
		Platform2PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Platform2PluginPrivate::jointM4->GetScopedName(),_velM4);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform2_M4= %f", _velM4);
	}


	//Callback subscriptions
	void Platform2PluginPrivate::OnMsgVelM(const interfaces::msg::MotorVels::SharedPtr _msgM)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSending Motor Velocities");
		Platform2PluginPrivate::SetVelocityM1(_msgM->vel_m1);
		Platform2PluginPrivate::SetVelocityM2(_msgM->vel_m2);
		Platform2PluginPrivate::SetVelocityM3(_msgM->vel_m3);
		Platform2PluginPrivate::SetVelocityM4(_msgM->vel_m4);

		//time.sleep(0.0);
	}

	//Event update callback for publishers
	void Platform2PluginPrivate::OnUpdate(const gazebo::common::UpdateInfo & /*_info*/)
	{
		//Declaration of variables
		interfaces::msg::MotorVels velM;

		//Get velocity joints
		velM.vel_m1=Platform2PluginPrivate::jointM1->GetVelocity(0);
		velM.vel_m2=Platform2PluginPrivate::jointM2->GetVelocity(0);
		velM.vel_m3=Platform2PluginPrivate::jointM3->GetVelocity(0);
		velM.vel_m4=Platform2PluginPrivate::jointM4->GetVelocity(0);
		//Publish
		Platform2PluginPrivate::sim_vel_M_->publish(velM);
	}




	//Reset
	void Platform2Plugin::Reset()
	{
	  impl_->ros_node_.reset();
	}


	GZ_REGISTER_MODEL_PLUGIN(Platform2Plugin)
}	//namespace gazebo_plugin_sim