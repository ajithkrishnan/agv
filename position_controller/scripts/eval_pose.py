from __future__ import division
import os
import numpy as np
import argparse
from glob import glob
from pose_evaluation_utils import *

parser = argparse.ArgumentParser()
parser.add_argument("--gtruth_dir", type=str, 
    help='Path to the directory with ground-truth trajectories')
parser.add_argument("--plan_dir", type=str, 
    help="Path to the directory with predicted trajectories")
args = parser.parse_args()

def main():
    ate_all = []

    # DEBUG: single file
    plan_file = os.path.join(args.plan_dir, 'plan.txt')
    gtruth_file = os.path.join(args.gtruth_dir,  'pose.txt')
    print("Plan File: {}".format(plan_file))
    print("Gtruth File: {}".format(gtruth_file))
    if not os.path.exists(gtruth_file):
        print("I am here!")
        return 
    ate = compute_ate(gtruth_file, plan_file)
    if ate == False:
        return
    ate_all.append(ate)

    ate_all = np.array(ate_all)
    print("ATE mean: %.4f, std: %.4f" % (np.mean(ate_all), np.std(ate_all)))

main()
