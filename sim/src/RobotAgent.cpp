/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "RobotAgent.h"


RobotAgent::RobotAgent() : state{0}, commands{0}
{

}



void RobotAgent::txRobotState()
{

}



double RobotAgent::rxRobotCommands()
{
    return commands.elevatorMotorSpeed;
}
