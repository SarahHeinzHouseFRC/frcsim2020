/**
 * Copyright (c) 2020 Team 3260
 */

/**
 * Storage class for holding parameters to be received from the sim
 */
public class SimState
{
    public int leftDriveEncoderPosition; // Integer from 0-1024
    public int rightDriveEncoderPosition; // Integer from 0-1024
    public int elevatorEncoderPos; // Integer from 0-1024

    public void fromJson(String str)
    {
        leftDriveEncoderPosition = Integer.parseInt(str.substring(22, 26));
        rightDriveEncoderPosition = Integer.parseInt(str.substring(49, 53));
        elevatorEncoderPos = Integer.parseInt(str.substring(74, 78));
    }
}
