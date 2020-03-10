/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include <ConfigReader.h>
#include "SimViewAgent.h"


SimViewAgent::SimViewAgent(const ConfigReader& config) :
        _numDroppedPackets(0), _verbose(config.verbose)
{
    _comms = std::make_unique<UdpNode>(config.sim.comms.simViewPort, config.simView.ip, config.simView.port);

    std::cout << "Rx from simview at " << config.sim.comms.ip << ":" << config.sim.comms.simViewPort << std::endl;
    std::cout << "Tx to simview at " << config.simView.ip << ":" << config.simView.port << std::endl;
}



void SimViewAgent::txSimState()
{
    // Transmit state
    std::string msg = _simState.toJson();

    if (_verbose)
    {
        printf("SimViewAgent: Sent %s\n", msg.c_str());
    }

    _comms->send(msg);
}



bool SimViewAgent::isConnected() const
{
    // As long as we've heard from the controls <= 100 packets ago, we're still connected
    return _numDroppedPackets < 100;
}
