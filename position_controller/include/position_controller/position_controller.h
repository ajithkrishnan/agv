#ifndef POSITION_CONTROLLER_H_
#define POSITION_CONTROLLER_H_

#include <cmath>
#include <sys/time.h>
#include <string>
#include <signal.h>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "tf2/utils.h"
#include "tf2/LinearMath/Transform.h"
#include "tf2/convert.h"
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

class PositionController
{
    public:

        tf2_ros::Buffer tfBuffer(ros::Duration(10));
        tf2_ros::TransformListener tlistener(tfBuffer);
        
        bool pid_enabled;
        pid_enabled = true;

        geometry_msgs::Twist cmd;
        // std_msgs::Float64 pos_x = 0.0, pos_y = 0.0, yaw = 0.0, sp_pos_x = 0.0, sp_pos_y = 0.0, sp_yaw = 0.0, control_effort_x = 0.0, control_effort_y = 0.0, control_effort_yaw = 0.0;
        std_msgs::Float64 pos_x, pos_y, yaw, sp_pos_x, sp_pos_y, sp_yaw, control_effort_x, control_effort_y, control_effort_yaw;

        void control();
        bool lookupTransform(const std::string &parent, const std::string &child, geometry_msgs::TransformStamped &trans);
        void cleanup(int sig);

        bool atSetpointPos();
        bool atSetpointYaw();
        void callbackX(std_msgs::Float64::ConstPtr& msg);
        void callbackY(std_msgs::Float64::ConstPtr& msg);
        void callbackYaw(std_msgs::Float64::ConstPtr& msg);
        
        ROS_INFO("Start Position Controller");

};

#endif


