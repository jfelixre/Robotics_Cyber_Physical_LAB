#ifndef _MECA_PLATFORM_PLUGIN_TEST_HH_
#define _VMECA_PLATFORM_PLUGIN_TEST_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo_plugin_sim/platform_plugin_test.hpp>

#include <thread>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <rclcpp/subscription_options.hpp>
#include <rclcpp/callback_group.hpp>
#include <gazebo_ros/node.hpp>
#include <std_msgs/msg/string.hpp>

namespace gazebo
{




  /// \brief A plugin to control a Velodyne sensor.
  class MecaPlatformTestPlugin : public ModelPlugin
  {
    /// \brief Constructor
    public: MecaPlatformTestPlugin() {}



    /// \brief The load function is called by Gazebo when the plugin is
    /// inserted into simulation
    /// \param[in] _model A pointer to the model that this plugin is
    /// attached to.
    /// \param[in] _sdf A pointer to the plugin's SDF element.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // Just output a message for now
      std::cerr << "\nThe platform plugin is attach to model....[" <<
        _model->GetName() << "]\n";


      // Create a topic name
      std::string topicNameM1 = "~/" + this->model->GetName() + "/vel_M1";
      std::string topicNameM2 = "~/" + this->model->GetName() + "/vel_M2";
      std::string topicNameM3 = "~/" + this->model->GetName() + "/vel_M3";
      std::string topicNameM4 = "~/" + this->model->GetName() + "/vel_M4";


      //ROS
      this->ros_node_ = gazebo_ros::Node::Get(_sdf);
      
      const gazebo_ros::QoS & qos = this->ros_node_->get_qos();


      //this->vel_M1_=this->ros_node_->create_subscription<std_msgs::msg::Float32>(topicNameM1,
      //  qos.get_subscription_qos(topicNameM1,rclcpp::QoS(1)),
      //  std::bind(&MecaPlatformTestPlugin::OnMsgVelM1,this->ros_node_.get(),std::placeholders::_1));





      // Safety check
      if (_model->GetJointCount() == 0)
      {
        std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
        return;
      }

      // Store the model pointer for convenience.
      this->model = _model;

      // Get the first joint. We are making an assumption about the model
      // having one joint that is the rotational joint.
      this->jointM1 = _model->GetJoints()[0];
      this->jointM2 = _model->GetJoints()[10];
      this->jointM3 = _model->GetJoints()[20];
      this->jointM4 = _model->GetJoints()[30];

      // Setup a P-controller, with a gain of 0.1.
      this->pidM1 = common::PID(0.1, 0, 0);
      this->pidM2 = common::PID(0.1, 0, 0);
      this->pidM3 = common::PID(0.1, 0, 0);
      this->pidM4 = common::PID(0.1, 0, 0);

      // Apply the P-controller to the joint.
      this->model->GetJointController()->SetVelocityPID(
          this->jointM1->GetScopedName(), this->pidM1);
      this->model->GetJointController()->SetVelocityPID(
          this->jointM2->GetScopedName(), this->pidM2);
      this->model->GetJointController()->SetVelocityPID(
          this->jointM3->GetScopedName(), this->pidM3);
      this->model->GetJointController()->SetVelocityPID(
          this->jointM4->GetScopedName(), this->pidM4);



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


      this->SetVelocityM1(velocityM1);
      this->SetVelocityM2(velocityM2);
      this->SetVelocityM3(velocityM3);
      this->SetVelocityM4(velocityM4);

       // Create the node
      this->nodeM1 = transport::NodePtr(new transport::Node());
      this->nodeM1->Init(this->model->GetWorld()->Name());
      this->nodeM2 = transport::NodePtr(new transport::Node());
      this->nodeM2->Init(this->model->GetWorld()->Name());
      this->nodeM3 = transport::NodePtr(new transport::Node());
      this->nodeM3->Init(this->model->GetWorld()->Name());
      this->nodeM4 = transport::NodePtr(new transport::Node());
      this->nodeM4->Init(this->model->GetWorld()->Name());
      this->nodeVel = transport::NodePtr(new transport::Node());
      this->nodeVel->Init(this->model->GetWorld()->Name());


      


      // Subscribe to the topic, and register a callback
      this->subM1 = this->nodeM1->Subscribe(topicNameM1,
         &MecaPlatformTestPlugin::OnMsgM1, this);
      this->subM2 = this->nodeM2->Subscribe(topicNameM2,
         &MecaPlatformTestPlugin::OnMsgM2, this);
      this->subM3 = this->nodeM3->Subscribe(topicNameM3,
         &MecaPlatformTestPlugin::OnMsgM3, this);
      this->subM4 = this->nodeM4->Subscribe(topicNameM4,
         &MecaPlatformTestPlugin::OnMsgM4, this);


