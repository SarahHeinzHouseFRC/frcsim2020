/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <string>
#include "RobotAgent.h"


RobotAgent::RobotAgent(const ConfigReader& config) : _state{0}, _commands{0}, _numDroppedPackets(0)
{
    _comms = new UdpNode(config.sim.port, config.controller.ip, config.controller.port);
}



void RobotAgent::txRobotState()
{
    // Transmit state
    char tmp[1024];
    sprintf(tmp, "{ %05d }", _state.elevatorEncoderPosition);
    std::string stateStr = tmp;

    _comms->send(stateStr);
}



bool RobotAgent::rxRobotCommands()
{
    std::string msg = _comms->receive();
    if (msg[0] == '{')
    {
        // Translate received commands from JSON and store into _commands...
        _commands.elevatorMotorSpeed = std::stoi(msg.substr(2, 7));

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
