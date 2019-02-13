#!/usr/bin/env python


from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import plotly.graph_objs as go
import plotly.tools as tls
import time
import plotly
import glob


plotly.offline.init_notebook_mode(connected=True)  


def plot(data_plan, data_pose):
    
    trace_plan = go.Scatter(x = data_plan[1],
                          y = data_plan[2], 
                          mode = 'lines+markers', 
                          name= 'Plan',
                          line=dict(width= 2.5),
                          marker=dict(size=5.5))

    trace_pose = go.Scatter(x = data_pose[1],
                        y = data_pose[2], 
                        mode = 'lines+markers', 
                        name= 'Pose',
                        line=dict(width= 1.5),
                        opacity=0.7, marker=dict(
                              size=3.5,
                              color='Orange'))
        
    data = [trace_plan, trace_pose]

if __name__ == '__main__':
    
    # parse command line
    parser = argparse.ArgumentParser(description='''
    This script extracts a trajectory from a txt file and plots it    
    ''')
    parser.add_argument('pose_file', help='pose text file (format: AMCL Odometry data)')
    parser.add_argument('plan_file', help='plan text file (format: move_base plan data)')

    args = parser.parse_args()

    tools.set_credentials_file(username='ajithkrishnanbm', api_key='MCCZv2hEgYCcbPL9gQ92')

    #data_plan=pd.read_csv(r'C:\Users\useradmin\Documents\AGV\plan.txt',sep= '\t',header=None)
    #data_pose=pd.read_csv(r'C:\Users\useradmin\Documents\AGV\pose.txt',sep= '\t',header=None)
    data_plan=pd.read_csv(args.plan_file,sep= '\t',header=None)
    data_pose=pd.read_csv(args.pose_file,sep= '\t',header=None)

    plot(data_plan, data_pose)

    layout= go.Layout(
        title= 'AGV Path Vs Pose',
        hovermode= 'closest',
        xaxis= dict(
            title= 'X Coordinate',
            ticklen= 5,
            zeroline= False,
            gridwidth= 2),
        yaxis=dict(
            title= 'Y Coordinate',
            ticklen= 5,
            zeroline= True,
            gridwidth= 2),
            showlegend= True)

    fig= go.Figure(data=data, layout=layout)

    # Plot in a different tab!
    plotly.offline.plot(fig, filename='Agv_pathVspose.html')      

    #plotly.offline.iplot(fig, filename='Agv_pathVspose.html')   #To plot within the noteook
                                                                
