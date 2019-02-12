#!/usr/bin/python
# Software License Agreement (BSD License)
#
# Copyright (c) 2013, Juergen Sturm, TUM
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of TUM nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Requirements: 
# sudo apt-get install python-argparse


import argparse
import sys
import os
import numpy as np
from plotly import tools
import plotly.plotly as py
import plotly.graph_objs as go

def read_file_list(filename):
    """
    Reads a Pose from a text file. 
    
    File format:
    The file format is "stamp x y yaw  where stamp denotes the time stamp 
    and x, y and yaw denote the Pose 
    
    """
    file = open(filename)
    data = file.read()
    lines = data.replace("\t"," ").split("\n") 
    list = [[v.strip() for v in line.split(" ") if v.strip()!=""] for line in lines if len(line)>0 and line[0]!="#"]

    #list = [(float(l[0]),l[1:]) for l in list if len(l)>1]
    #list = [(float(l[i]) for i in l ) for l in list if len(l)>1]
    list = [[float(l[0]),float(l[1]), float(l[2]), float(l[3])] for l in list if len(l)>1]
    return list

def plot(pose_list):
    pose_list_x = []
    pose_list_y = []
    pose_yaw_list = []
    time_list = []

    for pose in pose_list:
        time = pose[0]
        pose_x = pose[1]
        pose_y = pose[1]
        yaw = pose[2]

        pose_list_x.append(pose_x)
        pose_list_y.append(pose_y)
        pose_yaw_list.append(yaw)
        time_list.append(time)

    trace_x = go.Scatter(x=time_list, y=pose_list_x, name='x')
    trace_y = go.Scatter(x=time_list, y=pose_list_y, name='y')
    trace_yaw = go.Scatter(x=time_list, y=pose_yaw_list, name='yaw')

    # DEPRECATED
    #data = [trace_x, trace_y, trace_yaw]
    #fig = tools.make_subplots(rows=3, cols=1, specs=[[{}], [{}], [{}]], shared_xaxes=True, shared_yaxes=True)

    #fig.append_trace(trace_x, 3, 1)
    #fig.append_trace(trace_y, 2, 1)
    #fig.append_trace(trace_yaw, 1, 1)

    #fig['layout'].update(height=600, width=600, title='AGV odometry')
    #py.plot(fig, filename='odom_plotly')
    
    return trace_x, trace_y, trace_yaw 

if __name__ == '__main__':
    
    # parse command line
    parser = argparse.ArgumentParser(description='''
    This script extracts a trajectory from a txt file and plots it    
    ''')
    parser.add_argument('odom_file', help='odom text file (format: AMCL Odometry data)')
    parser.add_argument('plan_file', help='plan text file (format: move_base plan data)')

    args = parser.parse_args()

    tools.set_credentials_file(username='ajithkrishnanbm', api_key='MCCZv2hEgYCcbPL9gQ92')

    odom_list = read_file_list(args.odom_file)
    plan_list = read_file_list(args.plan_file)

    traces_x = []
    traces_y = []
    traces_yaw = []
    trace_x, trace_y, trace_yaw = plot(odom_list)
    traces_x.append(trace_x)
    traces_y.append(trace_y)
    traces_yaw.append(trace_yaw)

    trace_x, trace_y, trace_yaw = plot(plan_list)
    traces_x.append(trace_x)
    traces_y.append(trace_y)
    traces_yaw.append(trace_yaw)
        
    layout_x = go.Layout(
            title='AGV odometry',
            xaxis=dict(title='Timestamp'),
            yaxis=dict(title='Odometry X'),
            legend=dict(traceorder='reversed'),
            )

    layout_y = go.Layout(
            title='AGV odometry',
            xaxis=dict(title='Timestamp'),
            yaxis=dict(title='Odometry Y'),
            legend=dict(traceorder='reversed'),
            )
   
    layout_yaw = go.Layout(
            title='AGV odometry',
            xaxis=dict(title='Timestamp'),
            yaxis=dict(title='Odometry Yaw'),
            legend=dict(traceorder='reversed'),
            )

    fig_x = go.Figure(data=traces_x, layout=layout_x)
    fig_y = go.Figure(data=traces_y, layout=layout_y)
    fig_yaw = go.Figure(data=traces_yaw, layout=layout_yaw)

    py.plot(fig_x, filename='AGV Odomentry - stacked x')
    py.plot(fig_y, filename='AGV Odomentry - stacked y')
    py.plot(fig_yaw, filename='AGV Odomentry - stacked yaw')



    
    


