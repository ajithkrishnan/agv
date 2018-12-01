# catkin requires minimum of 2.8.3 or higher
cmake_minimum_required(VERSION 2.8.3)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

# this is where the name of the catkin package is specified.
# This name can be later referenced using ${PROJECT_NAME}
project(position_controller)

# Used to find other packages(dependencies) required to build the project.
# once a package is found, environment variables specifying the path of its
# exported header files, library dependencies etc. are set which can be used later.
# It is easier to specify catkin dependencies as COMPONENTS of catkin 
# rather than specifying a find_package for each of them. Doing so will create
# a single set of environment variables for all these "components" with a "catkin_"
# prefix. eg. catkin_INCLUDE_DIRS, catkin_LIBRARIES.
# This feature of CMake was utilized in the design of catkin to save typing time. 
find_package(catkin REQUIRED 
    COMPONENTS
        roscpp
        rospy
        std_msgs
        geometry_msgs
        tf
        tf2
        tf2_ros
)

# catkin specific macro for CMake 
# This function must be called before specifying any build targets 
# eg. add_library() or add_executable()
catkin_package(CATKIN_DEPENDS 
               geometry_msgs
               std_msgs
               roscpp)

# Two types of build targets: executables and libraries (used by executables
# at build/run time)
# beore generating the targets, the paths to their include files/libraries
# must be specified

# to include the arguments as part of the directory path
include_directories(include ${catkin_INCLUDE_DIRS})

# there is no need to use this function 
# the information is pulled by find_package()
#link_directories()

# specifiying the executable target name and the source file it is 
# generated from
add_executable(position_controller_node src/position_controller_node.cpp)

# specifying the libraries the executable depends on
# add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

# This is used to specify which libraries the executable links against
# catkin_LIBRARIES is used if "ros is not found"
target_link_libraries(position_controller_node ${catkin_LIBRARIES})

