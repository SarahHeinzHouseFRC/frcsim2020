cmake_minimum_required(VERSION 2.6)
project(robot_sim)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgDB osgGA osgText)

add_subdirectory(extern/box2d)

include_directories(
        include
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

set(SRCS
        src/main.cpp
        src/WorldModel.cpp
        src/FieldModel.cpp
        src/VehicleModel.cpp
        src/GamePieceModel.cpp
        src/FieldView.cpp
        src/VehicleView.cpp
        src/GamePieceView.cpp
        src/ViewUtils.cpp
        src/CoreAgent.cpp
        src/ConfigReader.cpp
        src/Scene.cpp
        src/Hud.cpp
        src/Visualizer.cpp
        src/Time.cpp
        src/Color.cpp
        src/Label.cpp
        src/UdpNode.cpp
        src/CollisionDetector.cpp
        src/Geometry.cpp
        )

add_executable(robot_sim ${SRCS})

target_link_libraries(robot_sim
        ${OPENSCENEGRAPH_LIBRARIES}
        yaml-cpp
        pthread
        box2d
)
