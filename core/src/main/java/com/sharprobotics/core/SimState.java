/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;


/**
 * Storage class for holding parameters to be received from the sim
 */
public class SimState
{
    public int leftDriveEncoderPosition; // Integer from 0-1024
    public int rightDriveEncoderPosition; // Integer from 0-1024
    public int elevatorEncoderPos; // Integer from 0-1024
}
