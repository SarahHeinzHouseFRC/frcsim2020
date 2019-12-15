/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef SHARP2019_ROBOTAGENT_H
#define SHARP2019_ROBOTAGENT_H

#include "Types.h"
#include "UdpNode.h"


/**
 * Receives commands and transmits robot state over comms.enc
 */
class RobotAgent
{
public:
    /**
     * Constructor
     */
    RobotAgent();

    /**
     * Publishes the robot's state
     */
    void txRobotState();

    /**
     * Receives robot commands
     * @return True if successful rx, false otherwise
     */
    bool rxRobotCommands();

    /**
     * Get the last command received
     */
    RobotCommands getRobotCommands() { return _commands; }

    /**
     * State to be sent
     */
    void setRobotState(const RobotState& state) { _state = state; }

private:
    RobotState _state;
    RobotCommands _commands;
    UdpNode *_comms;
};


#endif //SHARP2019_ROBOTAGENT_H
