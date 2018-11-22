#include <iostream>

#include "positionController.h"

void control()
{
    if (pid_enabled == true)
    {
        bool setpoint = false;
        bool trans = false;
    } 
}

// bool lookupTransform() 

bool atSetpointPos()
{
    float deadband = 0.01;
    bool disp = false;
    disp = lookupTransform("agv_base_footprint", "setpoint_pose");
    if (disp)
    {
        float displacement;
        //displacement =
        if (displacement <= deadband)
            return true;
        else
            return false; 
    }
    else
        return false;
}

bool atSetpointYaw()
{
    float deadband = 0.17453;
    bool disp = false;
    disp = lookupTransform("agv_base_footprint", "setpoint_pose"); 
    if (disp)
    {
        //quat
        //euler
        std_msgs::Float64 yaw = euler[2];
        if (yaw <= deadband)
            return true;
        else
            return false;
    }
    else
        return false;
}

void callback_x(std_msgs::Float64::ConstPtr& msg)
{
    //control_effort_x = msg;
}

void callback_y(std_msgs::Float64::ConstPtr& msg)
{
    //control_effort_x = msg;
}

void callback_yaw(std_msgs::Float64::ConstPtr& msg)
{
    //control_effort_x = msg;
}

int main(int argc, char **argv)
{
    ros::Publisher activator_x = nh.advertise<std_msgs::Bool>("/agv_mechanum/pid_x/pid_enable", 1);
    ros::Publisher activator_y = nh.advertise<std_msgs::Bool>("/agv_mechanum/pid_y/pid_enable", 1);
    ros::Publisher activator_yaw = nh.advertise<std_msgs::Bool>("/agv_mechanum/pid_yaw/pid_enable", 1);

    ros::Publisher pub_pid_x_setpoint = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_x/setpoint", 1);
    ros::Publisher pub_pid_y_setpoint = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_y/setpoint", 1);
    ros::Publisher pub_pid_yaw_setpoint = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_yaw/setpoint", 1);
    ros::Publisher pub_pid_x_state = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_x/state", 1);
    ros::Publisher pub_pid_y_state = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_y/state", 1);
    ros::Publisher pub_pid_yaw_state = nh.advertise<std_msgs::Float64>("/agv_mechanum/pid_yaw/state", 1);
    ros::Subscriber sub_pid_x_effort = nh.subscribe("/agv_mechanum/pid_x/control_effort", 1, callback_x);
    ros::Subscriber sub_pid_y_effort = nh.subscribe("/agv_mechanum/pid_y/control_effort", 1, callback_y);
    ros::Subscriber sub_pid_yaw_effort = nh.subscribe("/agv_mechanum/pid_yaw/control_effort", 1, callback_yaw);

    ros::Publisher pub_cmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    try
    {
        ros::init(argc, argv, "positionController");
        ros::start();

        ros::NodeHandle nh;

        
    }

    //catch()
    {
        return 0;
    }

    ros::spin();

    ros::shutdown();

    return 0;    

}