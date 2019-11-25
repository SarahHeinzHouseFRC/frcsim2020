/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include <RobotAgent.h>
#include "ConfigReader.h"
#include "Scene.h"
#include "Visualizer.h"
#include "Time.h"
#include "RobotAgent.h"
#include "RobotModel.h"


int main(int argc, char** argv)
{
    if (argc != 2) { std::cout << "Usage: ./robot_sim <config>" << std::endl; return 1; }

    // Read config file
    ConfigReader config(argv[1]);

    // Initialize robot
    double t = Time::now();
    RobotModel robot(config, t);

    // Initialize robot agent
    RobotAgent robotAgent;

    // Visualize robot model
    Scene scene(config);

    // Visualize the scene
    Visualizer vis(scene);

    // Run the simulation
    while (!vis.done())
    {
        // Receive commands
        robotAgent.rxRobotCommands();

        // Get the current time
        t = Time::now();

        // Update the robot state
        robot.update(t, robotAgent.commands.elevatorMotorSpeed);

        // Update the robot visualization
        scene.update(robot);

        // Transmit robot state
        robotAgent.txRobotState();

        // Step the visualizer
        vis.step();
    }

    return 0;
}
