//#include <iostream>
#include "position_controller/position_controller.h"


void cleanup(int sig)
{
    //ROS_INFO("Stop Position Controller");
    ros::shutdown();
}

//void handle_service()

void control(bool pid_enabled)
{
    if(pid_enabled == true)
    {
        bool setpoint_flag = false;
        bool trans_flag = false;
        geometry_msgs::TransformStamped setpoint, trans;

        setpoint_flag = lookupTransform("agv_base_footprint", "setpoint_pose", setpoint);
        trans_flag = lookupTransform("setpoint_pose", "agv_base_footprint", trans);

        if (setpoint_flag && trans_flag)
        {
            
            pub_pid_x_setpoint.publish(0);
            pub_pid_y_setpoint.publish(0);
            pub_pid_yaw_setpoint.publish(0);
            
            pos_x = trans.transform.translation.x;
            pos_y = trans.transform.translation.y;
            tf2::Quaternion quat(trans.transform.rotation.x, trans.transform.rotation.y,
                                 trans.transform.rotation.z, trans.transform.rotation.w);
            double yaw, pitch, roll;
            tf2::Matri3x3(quat).getEulerYPR(yaw, pitch, roll);
            
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

bool lookupTransform(const std::string &parent, const std::string &child, geometry_msgs::TransformStamped &trans)
{
    try
    {
        trans = buffer.lookupTransform(parent, child, ros::Time());
        return true;
    }
    except (tf2::ConnectivityException, tf2::LookupException, tf2::ExtrapolationException)
    {
        return false;
    }
} 

bool atSetpointPos()
{
    double deadband = 0.01;
    bool disp_flag = false;
    geometry_msgs::TransformStamped disp;
    disp_flag = lookupTransform("agv_base_footprint", "setpoint_pose", disp);
    if (disp_flag)
    {
        double displacement;
        displacement = sqrt(disp.transform.translation.x**2 + disp.transform.translation.y**2);
        if (displacement <= deadband)
        {
            return true;
        }
        else
        { 
            return false; 
        }
    }
    else
    {
        return false;
    }
}

bool atSetpointYaw()
{
    double deadband = 0.17453;
    bool disp_flag = false;
    geometry_msgs::TransformStamped disp;
    disp_flag = lookupTransform("agv_base_footprint", "setpoint_pose", disp); 
    if (disp_flag)
    {
        tf2::Quaternion quat(disp.transform.rotation.x, disp.transform.rotation.y, 
                    disp.transform.rotation.z, disp.transform.rotation.w);
        double yaw, pitch, roll;
        tf2::Matri3x3(quat).getEulerYPR(yaw, pitch, roll);
        if (yaw <= deadband)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//void callbackX(std_msgs::Float64::ConstPtr& msg)
void callbackX(std_msgs::Float64& msg){
    control_effort_x = msg.data;
}

void callbackY(std_msgs::Float64& msg)
{
    control_effort_y = msg.data;
}

void callbackYaw(std_msgs::Float64& msg)
{
    control_effort_yaw = msg.data;
}

int main(int argc, char **argv)
{
    try
    {
        ros::init(argc, argv, "PositionController", ros::init_options::NoSigintHandler);
        ros::start();

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
        ros::Subscriber sub_pid_x_effort = nh.subscribe("/agv_mechanum/pid_x/control_effort", 1, callbackX);
        ros::Subscriber sub_pid_y_effort = nh.subscribe("/agv_mechanum/pid_y/control_effort", 1, callbackY);
        ros::Subscriber sub_pid_yaw_effort = nh.subscribe("/agv_mechanum/pid_yaw/control_effort", 1, callbackYaw);

        ros::Publisher pub_cmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

        PositionController pc;
        ros::Rate rate(50);

        while (ros::ok())
        {
            pc.control(true);
            signal(SIGINT, cleanup);
            rate.sleep();
        }
    }

    catch(ros::Exception)
    {
        return(0);
    }

    
    //ros::shutdown();

    return(0);    
}
