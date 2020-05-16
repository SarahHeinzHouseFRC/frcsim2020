/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include "ConfigReader.h"
#include "Types.h"
#include "AbstractAgent.h"


/**
 * Receives heartbeat from and transmits world state to sim view over comms
 */
class SimViewAgent : public AbstractAgent
{
public:
    /**
     * Constructor
     */
    SimViewAgent(const ConfigReader& config);

    /**
     * Publishes the vehicle's sensors' states
     */
    void txSimState();

    /**
     * Receives sim view heartbeat
     */
    void rxHeartbeat();

    /**
     * State to be sent
     */
    void setSimState(const SimState& state) { _simState = state; }

private:
    SimState _simState;
    bool _verbose;
};
