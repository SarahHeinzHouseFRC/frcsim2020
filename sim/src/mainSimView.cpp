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
#include "SimAgent.h"

#define DEFAULT_CONFIG_FILE "../../config/robotConfig.yml"


int main(int argc, char** argv)
{
    // Configure from command line arguments
    ArgumentParser args(argc, argv);
    std::string configPath = (args.contains("--config") ? args.getValue("--config") : DEFAULT_CONFIG_FILE);
    bool verbose = args.contains("--verbose") || args.contains("-v");
    bool debugView = args.contains("--debug-view");
    int player = args.contains("--player") ? std::stoi(args.getValue("--player")) : 1;
    if (args.contains("--help") || args.contains("-h"))
    {
        std::cout << "\nUsage: ./robot_sim [flags]\n"
                     "\n"
                     "Optional arguments:\n"
                     "  -h, --help               Show this help message and exit\n"
                     "  --config <config_file>   Use the given config file instead of the default\n"
                     "  --debug-view             Launch with a lightweight view\n"
                     "  --player <num>           Player number (1-6)\n"
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
    config.headless = false;

    // Initialize comms with core
    config.sim.comms.simViewPort = 10000 + 10*(player-1);
    config.simView.port = 12000 + 10*(player-1);
    SimAgent simAgent(config);

    // Visualize vehicle and field
    Scene scene(config);
    Hud hud(config);

    // Visualize the scene
    Visualizer vis(scene, hud);

    // Launch rx comms in background thread
    std::thread rxThread([&]()
    {
        while (!vis.done())
        {
            // Receive commands
            bool rx = simAgent.rxSimState();
        }
    });

    // Run the sim
    while (!vis.done())
    {
        while (!vis.done())
        {
            // Update the vehicle and field visualizations based on their models
            SimState state = simAgent.getSimState();
            scene.update(state);

            // Update the hud
            bool connected = true;
            if (!simAgent.isConnected()) { connected = false; }
//            hud.displayConnectionStatus(connected);
            hud.displayTimerStatus(state.isTimerRunning, state.timer);
            hud.displayFieldScore(state.blueScore, state.redScore);
//            hud.displayVehicleState(wm.vehicleModel(i));

            // Step the visualizer
            vis.step();
        }
    }

    rxThread.join();

    return 0;
}
