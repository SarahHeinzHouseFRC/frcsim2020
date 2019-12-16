/**
 * Storage class for holding parameters to be received from the joystick
 */
public class JoystickCommands
{
    public int x; // Integer from -511 to 512
    public int y; // Integer from -511 to 512
    public int a; // Binary value 0 or 1
    public int b; // Binary value 0 or 1

    public void fromJson(String str)
    {
        x = Integer.parseInt(str.substring(2, 7));
        y = Integer.parseInt(str.substring(8, 13));
        a = Integer.parseInt(str.substring(14, 19));
        b = Integer.parseInt(str.substring(20, 25));
    }
}
