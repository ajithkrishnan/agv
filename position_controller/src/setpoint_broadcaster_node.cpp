#include "../include/position_controller/setpoint_broadcaster.h"
#include "setpoint_broadcaster.cpp"


int main(int argc, char **argv)
    {
        try
        {
            ros::init(argc,argv,"setpointBroadcaster");
            ros::start();

            broadcast::SetpointBroadcaster sp;
            ros::Rate rate(50);

            while(ros::ok())
            {
                sp.broadcastSetpoint();
                rate.sleep();
            }
        }
        catch (ros::Exception)
        {
            return 0;
        }
        //ros::spin();

        ros::shutdown();

        return 0;
    }
