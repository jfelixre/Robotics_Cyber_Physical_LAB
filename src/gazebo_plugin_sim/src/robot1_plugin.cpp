//Example set velocity 
//ros2 topic pub --once  /robot_platform_1/vel_Motors interfaces/msg/MotorPlatformVels "{vel_m1: 20}"
//Example get velocity
//ros2 topic echo /robot_platform_1/encoders 


#include <gazebo/common/Time.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>
#include <gazebo_plugin_sim/robot1_plugin.hpp>
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

namespace gazebo_plugin_sim
{
	class Robot1PluginPrivate
	{
	public:
		//Callbacks for subscriptions
		void OnMsgVelM(const interfaces::msg::MotorPlatformVels::SharedPtr _msgM);
		void OnMsgVelArm(const interfaces::msg::MotorArmVels::SharedPtr _msgArm);

		//Callback setvelocity
		void SetVelocityM1(const double &_velM1);
		void SetVelocityM2(const double &_velM2);
		void SetVelocityM3(const double &_velM3);
		void SetVelocityM4(const double &_velM4);
		void SetVelocityB1(const double &_velB1);
		void SetVelocityB2(const double &_velB2);
		void SetVelocityB3(const double &_velB3);
		void SetVelocityG1(const double &_velG1);

		//Callback event update
		void OnUpdate(const gazebo::common::UpdateInfo & /*_info*/);

		/// A pointer to the GazeboROS node.
  		gazebo_ros::Node::SharedPtr ros_node_;

  		//Subscribers
		rclcpp::Subscription<interfaces::msg::MotorPlatformVels>::SharedPtr vel_M_;
		rclcpp::Subscription<interfaces::msg::MotorArmVels>::SharedPtr vel_Arm_;

		//Publishers
		rclcpp::Publisher<interfaces::msg::MotorVelsWArm>::SharedPtr sim_vel_M_;
		rclcpp::Publisher<interfaces::msg::LimitSwitch>::SharedPtr LmSw_;


		/// Pointers to wheel joints.
		std::vector<gazebo::physics::JointPtr> joints_;

		/// Pointer to model.
		gazebo::physics::ModelPtr model_;

		//Pointer to the joint
		gazebo::physics::JointPtr jointM1;
		gazebo::physics::JointPtr jointM2;
		gazebo::physics::JointPtr jointM3;
		gazebo::physics::JointPtr jointM4;
		gazebo::physics::JointPtr jointB1;
		gazebo::physics::JointPtr jointB2;
		gazebo::physics::JointPtr jointB3;
		gazebo::physics::JointPtr jointG1;

		//PID controller for the joint
		gazebo::common::PID pidM1;
		gazebo::common::PID pidM2;
		gazebo::common::PID pidM3;
		gazebo::common::PID pidM4;
		gazebo::common::PID pidB1;
		gazebo::common::PID pidB2;
		gazebo::common::PID pidB3;
		gazebo::common::PID pidG1;

		//Event update for publisher
		gazebo::event::ConnectionPtr updateConnection;
	};

	Robot1Plugin::Robot1Plugin()
	: impl_(std::make_unique<Robot1PluginPrivate>())
	{

	} 

	Robot1Plugin::~Robot1Plugin()
	{

	}

