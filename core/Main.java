/**
 * Entry point
 */

public class Main
{
    public static void main(String[] args)
    {
        int elevatorMotorSpeed = 0;

        JoystickAgent joystickAgent = new JoystickAgent(4000, "localhost", 2000);
        RobotAgent robotAgent = new RobotAgent(6000, "localhost", 8000);

        while (true)
        {
            // Send joystick heartbeat
            joystickAgent.txHeartbeat();

            // Receive joystick commands
            joystickAgent.rxCommands();
            JoystickCommands commands = joystickAgent.commands;
            elevatorMotorSpeed = commands.y;

            // Send robot commands
            robotAgent.commands.elevatorMotorSpeed = elevatorMotorSpeed;
            robotAgent.txRobotCommands();

            // Receive robot state
            robotAgent.rxRobotState();
            RobotState state = robotAgent.state;
        }
    }
}
