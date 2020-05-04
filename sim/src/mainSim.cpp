/**
 * Copyright (c) 2020 Team 3260
 */

#include <iostream>
#include <thread>
#include "ArgumentParser.h"
#include "ConfigReader.h"
#include "Scene.h"
#include "Hud.h"
#include "Visualizer.h"
#include "Time.h"
#include "Timer.h"
#include "CoreAgent.h"
#include "SimViewAgent.h"
#include "WorldModel.h"

#define DEFAULT_CONFIG_FILE "../../config/robotConfig.yml"


int main(int argc, char** argv)
{
    // Configure from command line arguments
    ArgumentParser args(argc, argv);
    std::string configPath = (args.contains("--config") ? args.getValue("--config") : DEFAULT_CONFIG_FILE);
    bool verbose = args.contains("--verbose") || args.contains("-v");
    bool headless = args.contains("--headless");
    bool debugView = args.contains("--debug-view");
    if (args.contains("--help") || args.contains("-h"))
    {
        std::cout << "\nUsage: ./robot_sim [flags]\n"
                     "\n"
                     "Optional arguments:\n"
                     "  -h, --help               Show this help message and exit\n"
                     "  --config <config_file>   Use the given config file instead of the default\n"
                     "  --debug-view             Launch with a lightweight view\n"
                     "  --headless               Launch without visualization\n"
                     "  --verbose                Increase output verbosity\n";
        return 0;
    }

    // Read config file
    ConfigReader config;
    try
    {
        config.parse(configPath);
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    config.verbose = verbose;
    config.debugView = debugView;
    config.headless = headless;

    // Initialize vehicle and field models
    double t = Time::now();
    WorldModel wm(config, t);

    // Initialize a timer to countdown 2m 15s
    Timer timer(t, 135);

    // Initialize comms with core and simviews
    std::vector<CoreAgent> coreAgents;
    for (int i=0; i<config.players.size(); i++)
    {
        config.sim.comms.corePort = 8000 + 10*i;
        config.core.simPort = 6000 + 10*i;
        coreAgents.emplace_back(config);
    }
    std::vector<SimViewAgent> simViewAgents;
    for (int i=0; i<config.players.size(); i++)
    {
        config.simView.port = 12000 + 10*(i);
        config.sim.comms.simViewPort = 10000 + 10*(i);
        simViewAgents.emplace_back(config);
    }

    // Visualize vehicle and field
    Scene scene(config);
    Hud hud(config);

    // Visualize the scene
    Visualizer vis(scene, hud, headless);

    // Launch rx comms in background thread
    bool reset = false;
    std::thread rxThread([&]()
    {
        while (!vis.done())
        {
            for (int i=0; i<coreAgents.size(); i++)
            {
                CoreAgent& coreAgent = coreAgents.at(i);

                // Receive commands
                bool rx = coreAgent.rxCoreCommands();

                if (rx)
                {
                    // Update the vehicle's control surfaces based on the commands from core
                    auto rxCommands = coreAgent.getCoreCommands();

                    // If the user hits "start" on the joystick, then start/stop the countdown timer
                    timer.processCommand(rxCommands.timerStartStop, Time::now());

                    // If the user hits "guide" on the joystick, then reset the world
                    if (rxCommands.reset)
                    {
                        reset = true;
                    }

                    // If the timer hits zero, stop allowing the controllers to update the vehicle
                    if (timer.getValue() <= 0)
                    {
                        rxCommands.clear();
                    }
                    wm.vehicleModel(i).processCommands(rxCommands);
                }
            }
            usleep(100);
        }
    });

    // Launch tx comms in background thread
    std::thread txThread([&]()
    {
        while (!vis.done())
        {
            for (int i=0; i<coreAgents.size(); i++)
            {
                CoreAgent& coreAgent = coreAgents.at(i);
                coreAgent.setSensorState(wm.vehicleModel(i).getSensorState());
                coreAgent.txSensorState();
            }

            SimState s = wm.getSimState();
            s.isTimerRunning = timer.isRunning();
            s.timer = timer.getValue();

            for (int i=0; i<simViewAgents.size(); i++)
            {
                SimViewAgent& simViewAgent = simViewAgents.at(i);
                simViewAgent.setSimState(s);
                simViewAgent.txSimState();
            }

            usleep(1e4);
        }
    });

    std::thread visThread([&]()
    {
        while (!vis.done())
        {
            if (headless) { break; }

            SimState s = wm.getSimState();
            s.isTimerRunning = timer.isRunning();
            s.timer = timer.getValue();

            // Update the vehicle and field visualizations based on their models
            scene.update(s);

            // Update the hud
            std::vector<bool> connected;
            for (const auto& coreAgent : coreAgents)
            {
                connected.push_back(coreAgent.isConnected());
            }
            hud.displayConnectionStatus(connected);
            hud.displayTimerStatus(s.isTimerRunning, s.timer);
            hud.displayFieldScore(s.blueScore, s.redScore);

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

        if (reset)
        {
            wm.reset();
            timer.reset();
            reset = false;
        }
    }

    rxThread.join();
    txThread.join();
    visThread.join();

    return 0;
}
