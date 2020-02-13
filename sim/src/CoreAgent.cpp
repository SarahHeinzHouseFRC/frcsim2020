/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include <ConfigReader.h>
#include "CoreAgent.h"


CoreAgent::CoreAgent(const ConfigReader& config) :
        _sensorState{0}, _coreCommands{}, _numDroppedPackets(0), _verbose(config.verbose)
{
    _comms = new UdpNode(config.sim.comms.port, config.core.ip, config.core.vehiclePort);
}



void CoreAgent::txSensorState()
{
    // Transmit state
    std::string msg = _sensorState.asJson();

    _comms->send(msg);
}



bool CoreAgent::rxCoreCommands()
{
    std::string msg = _comms->receive();
    if (msg[0] == '{')
    {
        if (_verbose)
        {
            printf("CoreAgent: Received command %s\n", msg.c_str());
        }

        // Translate received commands from JSON and store into _coreCommands
        _coreCommands = CoreCommands(msg);

        // Reset dropped packets count
        _numDroppedPackets = 0;

        return true;
    }
    else
    {
        _coreCommands.clear();
        _numDroppedPackets++;
        return false;
    }
}



bool CoreAgent::isConnected()
{
    // As long as we've heard from the controls <= 100 packets ago, we're still connected
    return _numDroppedPackets < 100;
}
