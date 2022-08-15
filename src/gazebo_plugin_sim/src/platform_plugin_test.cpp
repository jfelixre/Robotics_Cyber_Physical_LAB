//Example set velocity 
//ros2 topic pub --once  /robot_platform_1/vel_M1 geometry_msgs/msg/Vector3 "{x: 20}"
//Example get velocity
//ros2 topic echo /robot_platform_1/sim_vel_M1 


#include <gazebo/common/Time.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>
#include <gazebo_plugin_sim/platform_plugin_test.hpp>
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

#include <memory>
#include <string>
#include <vector>

namespace gazebo_plugin_sim
{
	class PlatformPluginTestPrivate
	{
	public:
		//Callbacks for subscriptions
		void OnMsgVelM1(const geometry_msgs::msg::Vector3::SharedPtr _msgM1);
		void OnMsgVelM2(const geometry_msgs::msg::Vector3::SharedPtr _msgM2);
		void OnMsgVelM3(const geometry_msgs::msg::Vector3::SharedPtr _msgM3);
		void OnMsgVelM4(const geometry_msgs::msg::Vector3::SharedPtr _msgM4);

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
		rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr vel_M1_;
		rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr vel_M2_;
		rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr vel_M3_;
		rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr vel_M4_;

		//Publishers
		rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr sim_vel_M1_;
		rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr sim_vel_M2_;
		rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr sim_vel_M3_;
		rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr sim_vel_M4_;

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

	PlatformPluginTest::PlatformPluginTest()
	: impl_(std::make_unique<PlatformPluginTestPrivate>())
	{

	} 

	PlatformPluginTest::~PlatformPluginTest()
	{

	}

	void PlatformPluginTest::Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf)
	{
		impl_->model_ = _model;
		
		// Initialize ROS node
  		impl_->ros_node_ = gazebo_ros::Node::Get(_sdf);

  		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Plugin attached to Platform 1");

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
      	impl_->vel_M1_ = impl_->ros_node_->create_subscription<geometry_msgs::msg::Vector3>(
    		"~/vel_M1", qos.get_subscription_qos("~/vel_M1", rclcpp::QoS(0)),
    		std::bind(&PlatformPluginTestPrivate::OnMsgVelM1, impl_.get(), std::placeholders::_1));
      	impl_->vel_M2_ = impl_->ros_node_->create_subscription<geometry_msgs::msg::Vector3>(
    		"~/vel_M2", qos.get_subscription_qos("~/vel_M2", rclcpp::QoS(0)),
    		std::bind(&PlatformPluginTestPrivate::OnMsgVelM2, impl_.get(), std::placeholders::_1));
      	impl_->vel_M3_ = impl_->ros_node_->create_subscription<geometry_msgs::msg::Vector3>(
    		"~/vel_M3", qos.get_subscription_qos("~/vel_M3", rclcpp::QoS(0)),
    		std::bind(&PlatformPluginTestPrivate::OnMsgVelM3, impl_.get(), std::placeholders::_1));
      	impl_->vel_M4_ = impl_->ros_node_->create_subscription<geometry_msgs::msg::Vector3>(
    		"~/vel_M4", qos.get_subscription_qos("~/vel_M4", rclcpp::QoS(0)),
    		std::bind(&PlatformPluginTestPrivate::OnMsgVelM4, impl_.get(), std::placeholders::_1));

      	//publishers
      	impl_->sim_vel_M1_ = impl_->ros_node_->create_publisher<geometry_msgs::msg::Vector3>(
      		"~/sim_vel_M1", qos.get_publisher_qos("~/sim_vel_M1", rclcpp::QoS(1)));
      	impl_->sim_vel_M2_ = impl_->ros_node_->create_publisher<geometry_msgs::msg::Vector3>(
      		"~/sim_vel_M2", qos.get_publisher_qos("~/sim_vel_M2", rclcpp::QoS(1)));
      	impl_->sim_vel_M3_ = impl_->ros_node_->create_publisher<geometry_msgs::msg::Vector3>(
      		"~/sim_vel_M3", qos.get_publisher_qos("~/sim_vel_M3", rclcpp::QoS(1)));
      	impl_->sim_vel_M4_ = impl_->ros_node_->create_publisher<geometry_msgs::msg::Vector3>(
      		"~/sim_vel_M4", qos.get_publisher_qos("~/sim_vel_M4", rclcpp::QoS(1)));


      	//event update for publisher
      	impl_->updateConnection = gazebo::event::Events::ConnectWorldUpdateBegin(
      		std::bind(&PlatformPluginTestPrivate::OnUpdate, impl_.get(), std::placeholders::_1));

	}

