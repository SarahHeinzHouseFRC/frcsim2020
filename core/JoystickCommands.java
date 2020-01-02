/**
 * Storage class for holding parameters to be received from the joystick
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

    public void fromJson(String str)
    {
        xLeftJoystick = Integer.parseInt(str.substring(2, 7));
        yLeftJoystick = Integer.parseInt(str.substring(8, 13));
        xRightJoystick = Integer.parseInt(str.substring(14, 19));
        yRightJoystick = Integer.parseInt(str.substring(20, 25));
        a = Integer.parseInt(str.substring(26, 27));
        b = Integer.parseInt(str.substring(27, 28));
        x = Integer.parseInt(str.substring(28, 29));
        y = Integer.parseInt(str.substring(29, 30));
        upDpad = Integer.parseInt(str.substring(31, 32));
        downDpad = Integer.parseInt(str.substring(32, 33));
        leftDpad = Integer.parseInt(str.substring(33, 34));
        rightDpad = Integer.parseInt(str.substring(34, 35));

        // TODO: Error checking
    }
}
