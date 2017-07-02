#ifndef _VELODYNE_PLUGIN_HH_
#define _VELODYNE_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float32.h"

namespace gazebo
{
  class VelodynePlugin : public ModelPlugin
  {
    private: std::unique_ptr<ros::NodeHandle> rosNode;

    private: ros::Subscriber rosSubL;

    private: ros::Subscriber rosSubR;

    private: ros::CallbackQueue rosQueue;

    private: std::thread rosQueueThread;

    private: physics::ModelPtr model;

    private: physics::JointPtr left_wheel_joint;

    private: physics::JointPtr right_wheel_joint;

    private: transport::NodePtr node;

    private: transport::SubscriberPtr sub;

    private: common::PID pid;

    public: VelodynePlugin() {}

    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
        if (_model->GetJointCount() == 0)
        {
        std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
        return;
        }

        this->model = _model;
        this->left_wheel_joint = _model->GetJoints()[0];
        this->right_wheel_joint = _model->GetJoints()[1];

        this->pid = common::PID(0.1, 0, 0);

        this->model->GetJointController()->SetVelocityPID(
          this->left_wheel_joint->GetScopedName(), this->pid);
        this->model->GetJointController()->SetVelocityPID(
          this->right_wheel_joint->GetScopedName(), this->pid);

        this->node = transport::NodePtr(new transport::Node());
        #if GAZEBO_MAJOR_VERSION < 8
        this->node->Init(this->model->GetWorld()->GetName());
        #else
        this->node->Init(this->model->GetWorld()->Name());
        #endif

        if (!ros::isInitialized())
        {
          int argc = 0;
          char **argv = NULL;
          ros::init(argc, argv, "gazebo_client",
              ros::init_options::NoSigintHandler);
        }

        this->rosNode.reset(new ros::NodeHandle("gazebo_client"));

        ros::SubscribeOptions soL =
          ros::SubscribeOptions::create<std_msgs::Float32>(
              "/" + this->model->GetName() + "/vel_left_cmd",
              1,
              boost::bind(&VelodynePlugin::OnRosMsgLeft, this, _1),
              ros::VoidPtr(), &this->rosQueue);
        this->rosSubL = this->rosNode->subscribe(soL);

         ros::SubscribeOptions soR =
          ros::SubscribeOptions::create<std_msgs::Float32>(
              "/" + this->model->GetName() + "/vel_right_cmd",
              1,
              boost::bind(&VelodynePlugin::OnRosMsgRight, this, _1),
              ros::VoidPtr(), &this->rosQueue);
        this->rosSubR = this->rosNode->subscribe(soR);
        this->rosQueueThread =
          std::thread(std::bind(&VelodynePlugin::QueueThread, this));
      }

    public: void SetVelocityRight(const double &_vel)
    {
      this->model->GetJointController()->SetVelocityTarget(
          this->right_wheel_joint->GetScopedName(), _vel);
    }

    public: void SetVelocityLeft(const double &_vel)
    {
      this->model->GetJointController()->SetVelocityTarget(
          this->left_wheel_joint->GetScopedName(), _vel);
    }

    public: void OnRosMsgLeft(const std_msgs::Float32ConstPtr &_msg)
    {
        this->SetVelocityLeft(_msg->data);
    }

    public: void OnRosMsgRight(const std_msgs::Float32ConstPtr &_msg)
    {
      this->SetVelocityRight(_msg->data);
    }
    

    private: void QueueThread()
    {
      static const double timeout = 0.01;
      while (this->rosNode->ok())
      {
        this->rosQueue.callAvailable(ros::WallDuration(timeout));
      }
    }
  };

  GZ_REGISTER_MODEL_PLUGIN(VelodynePlugin)
}
#endif

