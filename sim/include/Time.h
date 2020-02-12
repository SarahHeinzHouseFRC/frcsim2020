/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_TIME_H
#define ROBOT_SIM_TIME_H


/**
 * Provides time to the whole simulation. For now, this simply uses the current epoch time, but the separation of this
 * class allows running the sim at 2x or 1/2x speed, for example.
 */
class Time
{
public:
    /**
     * Constructor
     */
    Time();

    /**
     * Returns current epoch time in seconds
     */
    static double now();
};


#endif //ROBOT_SIM_TIME_H
