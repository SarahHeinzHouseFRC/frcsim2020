/**
 * Copyright (c) 2020 Team 3260
 */

#include <chrono>
#include "AbstractAgent.h"


AbstractAgent::AbstractAgent(uint16_t rxPort, const std::string& txIp, uint16_t txPort) : _prevRxTime(getCurrentTime()), _connected(false)
{
    _comms = std::make_unique<UdpNode>(rxPort, txIp, txPort);
}



double AbstractAgent::getCurrentTime()
{
    using namespace std::chrono;

    const auto epoch = system_clock::now().time_since_epoch();
    return duration_cast<milliseconds>(epoch).count() / 1000.0;
}
