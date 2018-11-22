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