	void Robot1Plugin::Load(gazebo::physics::ModelPtr _model, sdf::ElementPtr _sdf)
	{
		impl_->model_ = _model;
		
		// Initialize ROS node
  		impl_->ros_node_ = gazebo_ros::Node::Get(_sdf);

  		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Plugin attached to Robot 1");

  		// Get QoS profiles
  		const gazebo_ros::QoS & qos = impl_->ros_node_->get_qos();

  		//Get joints
  		impl_->jointM1 = _model->GetJoints()[3];
  		impl_->jointM2 = _model->GetJoints()[13];
  		impl_->jointM3 = _model->GetJoints()[23];
  		impl_->jointM4 = _model->GetJoints()[33];
  		impl_->jointB1 = _model->GetJoints()[0];
  		impl_->jointB2 = _model->GetJoints()[1];
  		impl_->jointB3 = _model->GetJoints()[2];
  		impl_->jointG1 = _model->GetJoints()[43];

  		//Setup PID controller
  		impl_->pidM1 = gazebo::common::PID(0.1,0,0);
  		impl_->pidM2 = gazebo::common::PID(0.1,0,0);
  		impl_->pidM3 = gazebo::common::PID(0.1,0,0);
  		impl_->pidM4 = gazebo::common::PID(0.1,0,0);
  		impl_->pidB1 = gazebo::common::PID(0.1,0,0);
  		impl_->pidB2 = gazebo::common::PID(0.1,0,0);
  		impl_->pidB3 = gazebo::common::PID(0.1,0,0);
  		impl_->pidG1 = gazebo::common::PID(0.01,0,0);

  		//Apply PID controller to the joint
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM1->GetScopedName(), impl_->pidM1);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM2->GetScopedName(), impl_->pidM2);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM3->GetScopedName(), impl_->pidM3);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointM4->GetScopedName(), impl_->pidM4);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointB1->GetScopedName(), impl_->pidB1);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointB2->GetScopedName(), impl_->pidB2);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointB3->GetScopedName(), impl_->pidB3);
  		impl_->model_->GetJointController()->SetVelocityPID(
  			impl_->jointG1->GetScopedName(), impl_->pidG1);

      	// Default to zero velocity
      	double velocityM1 = 0;
      	double velocityM2 = 0;
      	double velocityM3 = 0;
      	double velocityM4 = 0;
      	double velocityB1 = 0;
      	double velocityB2 = 0;
      	double velocityB3 = 0;
      	double velocityG1 = 1;

      	// Check that the velocity element exists, then read the value
      	if (_sdf->HasElement("velocityM1"))
        	velocityM1 = _sdf->Get<double>("velocityM1");
        if (_sdf->HasElement("velocityM2"))
        	velocityM2 = _sdf->Get<double>("velocityM2");
        if (_sdf->HasElement("velocityM3"))
        	velocityM3 = _sdf->Get<double>("velocityM3");
        if (_sdf->HasElement("velocityM4"))
        	velocityM4 = _sdf->Get<double>("velocityM4");
        if (_sdf->HasElement("velocityB1"))
        	velocityB1 = _sdf->Get<double>("velocityB1");
        if (_sdf->HasElement("velocityB2"))
        	velocityB2 = _sdf->Get<double>("velocityB2");
        if (_sdf->HasElement("velocityB3"))
        	velocityB3 = _sdf->Get<double>("velocityB3");
        if (_sdf->HasElement("velocityG1"))
        	velocityG1 = _sdf->Get<double>("velocityG1");


    	//Set initial velocity
    	impl_->SetVelocityM1(velocityM1);
    	impl_->SetVelocityM2(velocityM2);
    	impl_->SetVelocityM3(velocityM3);
    	impl_->SetVelocityM4(velocityM4);
    	impl_->SetVelocityB1(velocityB1);
    	impl_->SetVelocityB2(velocityB2);
    	impl_->SetVelocityB3(velocityB3);
    	impl_->SetVelocityG1(velocityG1);





      	//subscribers
      	impl_->vel_M_ = impl_->ros_node_->create_subscription<interfaces::msg::MotorPlatformVels>(
    		"~/platform_vel_motors", qos.get_subscription_qos("~/platform_vel_motors", rclcpp::QoS(1)),
    		std::bind(&Robot1PluginPrivate::OnMsgVelM, impl_.get(), std::placeholders::_1));

		impl_->vel_Arm_ = impl_->ros_node_->create_subscription<interfaces::msg::MotorArmVels>(
    		"~/set_arm_joints_vel", qos.get_subscription_qos("~/set_arm_joints_vel", rclcpp::QoS(1)),
    		std::bind(&Robot1PluginPrivate::OnMsgVelArm, impl_.get(), std::placeholders::_1));

      	//publishers
      	impl_->sim_vel_M_ = impl_->ros_node_->create_publisher<interfaces::msg::MotorVelsWArm>(
      		"~/encoders", qos.get_publisher_qos("~/encoders", rclcpp::QoS(1)));

      	impl_->LmSw_ = impl_->ros_node_->create_publisher<interfaces::msg::LimitSwitch>(
      		"~/limit_switch", qos.get_publisher_qos("~/limit_switch", rclcpp::QoS(1)));


      	//event update for publisher
      	impl_->updateConnection = gazebo::event::Events::ConnectWorldUpdateBegin(
      		std::bind(&Robot1PluginPrivate::OnUpdate, impl_.get(), std::placeholders::_1));

	}

	//Void set velocity
	void Robot1PluginPrivate::SetVelocityM1(const double &_velM1)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointM1->GetScopedName(),_velM1);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_M1= %f", _velM1);
	}

	void Robot1PluginPrivate::SetVelocityM2(const double &_velM2)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointM2->GetScopedName(),_velM2);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_M2= %f", _velM2);
	}

	void Robot1PluginPrivate::SetVelocityM3(const double &_velM3)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointM3->GetScopedName(),_velM3);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_M3= %f", _velM3);
	}

	void Robot1PluginPrivate::SetVelocityM4(const double &_velM4)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointM4->GetScopedName(),_velM4);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_M4= %f", _velM4);
	}

	void Robot1PluginPrivate::SetVelocityB1(const double &_velB1)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointB1->GetScopedName(),_velB1);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_B1= %f", _velB1);
	}

	void Robot1PluginPrivate::SetVelocityB2(const double &_velB2)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointB2->GetScopedName(),_velB2);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_B2= %f", _velB2);
	}

	void Robot1PluginPrivate::SetVelocityB3(const double &_velB3)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointB3->GetScopedName(),_velB3);
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSet_Velocity_Robot1_B3= %f", _velB3);
	}

	void Robot1PluginPrivate::SetVelocityG1(const double &_velG1)
	{
		Robot1PluginPrivate::model_->GetJointController()->SetVelocityTarget(
			Robot1PluginPrivate::jointG1->GetScopedName(),_velG1);
		if (_velG1==1.0){
			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nGripper= Open");
		}
		if (_velG1==-1.0){
			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nGripper= Close");
		}
	}


	//Callback subscriptions
	void Robot1PluginPrivate::OnMsgVelM(const interfaces::msg::MotorPlatformVels::SharedPtr _msgM)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSending Motor Platform Velocities");
		Robot1PluginPrivate::SetVelocityM1(_msgM->vel_m1);
		Robot1PluginPrivate::SetVelocityM2(_msgM->vel_m2);
		Robot1PluginPrivate::SetVelocityM3(_msgM->vel_m3);
		Robot1PluginPrivate::SetVelocityM4(_msgM->vel_m4);
	}

	void Robot1PluginPrivate::OnMsgVelArm(const interfaces::msg::MotorArmVels::SharedPtr _msgArm)
	{
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\nSending Motor Arm Velocities");
		Robot1PluginPrivate::SetVelocityB1(_msgArm->vel_b1);
		Robot1PluginPrivate::SetVelocityB2(_msgArm->vel_b2);
		Robot1PluginPrivate::SetVelocityB3(_msgArm->vel_b3);
		Robot1PluginPrivate::SetVelocityG1(_msgArm->vel_g1);
	}

	//Event update callback for publishers
	void Robot1PluginPrivate::OnUpdate(const gazebo::common::UpdateInfo & /*_info*/)
	{
		//Declaration of variables
		interfaces::msg::MotorVelsWArm velM;
		interfaces::msg::LimitSwitch LM;
		float B1_pos, B2_pos, B3_pos;
		int LSB1_min=0;
		int LSB1_max=0;
		int LSB2_min=0;
		int LSB2_max=0;
		int LSB3_min=0;
		int LSB3_max=0;

		//Get velocity joints
		velM.vel_m1=Robot1PluginPrivate::jointM1->GetVelocity(0);
		velM.vel_m2=Robot1PluginPrivate::jointM2->GetVelocity(0);
		velM.vel_m3=Robot1PluginPrivate::jointM3->GetVelocity(0);
		velM.vel_m4=Robot1PluginPrivate::jointM4->GetVelocity(0);
		velM.vel_b1=Robot1PluginPrivate::jointB1->GetVelocity(0);
		velM.vel_b2=Robot1PluginPrivate::jointB2->GetVelocity(0);
		velM.vel_b3=Robot1PluginPrivate::jointB3->GetVelocity(0);
		velM.vel_g1=Robot1PluginPrivate::jointG1->GetVelocity(0);
		velM.vel_b1=Robot1PluginPrivate::jointB1->GetVelocity(0);
		velM.vel_b2=Robot1PluginPrivate::jointB2->GetVelocity(0);
		velM.vel_b3=(Robot1PluginPrivate::jointB3->GetVelocity(0)) * 0.02;

		//Get arm positions
		B1_pos=Robot1PluginPrivate::jointB1->Position(0);
		B2_pos=Robot1PluginPrivate::jointB2->Position(0);
		B3_pos=Robot1PluginPrivate::jointB3->Position(0);

		//velM.vel_b1=B1_pos;
		//velM.vel_b2=B2_pos*-1;
		//velM.vel_b3=B3_pos;
		//B2_pos=B2_pos*-1;

		if (B1_pos<-1.79){
			LSB1_min=1;
		}

		if (B1_pos>0.99){
			LSB1_max=1;
		}

		if (B2_pos<-2.09){
			LSB2_max=1;
		}

		if (B2_pos>0.79){
			LSB2_min=1;
		}

		if (B3_pos<-1.99){
			LSB3_min=1;
		}

		if (B3_pos>0.99){
			LSB3_max=1;
		}

		LM.b1_min=LSB1_min;
		LM.b1_max=LSB1_max;
		LM.b2_min=LSB2_min;
		LM.b2_max=LSB2_max;
		LM.b3_min=LSB3_min;
		LM.b3_max=LSB3_max;


		//Publish ;
		Robot1PluginPrivate::sim_vel_M_->publish(velM);
		Robot1PluginPrivate::LmSw_->publish(LM);
	}




	//Reset
	void Robot1Plugin::Reset()
	{
	  impl_->ros_node_.reset();
	}


	GZ_REGISTER_MODEL_PLUGIN(Robot1Plugin)
}	//namespace gazebo_plugin_sim