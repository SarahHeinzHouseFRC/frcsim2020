/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;


/**
 * Storage class for holding parameters to be received from the sim
 */
public class SimState
{
    public float x; // Position (meters)
    public float y; // Position (meters)
    public float theta; // Position (radians)
    public int leftDriveEncoder; // Ticks (0-1023)
    public int rightDriveEncoder; // Ticks (0-1023)
    public int numIngestedBalls; // 0+
    public float[][] lidarPoints; // LIDAR points
}
