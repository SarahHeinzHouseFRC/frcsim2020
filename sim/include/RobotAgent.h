/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef SHARP2019_ROBOTAGENT_H
#define SHARP2019_ROBOTAGENT_H

#include "ConfigReader.h"
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
    RobotAgent(const ConfigReader& config);

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
    RobotCommands getRobotCommands() const { return _commands; }

    /**
     * State to be sent
     */
    void setRobotState(const RobotState& state) { _state = state; }

    /**
     * Whether or not we're connected to the controls
     */
    bool isConnected();

private:
    RobotState _state;
    RobotCommands _commands;
    UdpNode *_comms;
    int _numDroppedPackets; ///< Count of how many packets have been missed
};


#endif //SHARP2019_ROBOTAGENT_H
