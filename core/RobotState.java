/**
 * Storage class for holding parameters to be received from the robot
 */
public class RobotState
{
    public int elevatorEncoderPos; // Integer from 0-1024

    public void fromJson(String str)
    {
        elevatorEncoderPos = Integer.parseInt(str.substring(2, 7));
    }
}
