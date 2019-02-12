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
    parser = argparse.ArgumentParser(description="Extract pose and plan from a ROS bag.")
    parser.add_argument("bag_file", help="Input ROS bag.")
    parser.add_argument("output_dir", help="Output directory.")
    parser.add_argument("pose_topic", help="pose topic.")
    parser.add_argument("plan_topic", help="Global plan topic.")    

    args = parser.parse_args()

    print "\nExtracting pose from %s on topic %s into %s/pose.txt" % (args.bag_file,
                                                          args.pose_topic, args.output_dir)
    print "\nExtracting plan from %s on topic %s into %s/plan.txt" % (args.bag_file,
                                                          args.plan_topic, args.output_dir)

    bag = rosbag.Bag(args.bag_file, "r")
    f_pose= open(args.output_dir+"pose.txt","w+")
    f_plan = open(args.output_dir+"plan.txt", "w+")

    count = 0

    print(bag)

    for topic, msg, t in bag.read_messages(topics=[args.pose_topic, args.plan_topic]):
        
        #print("entered for loop")

        # ODOM extraction

        if (topic==args.pose_topic):
            #print(msg.pose.pose)
            x, y, yaw = extract(msg.pose)
            #x, y, yaw = extract(msg.pose.pose)

            f_pose.write(str(t.secs) + "." + str(t.nsecs) + "\t" +
                str(x) + "\t" + str(y) + "\t" + str(yaw) + "\n")

        elif (topic==args.plan_topic):

            # PLAN extraction
            #print("extracting plan")

            if (count < len(msg.poses)):

                x, y, yaw = extract(msg.poses[count])

                f_plan.write(str(t.secs) + "." + str(t.nsecs) + "\t" +
                    str(x) + "\t" + str(y) + "\t" + str(yaw) + "\n")

            count += 1

    bag.close()
    f_pose.close() 
    f_plan.close()

    return

if __name__ == '__main__':
    main()

