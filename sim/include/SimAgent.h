/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef SHARP2019_COREAGENT_H
#define SHARP2019_COREAGENT_H

#include <mutex>
#include "ConfigReader.h"
#include "Types.h"
#include "UdpNode.h"


/**
 * Receives commands and transmits robot state over comms.enc
 */
class SimAgent
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
     * Receives state of sim
     * @return True if successful rx, false otherwise
     */
    bool rxSimState();

    /**
     * Get the last command received
     */
    SimState getSimState() { std::lock_guard<std::mutex> lockGuard(_m); return _simState; }

    /**
     * Whether or not we're connected to the controls
     */
    bool isConnected() const;

private:
    SimState _simState;
    std::unique_ptr<UdpNode> _comms;
    bool _verbose;
    int _numDroppedPackets; ///< Count of how many packets have been missed
    std::mutex _m;
};


#endif //SHARP2019_COREAGENT_H
