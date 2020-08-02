/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include "ConfigReader.h"
#include "Types.h"
#include "AbstractAgent.h"


/**
 * Receives commands from and transmits observations to core over comms
 */
class CoreAgent : public AbstractAgent
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
     * Receives robot commands from core
     * @return Latest core commands or empty commands if connection lost
     */
    CoreCommands rxCoreCommands();

    /**
     * State to be sent
     */
    void setSensorState(const SensorState& state) { _sensorState = state; }

private:
    SensorState _sensorState;
    CoreCommands _coreCommands;
    bool _verbose;
};
