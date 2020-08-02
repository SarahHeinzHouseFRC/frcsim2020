/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;


/**
 * Storage class for holding parameters to be received from the sim
 */
public class LidarPoint
{
    public double azimuth; // Angle from 0 - 2pi
    public double elevation; // Angle from 0 - 2pi
    public double range; // Distance in meters
}
