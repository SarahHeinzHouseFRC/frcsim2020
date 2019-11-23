/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include "RobotModel.h"
#include "RobotConfig.h"
#include "RobotAgent.h"
#include "Scene.h"


int main(int argc, char** argv)
{
    // Read config file
    RobotConfig config;

    // Initialize robot
    RobotModel robot;

    // Initialize robot agent (sends the robot's state)
    RobotAgent robotAgent;

    // Visualize robot model
    Scene scene;
}
