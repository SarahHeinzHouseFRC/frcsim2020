/**
 * Copyright (c) 2020 Team 3260
 */

#include <iostream>
#include <thread>
#include <mutex>
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
    std::string configPath = (args.contains("--config") ? args.getValue("--config") : DEFAULT_CONFIG_FILE);
    bool verbose = args.contains("--verbose") || args.contains("-v");
    bool debugView = args.contains("--debug-view");
    int playerId = args.contains("--player") ? std::stoi(args.getValue("--player")) - 1 : 0;

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
    if (playerId > config.players.size()-1)
    {
        std::cout << "Only " << config.players.size() << " players configured, choose 1-" << config.players.size() << std::endl;
        return 0;
    }

    // Initialize comms with sim
    config.sim.comms.simViewPort = 10000 + 10*(playerId);
    config.simView.port = 12000 + 10*(playerId);
    SimAgent simAgent(config);

    // Visualize vehicle and field
    Scene scene(config);
    Hud hud(config, playerId);

    // Visualize the scene
    Visualizer vis(scene, hud, playerId);

    std::mutex m;
    SimState state;

    // Launch rx comms in background thread
    std::thread rxThread([&]()
    {
        while (!vis.done())
        {
            // Receive commands
            std::lock_guard<std::mutex> lock(m);
            state = simAgent.rxSimState();
        }
    });

    std::thread txThread([&]()
    {
        while (!vis.done())
        {
            // Transmit heartbeat
            simAgent.txHeartbeat();

            usleep(1e4);
        }
    });

    // Run the sim
    while (!vis.done())
    {
        while (!vis.done())
        {
            m.lock();
            // Update the vehicle and field visualizations based on their models
            scene.update(state);

            // Update the hud
            hud.displayConnectionStatus(simAgent.isConnected(), playerId);
            hud.displayTimerStatus(state.isTimerRunning, state.timer);
            hud.displayFieldScore(state.blueScore, state.redScore);
            hud.displayVehicleState(state, playerId);
            m.unlock();

            // Step the visualizer
            vis.step();
        }
    }

    rxThread.join();

    return 0;
}
