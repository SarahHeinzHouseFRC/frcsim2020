/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include "ConfigReader.h"
#include "Types.h"
#include "UdpNode.h"


/**
 * Receives commands and transmits robot state over comms.enc
 */
class SimViewAgent
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
     * State to be sent
     */
    void setSimState(const SimState& state) { _simState = state; }

    /**
     * Whether or not we're connected to the controls
     */
    bool isConnected() const;

private:
    SimState _simState;
    std::unique_ptr<UdpNode> _comms;
    bool _verbose;
    int _numDroppedPackets; ///< Count of how many packets have been missed
};
