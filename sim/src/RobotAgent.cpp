/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <string>
#include "RobotAgent.h"


RobotAgent::RobotAgent() : _state{0}, _commands{0}
{
    _comms = new UdpNode(4000, 8000);
}



void RobotAgent::txRobotState()
{
    // Translate _state to JSON...
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

        printf("Node 4000: Received command %s -> %d\n", msg.c_str(), _commands.elevatorMotorSpeed);
        return true;
    }
    else
    {
        return false;
    }
}
