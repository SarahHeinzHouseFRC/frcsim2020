/**
 * Copyright (c) 2020 Team 3260
 */

/**
 * Storage class for holding parameters to be sent to the sim
 */
public class SimCommands
{
    public int elevatorMotorSpeed; // Integer from -512 to 512
    public int leftDriveMotorSpeed; // Integer from -512 to 512
    public int rightDriveMotorSpeed; // Integer from -512 to 512
    public int back; // Integer 0 or 1
    public int guide; // Integer 0 or 1
    public int start; // Integer 0 or 1

    public String toJson()
    {
        String str = String.format("{ 'leftDriveMotorSpeed': %04d, 'rightDriveMotorSpeed': %04d, 'elevatorMotorSpeed': %04d, 'back': %01d, 'guide': %01d, 'start': %01d }",
                leftDriveMotorSpeed, rightDriveMotorSpeed, elevatorMotorSpeed, back, guide, start);
        return str;
    }
}
