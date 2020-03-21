/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include "SimAgent.h"


SimAgent::SimAgent(const ConfigReader& config) :
        _numDroppedPackets(0), _verbose(config.verbose)
{
    _comms = std::make_unique<UdpNode>(config.simView.port, config.sim.comms.ip, config.sim.comms.simViewPort);

    std::cout << "Rx from sim at " << config.simView.ip << ":" << config.simView.port << std::endl;
    std::cout << "Tx to sim at " << config.sim.comms.ip << ":" << config.sim.comms.simViewPort << std::endl;
}



void SimAgent::txHeartbeat()
{
    // Transmit state
    std::string msg = "{}";

    if (_verbose)
    {
        printf("SimAgent: Sent %s\n", msg.c_str());
    }

    _comms->send(msg);
}



bool SimAgent::rxSimState()
{
    std::string msg = _comms->receive();
    if (msg.length() > 0 && msg[0] == '{')
    {
        if (_verbose)
        {
            printf("SimAgent: Received command %s\n", msg.c_str());
        }

        // Translate received commands from JSON and store into _simState
        std::lock_guard<std::mutex> lockGuard(_m);
        _simState.fromJson(msg);

        // Reset dropped packets count
        _numDroppedPackets = 0;

        return true;
    }
    else
    {
        _numDroppedPackets++;
        return false;
    }
}



bool SimAgent::isConnected() const
{
    // As long as we've heard from the controls <= 100 packets ago, we're still connected
    return _numDroppedPackets < 100;
}
