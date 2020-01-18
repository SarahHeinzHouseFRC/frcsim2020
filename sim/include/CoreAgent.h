/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef SHARP2019_COREAGENT_H
#define SHARP2019_COREAGENT_H

#include "ConfigReader.h"
#include "Types.h"
#include "UdpNode.h"


/**
 * Receives commands and transmits robot state over comms.enc
 */
class CoreAgent
{
public:
    /**
     * Constructor
     */
    CoreAgent(const ConfigReader& config);

    /**
     * Publishes the vehicle's sensors' states
     */
    void txSensorState();

    /**
     * Receives robot commands
     * @return True if successful rx, false otherwise
     */
    bool rxCoreCommands();

    /**
     * Get the last command received
     */
    CoreCommands getCoreCommands() const { return _coreCommands; }

    /**
     * State to be sent
     */
    void setSensorState(const SensorState& state) { _sensorState = state; }

    /**
     * Whether or not we're connected to the controls
     */
    bool isConnected();

private:
    SensorState _sensorState;
    CoreCommands _coreCommands;
    UdpNode *_comms;
    int _numDroppedPackets; ///< Count of how many packets have been missed
};


#endif //SHARP2019_COREAGENT_H
