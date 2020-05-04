/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;


/**
 * Storage class for holding parameters to be received from the joystick.
 */
public class JoystickCommands
{
    public int[] leftJoystick; // Integer from -512 to 512
    public int[] rightJoystick; // Integer from -512 to 512
    public int[] dpad; // Integer value 0 or 1
    public int leftTrigger; // Integer from 0 to 512
    public int rightTrigger; // Integer from 0 to 512
    public int leftBumper; // Integer value 0 or 1
    public int rightBumper; // Integer value 0 or 1
    public int a; // Integer value 0 or 1
    public int b; // Integer value 0 or 1
    public int x; // Integer value 0 or 1
    public int y; // Integer value 0 or 1
    public int back; // Integer value 0 or 1
    public int guide; // Integer value 0 or 1
    public int start; // Integer value 0 or 1

    JoystickCommands()
    {
        leftJoystick = new int[2];
        rightJoystick = new int[2];
        dpad = new int[4];
    }
}