	//Void set velocity
	void PlatformPluginTestPrivate::SetVelocityM1(const double &_velM1)
	{
		PlatformPluginTestPrivate::model_->GetJointController()->SetVelocityTarget(
			PlatformPluginTestPrivate::jointM1->GetScopedName(),_velM1);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform1_M1= %f", _velM1);
	}

	void PlatformPluginTestPrivate::SetVelocityM2(const double &_velM2)
	{
		PlatformPluginTestPrivate::model_->GetJointController()->SetVelocityTarget(
			PlatformPluginTestPrivate::jointM2->GetScopedName(),_velM2);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform1_M2= %f", _velM2);
	}

	void PlatformPluginTestPrivate::SetVelocityM3(const double &_velM3)
	{
		PlatformPluginTestPrivate::model_->GetJointController()->SetVelocityTarget(
			PlatformPluginTestPrivate::jointM3->GetScopedName(),_velM3);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform1_M3= %f", _velM3);
	}

	void PlatformPluginTestPrivate::SetVelocityM4(const double &_velM4)
	{
		PlatformPluginTestPrivate::model_->GetJointController()->SetVelocityTarget(
			PlatformPluginTestPrivate::jointM4->GetScopedName(),_velM4);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Platform1_M4= %f", _velM4);
	}


	//Callback subscriptions
	void PlatformPluginTestPrivate::OnMsgVelM1(const geometry_msgs::msg::Vector3::SharedPtr _msgM1)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nPross M1");
		PlatformPluginTestPrivate::SetVelocityM1(_msgM1->x);
	}

	void PlatformPluginTestPrivate::OnMsgVelM2(const geometry_msgs::msg::Vector3::SharedPtr _msgM2)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nPross M2");
		PlatformPluginTestPrivate::SetVelocityM2(_msgM2->x);
	}

	void PlatformPluginTestPrivate::OnMsgVelM3(const geometry_msgs::msg::Vector3::SharedPtr _msgM3)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nPross M3");
		PlatformPluginTestPrivate::SetVelocityM3(_msgM3->x);
	}

	void PlatformPluginTestPrivate::OnMsgVelM4(const geometry_msgs::msg::Vector3::SharedPtr _msgM4)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nPross M4");
		PlatformPluginTestPrivate::SetVelocityM4(_msgM4->x);
	}


	//Event update callback for publishers
	void PlatformPluginTestPrivate::OnUpdate(const gazebo::common::UpdateInfo & /*_info*/)
	{
		//Declaration of variables
		geometry_msgs::msg::Vector3 velM1;
		geometry_msgs::msg::Vector3 velM2;
		geometry_msgs::msg::Vector3 velM3;
		geometry_msgs::msg::Vector3 velM4;
		//Get velocity joints
		velM1.x=PlatformPluginTestPrivate::jointM1->GetVelocity(0);
		velM2.x=PlatformPluginTestPrivate::jointM2->GetVelocity(0);
		velM3.x=PlatformPluginTestPrivate::jointM3->GetVelocity(0);
		velM4.x=PlatformPluginTestPrivate::jointM4->GetVelocity(0);
		//Publish
		PlatformPluginTestPrivate::sim_vel_M1_->publish(velM1);
		PlatformPluginTestPrivate::sim_vel_M2_->publish(velM2);
		PlatformPluginTestPrivate::sim_vel_M3_->publish(velM3);
		PlatformPluginTestPrivate::sim_vel_M4_->publish(velM4);
	}




	//Reset
	void PlatformPluginTest::Reset()
	{
	  impl_->ros_node_.reset();
	}


	GZ_REGISTER_MODEL_PLUGIN(PlatformPluginTest)
}	//namespace gazebo_plugin_sim