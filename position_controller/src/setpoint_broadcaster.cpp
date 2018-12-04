#include <iostream>
#include "../include/position_controller/position_controller.h"

namespace pose_con
{
    void SetpointBroadcaster::callbackSetpoint(const geometry_msgs::PoseStamped::ConstPtr& msg)
    {
        tr.header.frame_id = "odom";
        tr.child_frame_id = "setpoint_pose";
        tr.transform.translation.x = msg->pose.position.x;
        tr.transform.translation.y = msg->pose.position.y;
        tr.transform.translation.z = msg->pose.position.z;
        tr.transform.rotation.x = msg->pose.orientation.x;
        tr.transform.rotation.y = msg->pose.orientation.y;
        tr.transform.rotation.z = msg->pose.orientation.z;
        tr.transform.rotation.w = msg->pose.orientation.w;
    }

    void SetpointBroadcaster::broadcastSetpoint()
    {
        tr.header.stamp = ros::Time(0);
        br.sendTransform(tr);
    }

    
};
