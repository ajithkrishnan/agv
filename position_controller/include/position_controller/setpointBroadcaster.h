#ifndef SETPOINT_BROADCASTER_H
#define SETPOINT_BROADCASTER_H

#include <cmath>
#include <sys/time.h>
#include <string>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

class SetpointBroadcaster()
{
    public:
        tf2_ros::TransformBroadcaster br;
        ros::Subscriber sub_sp = np.subscribe("/agv_mecanum/sp_pose", 1, callbackSetpoint);

        geometry_msgs::PoseStamped setpoint;
        geometry_msgs::TransformStamped t;
        t.header.frame_id = "odom";
        t.child_frame_id = "setpoint_pose";
        t.transform.translation.x = 0;
        t.transform.translation.y = 0;
        t.transform.translation.z = 0;
        t.transform.rotation.x = 0;
        t.transform.rotation.y = 0;
        t.transform.rotation.z = 0;
        t.transform.rotation.w = 0;

        //rospy.on_shutdown

        void callbackSetpoint();
        void broadcastSetpoint();

        ROS_INFO("Start Setpoint Broadcaster");
}

#endif