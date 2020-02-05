/**
 * Copyright (c) 2020 Team 3260
 */

/**
 * Storage class for holding parameters to be sent to the sim
 */
public class SimCommands
{
    public int elevatorMotorSpeed; // Integer from -511 to 512
    public int leftDriveMotorSpeed; // Integer from -511 to 512
    public int rightDriveMotorSpeed; // Integer from -511 to 512
    public int back; // Integer 0 or 1
    public int guide; // Integer 0 or 1
    public int start; // Integer 0 or 1

    public String toJson()
    {
        String str = String.format("{ %05d %05d %05d %01d%01d%01d }",
                leftDriveMotorSpeed, rightDriveMotorSpeed, elevatorMotorSpeed, back, guide, start);
        return str;
    }
}
