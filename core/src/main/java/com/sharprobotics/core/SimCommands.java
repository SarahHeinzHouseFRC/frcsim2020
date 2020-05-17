/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;

import java.util.ArrayList;


/**
 * Storage class for holding parameters to be sent to the sim
 */
public class SimCommands
{
    public int leftDriveMotorSpeed; // Integer from -512 to 512
    public int rightDriveMotorSpeed; // Integer from -512 to 512
    public int intakeCenterMotorSpeed; // Integer from -512 to 512
    public int intakeLeftMotorSpeed; // Integer from -512 to 512
    public int intakeRightMotorSpeed; // Integer from -512 to 512
    public int tubeMotorSpeed; // Integer from -512 to 512
    public int timerStartStop; // Integer 0 or 1
    public int reset; // Integer 0 or 1
    public int outtake; // Integer 0 or 1
    public ArrayList<Drawer> draw; // Shapes to draw

    public SimCommands()
    {
        draw = new ArrayList<Drawer>();
    }
}
