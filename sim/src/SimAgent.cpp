/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include "SimAgent.h"

/** Connection timeout in seconds */
#define CONNECTED_TIMEOUT 0.5


SimAgent::SimAgent(const ConfigReader& config) :
        AbstractAgent(config.simView.port, config.sim.comms.ip, config.sim.comms.simViewPort),
        _simState{},
        _verbose(config.verbose)

{
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



SimState SimAgent::rxSimState()
{
    std::string msg = _comms->receive();
    if (msg.length() > 0 && msg[0] == '{')
    {
        _connected = true;

        if (_verbose)
        {
            printf("SimAgent: Received command %s\n", msg.c_str());
        }

        // Translate received commands from JSON and store into _simState
        _simState.fromJson(msg);

        // Save time of last rx
        _prevRxTime = getCurrentTime();
    }
    else
    {
        if (getCurrentTime() - _prevRxTime > CONNECTED_TIMEOUT)
        {
            _connected = false;
            _simState.clear();
        }
    }

    return _simState;
}
