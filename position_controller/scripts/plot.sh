#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: ./bag_to_txt.sh <path-to-bagfile> "
    break

else
    OUTPUT_DIR=${PWD}
    BAGFILE=$1
    GT_TOPIC="/agv_mecanum/ground_truth/state"
    POSE_TOPIC="/amcl_pose"
    PLAN_TOPIC="/move_base/TebLocalPlannerROS/global_plan"

    #python bag_to_txt.py $BAGFILE $OUTPUT_DIR/ $POSE_TOPIC $PLAN_TOPIC
    python bag_to_txt.py \
        --bagfile $BAGFILE \
        --output_dir $OUTPUT_DIR/ \
        --pose_topic $GT_TOPIC \
        --plan_topic $PLAN_TOPIC

    #python plot_plotly.py pose.txt plan.txt 

fi
