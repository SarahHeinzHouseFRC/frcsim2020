/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include "ConfigReader.h"
#include "Scene.h"
#include "Hud.h"
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
    RobotAgent robotAgent(config);

    // Visualize robot model
    Scene scene(config);
    Hud hud(config);

    // Visualize the scene
    Visualizer vis(scene, hud);

    // Run the simulation
    while (!vis.done())
    {
        // Receive commands
        bool rx = robotAgent.rxRobotCommands();

        if (rx)
        {
            // Update the robot's control surfaces based on the received commands
            auto rxCommands = robotAgent.getRobotCommands();
            robot.processCommands(rxCommands);
        }

        // Get current time (sec)
        t = Time::now();

        // Update the robot state given the received robot commands
        robot.update(t);

        // Update the robot visualization
        scene.update(robot);

        // Transmit robot state
        robotAgent.setRobotState(robot.getState());
        robotAgent.txRobotState();

        // Update the hud
        hud.displayConnected(robotAgent.isConnected());
        hud.displayRobotState(robot);

        // Step the visualizer
        vis.step();
    }

    return 0;
}
