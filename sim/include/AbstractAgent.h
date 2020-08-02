/**
 * Copyright (c) 2020 Team 3260
 */

#pragma once

#include <memory>
#include "UdpNode.h"


class AbstractAgent
{
public:
    /**
     * Constructor
     */
    AbstractAgent(uint16_t rxPort, const std::string& txIp, uint16_t txPort);

    /**
     * Whether or not we're connected to the controls
     */
    bool isConnected() const { return _connected; }

protected:
    /**
     * Returns the current epoch time in seconds (based on hardware time, not simulator time)
     */
    static double getCurrentTime();

    std::unique_ptr<UdpNode> _comms;
    double _prevRxTime; // Time last packet was received
    bool _connected;
};
