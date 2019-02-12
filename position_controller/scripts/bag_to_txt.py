#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import argparse

import rosbag
from sensor_msgs.msg import Image
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry, Path
from tf.transformations import euler_from_quaternion


def extract(msg):

        x = msg.pose.position.x
        y = msg.pose.position.y
        orientation_quat = msg.pose.orientation
        orientation_list = [orientation_quat.x, orientation_quat.y, 
                orientation_quat.z, orientation_quat.w]
        (_,_,yaw) = euler_from_quaternion(orientation_list)
        
        return x, y, yaw

def main():
    """Extract Odometry Info from a rosbag.
    """
    parser = argparse.ArgumentParser(description="Extract odom from a ROS bag.")
    parser.add_argument("bag_file", help="Input ROS bag.")
    parser.add_argument("output_dir", help="Output directory.")
    parser.add_argument("odom_topic", help="odom topic.")
    parser.add_argument("plan_topic", help="Global plan topic.")    

    args = parser.parse_args()

    print "\nExtracting plan from %s on topic %s into %s" % (args.bag_file,
                                                          args.plan_topic, args.output_dir)
    print "\nExtracting odom from %s on topic %s into %s" % (args.bag_file,
                                                          args.odom_topic, args.output_dir)

    bag = rosbag.Bag(args.bag_file, "r")
    f_odom= open(args.output_dir+"odom.txt","w+")
    f_plan = open(args.output_dir+"plan.txt", "w+")

    count = 0

    for topic, msg, t in bag.read_messages(topics=[args.odom_topic, args.plan_topic]):
        
        # ODOM extraction

        if (topic==args.odom_topic):
            x, y, yaw = extract(msg.pose)

            f_odom.write(str(t.secs) + "." + str(t.nsecs) + "\t" +
                str(x) + "\t" + str(y) + "\t" + str(yaw) + "\n")

        elif (topic==args.plan_topic):

            # PLAN extraction

            if (count < len(msg.poses)):

                x, y, yaw = extract(msg.poses[count])

                f_plan.write(str(t.secs) + "." + str(t.nsecs) + "\t" +
                    str(x) + "\t" + str(y) + "\t" + str(yaw) + "\n")

            count += 1

    bag.close()
    f_odom.close() 
    f_plan.close()

    return

if __name__ == '__main__':
    main()

