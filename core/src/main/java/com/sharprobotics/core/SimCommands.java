/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;


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

    public String toJson()
    {
        String str = String.format("{ 'leftDriveMotorSpeed': %04d, 'rightDriveMotorSpeed': %04d, 'intakeCenterMotorSpeed': %04d, 'intakeLeftMotorSpeed': %04d, 'intakeRightMotorSpeed': %04d, 'tubeMotorSpeed': %04d, 'timerStartStop': %01d, 'reset': %01d, 'outtake': %01d }",
                leftDriveMotorSpeed, rightDriveMotorSpeed, intakeCenterMotorSpeed, intakeLeftMotorSpeed, intakeRightMotorSpeed, tubeMotorSpeed, timerStartStop, reset, outtake);
        return str;
    }
}
