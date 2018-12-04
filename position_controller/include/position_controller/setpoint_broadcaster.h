#ifndef SETPOINT_BROADCASTER_H
#define SETPOINT_BROADCASTER_H

#include <cmath>
#include <sys/time.h>
#include <string.h>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_broadcaster.h>

namespace broadcast
{
    class SetpointBroadcaster
    {
        public:
            ros::NodeHandle np;
            tf2_ros::TransformBroadcaster br;
            ros::Subscriber sub_sp = np.subscribe("/agv_mecanum/sp_pose", 1, &SetpointBroadcaster::callbackSetpoint, this);

            geometry_msgs::PoseStamped setpoint;
            geometry_msgs::TransformStamped tr;
                        
            //rospy.on_shutdown
            
            void callbackSetpoint(const geometry_msgs::PoseStamped::ConstPtr& msg);
            void broadcastSetpoint();

            //ROS_INFO("Start Setpoint Broadcaster");
    };
};
#endif
