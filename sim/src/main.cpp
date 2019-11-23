/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include "RobotModel.h"
#include "ConfigReader.h"
#include "RobotAgent.h"
#include "Scene.h"


int main(int argc, char** argv)
{
    if (argc != 2) { std::cout << "Usage: ./robot_sim <config>" << std::endl; return 1; }

    // Read config file
    ConfigReader config(argv[1]);

    // Initialize robot
    RobotModel robot;

    // Initialize robot agent (sends the robot's state)
    RobotAgent robotAgent;

    // Visualize robot model
    Scene scene;
    scene.render();

    return 0;
}
