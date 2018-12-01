//#include <iostream>
#include "../include/position_controller/position_controller.h"


namespace pose_con
{
    
void PositionController::cleanup(int sig)
        {
            //ROS_INFO("Stop Position Controller");
            ros::shutdown();
        }
        
bool PositionController::lookupTransform(const std::string &parent, const std::string &child, geometry_msgs::TransformStamped &trans)
        {
            try
            {
                tf2_ros::Buffer buffer(ros::Duration(10.0));
                //tf2_ros::TransformListener tlistener(buffer);
                trans = buffer.lookupTransform(parent, child, ros::Time());
                return true;
            }
            catch (tf2::LookupException& ex)
            {    
              ROS_ERROR_THROTTLE(1.0, "No Transform available Error looking up robot pose: %s\n", ex.what());
              return false;
            }    
            catch (tf2::ConnectivityException& ex)                                                                             
            {    
              ROS_ERROR_THROTTLE(1.0, "Connectivity Error looking up robot pose: %s\n", ex.what());
              return false;
            }    
            catch (tf2::ExtrapolationException& ex)
            {    
              ROS_ERROR_THROTTLE(1.0, "Extrapolation Error looking up robot pose: %s\n", ex.what());
              return false;
            } 
        } 
        
bool PositionController::atSetpointPos()
        {
            double deadband = 0.01;
            bool disp_flag = false;
            geometry_msgs::TransformStamped disp;
            disp_flag = lookupTransform("agv_base_footprint", "setpoint_pose", disp);
            if (disp_flag)
            {
                double displacement;
                displacement = sqrt(std::pow(disp.transform.translation.x,2) + std::pow(disp.transform.translation.y,2));
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

bool PositionController::atSetpointYaw()
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
                tf2::Matrix3x3(quat).getEulerYPR(yaw, pitch, roll);
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
        
void PositionController::control()
        {
            if(pid_enabled == true)
            {
                bool setpoint_flag = false;
                bool trans_flag = false;
                geometry_msgs::TransformStamped setpoint, trans;

                setpoint_flag = this->lookupTransform("agv_base_footprint", "setpoint_pose", setpoint);
                trans_flag = this->lookupTransform("setpoint_pose", "agv_base_footprint", trans);

                if (setpoint_flag && trans_flag)
                {
                    // Debug
                    pos_x.data = 0;
                    pub_pid_x_setpoint.publish(pos_x);
                    pos_y.data = 0;
                    pub_pid_y_setpoint.publish(pos_y);
                    pos_yaw.data = 0;
                    pub_pid_yaw_setpoint.publish(pos_yaw);
                    
                    pos_x.data = trans.transform.translation.x;
                    pos_y.data = trans.transform.translation.y;
                    tf2::Quaternion quat(trans.transform.rotation.x, trans.transform.rotation.y,
                                         trans.transform.rotation.z, trans.transform.rotation.w);
                    double yaw, pitch, roll;
                    tf2::Matrix3x3(quat).getEulerYPR(yaw, pitch, roll);
                    
                    pos_yaw.data = yaw;
                    
                    pub_pid_x_state.publish(pos_x);
                    pub_pid_y_state.publish(pos_y);
                    pub_pid_yaw_state.publish(pos_yaw);

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
        
void PositionController::callbackX(const std_msgs::Float64::ConstPtr& msg)
        {
            control_effort_x.data = msg->data;
        }

void PositionController::callbackY(const std_msgs::Float64::ConstPtr& msg)
        {
            control_effort_y.data = msg->data;
        }

void PositionController::callbackYaw(const std_msgs::Float64::ConstPtr& msg)
        {
            control_effort_yaw.data = msg->data;
        }
};
