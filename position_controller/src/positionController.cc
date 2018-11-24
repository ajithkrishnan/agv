#include <iostream>
#include "positionController.h"

void control()
{
    if (pid_enabled == true)
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
        trans = tfBuffer.lookupTransform(parent, child, ros::Time());
        return true;
    }
    except (tf2::ConnectivityException, tf2::LookupException, tf2::ExtrapolationException)
        return false;
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
            return true;
        else
            return false; 
    }
    else
        return false;
}

bool atSetpointYaw()
{
    double deadband = 0.17453;
    bool disp_flag = false;
    geometry_msgs::TransformStamped disp;
    disp_flag = lookupTransform("agv_base_footprint", "setpoint_pose", disp); 
    if (disp_flag)
    {
        f2::Quaternion quat(disp.transform.rotation.x, disp.transform.rotation.y, 
                    disp.transform.rotation.z, disp.transform.rotation.w)
        double yaw, pitch, roll;
        tf2::Matri3x3(quat).getEulerYPR(yaw, pitch, roll);
        if (yaw <= deadband)
            return true;
        else
            return false;
    }
    else
        return false;
}

void callbackX(std_msgs::Float64::ConstPtr& msg)
{
    //control_effort_x = msg;
}

void callbackY(std_msgs::Float64::ConstPtr& msg)
{
    //control_effort_x = msg;
}

void callbackYaw(std_msgs::Float64::ConstPtr& msg)
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
        ros::Rate rate(50);

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