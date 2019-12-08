/**
 * Storage class for holding parameters to be received from the robot
 */
public class RobotState
{
    /** Integer from 0-1024 */
    public int elevatorEncoderPos;

    public void fromJson(String str)
    {
        elevatorEncoderPos = Integer.parseInt(str.substring(2, 7));
    }
}
