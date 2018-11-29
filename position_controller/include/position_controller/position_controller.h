#ifndef POSITION_CONTROLLER_H
#define POSITION_CONTROLLER_H

#include <cmath>
#include <sys/time.h>
#include <string>
#include <signal.h>
#include <stdbool.h>
#include <ros/duration.h>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/buffer.h>
//#include <tf2_ros/transform_listener.h>
#include <tf2/utils.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2/convert.h>


namespace pose_con
{
class PositionController
{
    public:

        //ros::Duration buffer_time = 10.0;

        ros::NodeHandle nh;

        ros::Publisher activator_x = nh.advertise<std_msgs::Bool>("/agv_mechanum/pid_x/pid_enable", 1);
        ros::Publisher activator_y = nh.advertise<std_msgs::Bool>("/agv_mechanum/pid_y/pid_enable", 1);
        ros::Publisher activator_yaw = nh.advertise<std_msgs::Bool>("/agv_mechanum/pid_yaw/pid_enable", 1);

        ros::Publisher pub_pid_x_setpoint = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_x/setpoint", 1);
        ros::Publisher pub_pid_y_setpoint = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_y/setpoint", 1);
        ros::Publisher pub_pid_yaw_setpoint = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_yaw/setpoint", 1);
        ros::Publisher pub_pid_x_state = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_x/state", 1);
        ros::Publisher pub_pid_y_state = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_y/state", 1);
        ros::Publisher pub_pid_yaw_state = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_yaw/state", 1);
        ros::Subscriber sub_pid_x_effort = nh.subscribe("/agv_mechanum/pid_x/control_effort", 1, &PositionController::callbackX, this);
        ros::Subscriber sub_pid_y_effort = nh.subscribe("/agv_mechanum/pid_y/control_effort", 1, &PositionController::callbackY, this);
        ros::Subscriber sub_pid_yaw_effort = nh.subscribe("/agv_mechanum/pid_yaw/control_effort", 1, &PositionController::callbackYaw, this);

        ros::Publisher pub_cmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
        // DEBUG
        //tf2_ros::Buffer buffer(ros::Duration(10));
        //tf2_ros::Buffer buffer;
        //tf2_ros::TransformListener tf(buffer);
        //tf2_ros::TransformListener tlistener(buffer);

        bool pid_enabled =true;
        
        geometry_msgs::Twist cmd;
        // std_msgs::Float64 pos_x = 0.0, pos_y = 0.0, yaw = 0.0, sp_pos_x = 0.0, sp_pos_y = 0.0, sp_yaw = 0.0, control_effort_x = 0.0, control_effort_y = 0.0, control_effort_yaw = 0.0;
        std_msgs::Float64 pos_x, pos_y, pos_yaw, sp_pos_x, sp_pos_y, sp_yaw, control_effort_x, control_effort_y, control_effort_yaw;
        
        void cleanup(int sig);
        bool lookupTransform(const std::string &parent, const std::string &child, geometry_msgs::TransformStamped &trans);
        //void handle_service()
        bool atSetpointPos();
        bool atSetpointYaw();
        void control();
        
        void callbackX(const std_msgs::Float64::ConstPtr& msg);
        void callbackY(const std_msgs::Float64::ConstPtr& msg);
        void callbackYaw(const std_msgs::Float64::ConstPtr& msg);

        //ROS_INFO("Start Position Controller");

};
};

#endif


