cmake_minimum_required(VERSION 3.0)
project(sim)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgDB osgGA osgText)
find_package(Box2D)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(
        include
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
        ${BOX2D_INCLUDE_DIR}
        /usr/include/rapidjson
)

set(SIM_SRCS
        src/mainSim.cpp
        src/WorldModel.cpp
        src/FieldModel.cpp
        src/VehicleModel.cpp
        src/FieldView.cpp
        src/VehicleView.cpp
        src/GamePieceView.cpp
        src/ViewUtils.cpp
        src/AbstractAgent.cpp
        src/CoreAgent.cpp
        src/SimViewAgent.cpp
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
        src/PhysicsEngineLidar.cpp
        src/KeyHandler.cpp
        )

add_executable(sim ${SIM_SRCS})

target_link_libraries(sim
        ${OPENSCENEGRAPH_LIBRARIES}
        ${BOX2D_LIBRARY}
        yaml-cpp
        pthread
)

set(SIM_VIEW_SRCS
        src/mainSimView.cpp
        src/FieldView.cpp
        src/VehicleView.cpp
        src/GamePieceView.cpp
        src/ViewUtils.cpp
        src/AbstractAgent.cpp
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
        src/KeyHandler.cpp
        )

add_executable(simView ${SIM_VIEW_SRCS})

target_link_libraries(simView
        ${OPENSCENEGRAPH_LIBRARIES}
        yaml-cpp
        pthread
)
