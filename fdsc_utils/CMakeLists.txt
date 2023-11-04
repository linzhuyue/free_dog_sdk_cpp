cmake_minimum_required(VERSION 3.0.2)
project(fdsc_utils)

set(CMAKE_CXX_FLAGS "-std=c++2a")
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -fPIC")

set(CATKIN_PACKAGE_DEPENDENCIES
  roscpp
  rospy
  std_msgs
)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(catkin REQUIRED COMPONENTS
  ${CATKIN_PACKAGE_DEPENDENCIES}
)
###################################
## catkin specific configuration ##
###################################

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    ${PROJECT_NAME}
  CATKIN_DEPENDS
    ${CATKIN_PACKAGE_DEPENDENCIES}
  DEPENDS
# 
  CFG_EXTRAS
    # 
)
###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)

set(FDSCSOURCE
 src/common.cpp
 src/unitreeConnectBoost.cpp
 src/lowState.cpp
 src/lowCmd.cpp
 src/highCmd.cpp
 src/highState.cpp
 )

 add_library(${PROJECT_NAME}
 ${FDSCSOURCE}
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
target_compile_options(${PROJECT_NAME} PUBLIC -W -Wall -Wextra -pedantic -pedantic-errors)

##############
## Highlevel ##
##############
add_executable(${PROJECT_NAME}_pushup_node
        ${FDSCSOURCE}
        src/examples/exp_pushups_highlevel.cpp
        
        )
target_link_libraries(${PROJECT_NAME}_pushup_node
  ${catkin_LIBRARIES}
)
add_executable(${PROJECT_NAME}_rotate90_node
      ${FDSCSOURCE}
      src/examples/exp_rotate_90_highlevel.cpp
      
        )
target_link_libraries(${PROJECT_NAME}_rotate90_node
  ${catkin_LIBRARIES}
)
add_executable(${PROJECT_NAME}_walk_node
      ${FDSCSOURCE}
      src/examples/exp_walk_highlevel.cpp
      
        )
target_link_libraries(${PROJECT_NAME}_walk_node
  ${catkin_LIBRARIES}
)
##############
## Lowlevel ##
##############
add_executable(${PROJECT_NAME}_pd_node
        ${FDSCSOURCE}
        src/examples/exp_joint_position_control_lowlevel.cpp
        
        )
target_link_libraries(${PROJECT_NAME}_pd_node
  ${catkin_LIBRARIES}
)
add_executable(${PROJECT_NAME}_vel_node
        ${FDSCSOURCE}
        src/examples/exp_joint_velocity_control_lowlevel.cpp
        
        )
target_link_libraries(${PROJECT_NAME}_vel_node
  ${catkin_LIBRARIES}
  
)
add_executable(${PROJECT_NAME}_tau_node
        ${FDSCSOURCE}
        src/examples/exp_joint_torque_control_lowlevel.cpp
        
        )
target_link_libraries(${PROJECT_NAME}_tau_node
  ${catkin_LIBRARIES}
)
add_executable(${PROJECT_NAME}_tau_tracking_node
        ${FDSCSOURCE}
        src/examples/exp_joint_torquetracking_control_lowlevel.cpp
        )
target_link_libraries(${PROJECT_NAME}_tau_tracking_node
  ${catkin_LIBRARIES}
)
###########
## Test ##
###########
add_executable(${PROJECT_NAME}_test_node
        ${FDSCSOURCE}
        # src/test/test_boost_udp.cpp
        src/test/test_common.cpp
        )
target_link_libraries(${PROJECT_NAME}_test_node
  ${catkin_LIBRARIES}
)
