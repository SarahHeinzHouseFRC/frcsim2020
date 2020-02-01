/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include <thread>
#include "ConfigReader.h"
#include "Scene.h"
#include "Hud.h"
#include "Visualizer.h"
#include "Time.h"
#include "Timer.h"
#include "CoreAgent.h"
#include "WorldModel.h"
#include "CollisionDetector.h"


int main(int argc, char** argv)
{
    // Use default or user-specified config
    std::string configPath = (argc < 2 ? "../../config/robotConfig.yml" : argv[1]);

    // Configure options
    bool verbose = false;
    if (argc > 2 && strcmp(argv[2], "--verbose") == 0)
    {
        verbose = true;
    }

    bool debugView = false;
    if (argc > 2 && strcmp(argv[2], "--debug-view") == 0)
    {
        debugView = true;
    }

    // Read config file
    ConfigReader config;
    config.verbose = verbose;
    config.debugView = debugView;
    try
    {
        config.parse(configPath);
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    // Initialize vehicle and field models
    double t = Time::now();
    WorldModel wm(config, t);
    CollisionDetector collisions(wm, t);

    // Initialize a timer to countdown 2m 15s
    Timer timer(t, 135);

    // Initialize comms with core
    CoreAgent coreAgent(config);

    // Visualize vehicle and field
    Scene scene(config, wm);
    Hud hud(config);

    // Visualize the scene
    Visualizer vis(scene, hud);

    // Launch rx comms in background thread
    bool reset = false;
    std::thread rxThread([&]()
    {
        while (!vis.done())
        {
            // Receive commands
            bool rx = coreAgent.rxCoreCommands();

            if (rx)
            {
                // Update the vehicle's control surfaces based on the commands from core
                auto rxCommands = coreAgent.getCoreCommands();

                // If the user hits "start" on the joystick, then start/stop the countdown timer
                timer.processCommand(rxCommands.start, Time::now());

                // If the user hits "guide" on the joystick, then reset the world
                if (rxCommands.guide)
                {
                    reset = true;
                }

                // If the timer hits zero, stop allowing the controller to update the vehicle
                if (timer.getValue() <= 0)
                {
                    rxCommands.reset();
                }
                wm.vehicleModel().processCommands(rxCommands);
            }
        }
    });

    // Launch tx comms in background thread
    std::thread txThread([&]()
    {
        while (!vis.done())
        {
            coreAgent.setSensorState(wm.vehicleModel().getSensorState());
            coreAgent.txSensorState();
        }
    });

    std::thread visThread([&]()
    {
        while (!vis.done())
        {
            // Update the vehicle and field visualizations based on their models
            scene.update(wm);

            // Update the hud
            hud.displayConnectionStatus(coreAgent.isConnected());
            hud.displayVehicleState(wm.vehicleModel());
            hud.displayTimerStatus(timer.isRunning(), timer.getValue());
            hud.displayNumCollisions(wm.fieldModel().getNumCollisions());

            // Step the visualizer
            vis.step();
        }
    });

    // Run the sim
    while (!vis.done())
    {
        // Get current time (sec)
        t = Time::now();

        // Update the timer
        timer.update(t);

        // Update the world to reflect the current time
        wm.update(t);

        // Apply collisions and constraints
        collisions.update(wm, t);

        if (reset)
        {
//            wm.reset();
//            collisions.reset(wm);
            timer.reset();
            reset = false;
        }
    }

    rxThread.join();
    txThread.join();
    visThread.join();

    return 0;
}
