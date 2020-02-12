/**
 * Copyright (c) 2020 Team 3260
 */

 /**
 * Storage class for holding parameters to be received from the joystick.
 */
public class JoystickCommands
{
    public int xLeftJoystick; // Integer from -511 to 512
    public int yLeftJoystick; // Integer from -511 to 512
    public int xRightJoystick; // Integer from -511 to 512
    public int yRightJoystick; // Integer from -511 to 512
    public int a; // Binary value 0 or 1
    public int b; // Binary value 0 or 1
    public int x; // Binary value 0 or 1
    public int y; // Binary value 0 or 1
    public int upDpad; // Binary value 0 or 1
    public int downDpad; // Binary value 0 or 1
    public int leftDpad; // Binary value 0 or 1
    public int rightDpad; // Binary value 0 or 1
    public int back; // Binary value 0 or 1
    public int guide; // Binary value 0 or 1
    public int start; // Binary value 0 or 1

    public void fromJson(String str)
    {
        xLeftJoystick = Integer.parseInt(str.substring(20, 24));
        yLeftJoystick = Integer.parseInt(str.substring(26, 30));
        xRightJoystick = Integer.parseInt(str.substring(53, 57));
        yRightJoystick = Integer.parseInt(str.substring(59, 63));
        upDpad = Integer.parseInt(str.substring(77, 78));
        downDpad = Integer.parseInt(str.substring(80, 81));
        leftDpad = Integer.parseInt(str.substring(83, 84));
        rightDpad = Integer.parseInt(str.substring(86, 87));
        a = Integer.parseInt(str.substring(104, 105));
        b = Integer.parseInt(str.substring(107, 108));
        x = Integer.parseInt(str.substring(110, 111));
        y = Integer.parseInt(str.substring(113, 114));
        back = Integer.parseInt(str.substring(126, 127));
        guide = Integer.parseInt(str.substring(139, 140));
        start = Integer.parseInt(str.substring(151, 152));
    }
}
