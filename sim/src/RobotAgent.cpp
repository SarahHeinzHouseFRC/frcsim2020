/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <string>
#include <ConfigReader.h>
#include "RobotAgent.h"


RobotAgent::RobotAgent(const ConfigReader& config) : _state{0}, _commands{}, _numDroppedPackets(0)
{
    _comms = new UdpNode(config.vehicle.port, config.core.ip, config.core.vehiclePort);
}



void RobotAgent::txRobotState()
{
    // Transmit state
    std::string stateJson = _state.asJson();

    _comms->send(stateJson);
}



bool RobotAgent::rxRobotCommands()
{
    std::string msg = _comms->receive();
    if (msg[0] == '{')
    {
        // Translate received commands from JSON and store into _commands
        _commands = RobotCommands(msg);

        // Reset dropped packets count
        _numDroppedPackets = 0;

        printf("RobotAgent: Received command %s -> %d\n", msg.c_str(), _commands.elevatorMotorSpeed);
        return true;
    }
    else
    {
        _numDroppedPackets++;
        return false;
    }
}



bool RobotAgent::isConnected()
{
    // As long as we've heard from the controls <= 10 packets ago, we're still connected
    return _numDroppedPackets < 100;
}
