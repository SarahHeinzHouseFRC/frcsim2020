/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;


/**
 * Storage class for holding parameters to be received from the sim
 */
public class SimState
{
    public int leftDriveEncoder; // Integer from 0-1024
    public int rightDriveEncoder; // Integer from 0-1024
    public LidarPoint[] lidarSweep; // LIDAR sweep
}
