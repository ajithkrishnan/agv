#include <iostream>

#include "positionController.h"

class positionController()
{
    public:
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

        tf2_ros::Buffer tBuffer;
        tf2_ros::TransformListener tlistener(tBuffer);

        bool pid_enabled = true;

        sensor_msgs::Twist cmd;
        std_msgs::Float64 pos_x = 0.0, pos_y = 0.0, yaw = 0.0, sp_pos_x = 0.0, sp_pos_y = 0.0, sp_yaw = 0.0, control_effort_x = 0.0, control_effort_y = 0.0, control_effort_yaw = 0.0;

        void control();

        bool atSetpointPos();
        bool atSetpointYaw();
        void callback_x();
        void callback_y();
        void callback_yaw();
        
        ROS_INFO("Start Position Controller");

}

void control()
{
    if (pid_enabled == true)
    {
        bool setpoint = false;
        bool trans = false;

        setpoint = lookupTransform("agv_base_footprint", "setpoint_pose");
        trans = lookupTransform("setpoint_pose", "agv_base_footprint");

        if (setpoint && trans)
        {
            
            pub_pid_x_setpoint.publish(0);
            pub_pid_y_setpoint.publish(0);
            pub_pid_yaw_setpoint.publish(0);
            
            pos_x = trans.transform.translation.x;
            pos_y = trans.transform.translation.y;
            // quat
            // euler
            yaw = euler[2];

            pub_pid_x_state.publish(pos_x);
            pub_pid_y_state.publish(pos_y);
            pub_pid_yaw_state.publish(yaw);

            if(!atSetpointYaw())
            {
                cmd.linear.x = 0;
                cmd.linear.y = 0;
                cmd.angular.z = control_effort_yaw.data;
                pub_cmd.publish(cmd);
            }
            else if(!atSetpointPos())
            {
                cmd.linear.x = control_effort_x.data;
                cmd.linear.y = control_effort_y.data;
                cmd.angular.z = control_effort_yaw.data;
                pub_cmd.publish(cmd);
            }
            else
            {
                cmd.linear.x = 0;
                cmd.linear.y = 0;
                cmd.angular.z = 0;
                pub_cmd.publish(cmd);
            }
            
        }
    } 
}

//bool lookupTransform() 

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
    try
    {
        ros::init(argc, argv, "positionController");
        ros::start();

        ros::NodeHandle nh;

        PositionController pc();
        ros::Rate rate(10);

        while (ros::ok())
        {
            pc.control();
            rate.sleep();
        }
    }

    catch(ros::Exception)
    {
        return 0;
    }

    ros::spin();

    ros::shutdown();

    return 0;    
}