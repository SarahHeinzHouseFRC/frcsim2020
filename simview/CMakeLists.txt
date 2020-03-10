cmake_minimum_required(VERSION 3.0)
project(robot_sim_view)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgDB osgGA osgText)

include_directories(
        include
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
        /usr/include/rapidjson
)

set(SRCS
        src/main.cpp
        src/FieldView.cpp
        src/VehicleView.cpp
        src/GamePieceView.cpp
        src/ViewUtils.cpp
        src/SimAgent.cpp
        src/ConfigReader.cpp
        src/Scene.cpp
        src/Hud.cpp
        src/HudElement.cpp
        src/Visualizer.cpp
        src/Color.cpp
        src/UdpNode.cpp
        src/Geometry.cpp
        src/ArgumentParser.cpp
        )

add_executable(robot_sim_view ${SRCS})

target_link_libraries(robot_sim_view
        ${OPENSCENEGRAPH_LIBRARIES}
        yaml-cpp
        pthread
)
