/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include "SimViewAgent.h"

/** Connection timeout in seconds */
#define CONNECTED_TIMEOUT 0.5


SimViewAgent::SimViewAgent(const ConfigReader& config) :
        AbstractAgent(config.sim.comms.simViewPort, config.simView.ip, config.simView.port),
        _simState{},
        _verbose(config.verbose)
{
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



void SimViewAgent::rxHeartbeat()
{
    std::string msg = _comms->receive();
    if (msg.length() > 0 && msg == "{}")
    {
        _connected = true;

        // Save time of last rx
        _prevRxTime = getCurrentTime();
    }
    else
    {
        if (getCurrentTime() - _prevRxTime > CONNECTED_TIMEOUT)
        {
            _connected = false;
        }
    }
}
