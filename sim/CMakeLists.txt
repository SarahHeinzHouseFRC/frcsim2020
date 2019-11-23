cmake_minimum_required(VERSION 2.6)
project(robot_sim)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgDB osgGA osgText)

include_directories(
        include
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

set(SRCS
        src/main.cpp
        src/RobotModel.cpp
        src/RobotAgent.cpp
        src/ConfigReader.cpp
        src/Scene.cpp
        src/Color.cpp
        )

add_executable(robot_sim ${SRCS})

target_link_libraries(robot_sim
        ${OPENSCENEGRAPH_LIBRARIES}
)
