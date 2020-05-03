/**
 * Copyright (c) 2020 Team 3260
 */

 /**
 * Storage class for holding parameters to be received from the joystick.
 */
public class JoystickCommands
{
    public int xLeftJoystick; // Integer from -512 to 512
    public int yLeftJoystick; // Integer from -512 to 512
    public int xRightJoystick; // Integer from -512 to 512
    public int yRightJoystick; // Integer from -512 to 512
    public int upDpad; // Integer value 0 or 1
    public int downDpad; // Integer value 0 or 1
    public int leftDpad; // Integer value 0 or 1
    public int rightDpad; // Integer value 0 or 1
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
        leftTrigger = Integer.parseInt(str.substring(106, 110));
        rightTrigger = Integer.parseInt(str.substring(128, 132));
        leftBumper = Integer.parseInt(str.substring(148, 149));
        rightBumper = Integer.parseInt(str.substring(166, 167));
        a = Integer.parseInt(str.substring(174, 175));
        b = Integer.parseInt(str.substring(182, 183));
        x = Integer.parseInt(str.substring(190, 191));
        y = Integer.parseInt(str.substring(198, 199));
        back = Integer.parseInt(str.substring(209, 210));
        guide = Integer.parseInt(str.substring(221, 222));
        start = Integer.parseInt(str.substring(233, 234));
    }
}
