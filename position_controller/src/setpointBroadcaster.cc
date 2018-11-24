#include <iostream>
#include "setpointBroadcaster.h"

//void callbackSetpoint(std_msgs::Float64::ConstPtr &msg)
{
    t.transform.translation.x = msg.pose.position.x;
    t.transform.translation.y = msg.pose.position.y;
    t.transform.translation.z = msg.pose.position.z;
    t.transform.rotation.x = msg.pose.orientation.x;
    t.transform.rotation.y = msg.pose.orientation.y;
    t.transform.rotation.z = msg.pose.orientation.z;
    t.transform.rotation.w = msg.pose.orientation.w;
}

void broadcastSetpoint()
{
    t.header.stamp = ros::Time::now();
    br.sendTransform(t);
}

int main(int argc, char **argv)
{
    try
    {
        ros::init(argc,argv,"setpointBroadcaster");
        ros::start();

        ros::NodeHandle nh;

        SetpointBroadcaster sp;
        ros::Rate rate(20);

        while(ros::ok())
        {
            sp.broadcastSetpoint();
            rate.sleep();
        }
    }
    catch (ros::Exception)
        return 0;
    
    ros::spin();

    ros::shutdown();

    return 0;
}
