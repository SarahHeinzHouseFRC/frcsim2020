/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include "ConfigReader.h"
#include "Types.h"
#include "AbstractAgent.h"


/**
 * Receives world state from and transmits heartbeat to sim over comms
 */
class SimAgent : public AbstractAgent
{
public:
    /**
     * Constructor
     */
    SimAgent(const ConfigReader& config);

    /**
     * Publishes an empty message
     */
    void txHeartbeat();

    /**
     * Receives sim state
     * @return Latest sim state or empty state if connection lost
     */
    SimState rxSimState();

private:
    SimState _simState;
    bool _verbose;
};
