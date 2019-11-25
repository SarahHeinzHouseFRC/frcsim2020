/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <chrono>
#include "Time.h"


Time::Time()
{

}



double Time::now()
{
    const auto epoch = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count() / 1000.0;
}