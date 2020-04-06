/**
 * Copyright (c) 2020 Team 3260
 */

#include <string>
#include "CoreAgent.h"

#define NUM_ALLOWABLE_DROPPED_PACKETS 500


CoreAgent::CoreAgent(const ConfigReader& config) :
        _sensorState{0}, _coreCommands{}, _numDroppedPackets(0), _verbose(config.verbose)
{
    _comms = std::make_unique<UdpNode>(config.sim.comms.corePort, config.core.ip, config.core.simPort);

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



bool CoreAgent::rxCoreCommands()
{
    _coreCommands.clear();
    std::string msg = _comms->receive();
    if (msg.length() > 0 && msg[0] == '{')
    {
        if (_verbose)
        {
            printf("CoreAgent: Received command %s\n", msg.c_str());
        }

        // Translate received commands from JSON and store into _coreCommands
        _coreCommands.fromJson(msg);

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



bool CoreAgent::isConnected() const
{
    // As long as we've heard from the controls <= 100 packets ago, we're still connected
    return _numDroppedPackets < NUM_ALLOWABLE_DROPPED_PACKETS;
}
