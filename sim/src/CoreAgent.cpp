/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include "CoreAgent.h"

/** Connection timeout in seconds */
#define CONNECTED_TIMEOUT 0.5


CoreAgent::CoreAgent(const ConfigReader& config) :
        AbstractAgent(config.sim.comms.corePort, config.core.ip, config.core.simPort),
        _coreCommands{},
        _sensorState{},
        _verbose(config.verbose)
{
    std::cout << "Rx from core at 127.0.0.1:" << config.sim.comms.corePort << std::endl;
    std::cout << "Tx to core at " << config.core.ip << ":" << config.core.simPort << std::endl;
}



void CoreAgent::txSensorState()
{
    // Transmit state
    std::string msg = _sensorState.toJson();

    if (_verbose)
    {
        printf("CoreAgent: Sent %s\n", msg.c_str());
    }

    _comms->send(msg);
}



CoreCommands CoreAgent::rxCoreCommands()
{
    std::string msg = _comms->receive();
    if (msg.length() > 0 && msg[0] == '{')
    {
        _connected = true;

        if (_verbose)
        {
            printf("CoreAgent: Received command %s\n", msg.c_str());
        }

        // Translate received commands from JSON and store into _coreCommands
        _coreCommands.clear();
        _coreCommands.fromJson(msg);

        // Save time of last rx
        _prevRxTime = getCurrentTime();
    }
    else
    {
        if (getCurrentTime() - _prevRxTime > CONNECTED_TIMEOUT)
        {
            _connected = false;
            _coreCommands.clear();
        }
    }

    return _coreCommands;
}
