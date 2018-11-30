#include "position_controller/position_controller.h"
#include <std_msgs/Float64.h>

//using namespace pose_con;


int main(int argc, char **argv)
{

    try
    {

        //ros::init(argc, argv, "PositionController", ros::init_options::NoSigintHandler);
        ros::init(argc, argv, "PositionController");
        ros::start();

        pose_con::PositionController pc;
       
        ros::Rate rate(50);

        while (ros::ok())
        {
            pc.control();
            //signal(SIGINT, pc.cleanup;
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
