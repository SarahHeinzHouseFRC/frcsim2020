cmake_minimum_required(VERSION 3.0)
project(robot_sim)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgDB osgGA osgText)
find_package(Box2D)

include_directories(
        include
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
        ${BOX2D_INCLUDE_DIR}
        /usr/include/rapidjson
)

set(SRCS
        src/main.cpp
        src/WorldModel.cpp
        src/FieldModel.cpp
        src/VehicleModel.cpp
        src/FieldView.cpp
        src/VehicleView.cpp
        src/GamePieceView.cpp
        src/ViewUtils.cpp
        src/CoreAgent.cpp
        src/ConfigReader.cpp
        src/Scene.cpp
        src/Hud.cpp
        src/HudElement.cpp
        src/Visualizer.cpp
        src/Time.cpp
        src/Color.cpp
        src/UdpNode.cpp
        src/PhysicsEngine.cpp
        src/Geometry.cpp
        src/ArgumentParser.cpp
        )

add_executable(robot_sim ${SRCS})

target_link_libraries(robot_sim
        ${OPENSCENEGRAPH_LIBRARIES}
        ${BOX2D_LIBRARY}
        yaml-cpp
        pthread
)
