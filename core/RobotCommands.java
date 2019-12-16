/**
 * Storage class for holding parameters to be sent to the robot
 */
public class RobotCommands
{
    public int elevatorMotorSpeed; // Integer from -511 to 512

    public String toJson()
    {
        String str = String.format("{ %05d }", elevatorMotorSpeed);
        return str;
    }
}