      //Para enviar velocidades
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&MecaPlatformTestPlugin::OnUpdate, this, _1));



    }

    /// \brief Set the velocity of the Velodyne
    /// \param[in] _vel New target velocity
    public: void SetVelocityM1(const double &_velM1)//(ignition::math::Vector4d vel)
    {
      // Set the joint's target velocity.
      this->model->GetJointController()->SetVelocityTarget(
          this->jointM1->GetScopedName(), _velM1);
    }
    public: void SetVelocityM2(const double &_velM2)//(ignition::math::Vector4d vel)
    {
      // Set the joint's target velocity.
      this->model->GetJointController()->SetVelocityTarget(
          this->jointM2->GetScopedName(), _velM2);
    }
    public: void SetVelocityM3(const double &_velM3)//(ignition::math::Vector4d vel)
    {
      // Set the joint's target velocity.
      this->model->GetJointController()->SetVelocityTarget(
          this->jointM3->GetScopedName(), _velM3);
    }
    public: void SetVelocityM4(const double &_velM4)//(ignition::math::Vector4d vel)
    {
      // Set the joint's target velocity.
      this->model->GetJointController()->SetVelocityTarget(
          this->jointM4->GetScopedName(), _velM4);
    }

    private: void OnMsgVelM1(const double &_msgM1)
    {
      this->SetVelocityM1(_msgM1);
    }

    /// \brief Handle incoming message
    /// \param[in] _msg Repurpose a vector3 message. This function will
    /// only use the x component.
    private: void OnMsgM1(ConstVector3dPtr &_msgM1)
    {
      this->SetVelocityM1(_msgM1->x());
    }
    private: void OnMsgM2(ConstVector3dPtr &_msgM2)
    {
      this->SetVelocityM2(_msgM2->x());
    }
    private: void OnMsgM3(ConstVector3dPtr &_msgM3)
    {
      this->SetVelocityM3(_msgM3->x());
    }
    private: void OnMsgM4(ConstVector3dPtr &_msgM4)
    {
      this->SetVelocityM4(_msgM4->x());
    }

    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
      {

        double velM1 = this->jointM1->GetVelocity(0);
        double velM2 = this->jointM2->GetVelocity(0);
        double velM3 = this->jointM3->GetVelocity(0);
        double velM4 = this->jointM4->GetVelocity(0);
        
        this->pubM1 =
          this->nodeVel->Advertise<gazebo::msgs::Vector3d>("~/" + this->model->GetName() + "/sim_vel_M1");

        gazebo::msgs::Vector3d msgM1;
        gazebo::msgs::Set(&msgM1, ignition::math::Vector3d(velM1, 0, 0));
        pubM1->Publish(msgM1);

        this->pubM2 =
          this->nodeVel->Advertise<gazebo::msgs::Vector3d>("~/" + this->model->GetName() + "/sim_vel_M2");

        gazebo::msgs::Vector3d msgM2;
        gazebo::msgs::Set(&msgM2, ignition::math::Vector3d(velM2, 0, 0));
        pubM2->Publish(msgM2);

        this->pubM3 =
          this->nodeVel->Advertise<gazebo::msgs::Vector3d>("~/" + this->model->GetName() + "/sim_vel_M3");

        gazebo::msgs::Vector3d msgM3;
        gazebo::msgs::Set(&msgM3, ignition::math::Vector3d(velM3, 0, 0));
        pubM3->Publish(msgM3);

        this->pubM4 =
          this->nodeVel->Advertise<gazebo::msgs::Vector3d>("~/" + this->model->GetName() + "/sim_vel_M4");

        gazebo::msgs::Vector3d msgM4;
        gazebo::msgs::Set(&msgM4, ignition::math::Vector3d(velM4, 0, 0));
        pubM4->Publish(msgM4);
      }





    /// \brief A node used for transport
    private: transport::NodePtr nodeM1;
    private: transport::NodePtr nodeM2;
    private: transport::NodePtr nodeM3;
    private: transport::NodePtr nodeM4;
    private: transport::NodePtr nodeVel;

    /// \brief A subscriber to a named topic.
    private: transport::SubscriberPtr subM1;
    private: transport::SubscriberPtr subM2;
    private: transport::SubscriberPtr subM3;
    private: transport::SubscriberPtr subM4;

    // Publishers
    private: transport::PublisherPtr pubM1;
    private: transport::PublisherPtr pubM2;
    private: transport::PublisherPtr pubM3;
    private: transport::PublisherPtr pubM4;

    /// \brief Pointer to the model.
    private: physics::ModelPtr model;

    /// \brief Pointer to the joint.
    private: physics::JointPtr jointM1;
    private: physics::JointPtr jointM2;
    private: physics::JointPtr jointM3;
    private: physics::JointPtr jointM4;

    /// \brief A PID controller for the joint.
    private: common::PID pidM1;
    private: common::PID pidM2;
    private: common::PID pidM3;
    private: common::PID pidM4;

    private: event::ConnectionPtr updateConnection;


    /// \brief A node use for ROS transport
    private: gazebo_ros::Node::SharedPtr ros_node_;
    private: rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr vel_M1_;


  
  };

      // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
      GZ_REGISTER_MODEL_PLUGIN(MecaPlatformTestPlugin)
}
#endif