/**
 * Storage class for holding parameters to be sent to the robot
 */
public class RobotCommands
{
    /** Integer from 0-1024 */
    public int elevatorMotorSpeed;

    public String toJson()
    {
        String str = String.format("{ %05d }", elevatorMotorSpeed);
        return str;
    }
}
