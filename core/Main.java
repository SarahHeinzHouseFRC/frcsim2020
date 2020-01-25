/**
 * Entry point
 */

public class Main
{
    public static void main(String[] args)
    {
        int elevatorMotorSpeed = 0;
        int leftDriveMotorSpeed = 0;
        int rightDriveMotorSpeed = 0;

        JoystickAgent joystickAgent = new JoystickAgent(4000, "localhost", 2000);
        RobotAgent robotAgent = new RobotAgent(6000, "localhost", 8000);

        while (true)
        {
            // Send joystick heartbeat
            joystickAgent.txHeartbeat();

            // Receive joystick commands
            joystickAgent.rxCommands();
            JoystickCommands commands = joystickAgent.commands;
            leftDriveMotorSpeed = wrap(commands.yLeftJoystick + commands.xRightJoystick/2, -511, 512);
            rightDriveMotorSpeed = wrap(commands.yLeftJoystick - commands.xRightJoystick/2, -511, 512);

            elevatorMotorSpeed = 0;
            if (commands.upDpad == 1)
            {
                elevatorMotorSpeed = 512;
            }
            else if (commands.downDpad == 1)
            {
                elevatorMotorSpeed = -511;
            }

            // Construct robot commands
            robotAgent.commands.leftDriveMotorSpeed = leftDriveMotorSpeed;
            robotAgent.commands.rightDriveMotorSpeed = rightDriveMotorSpeed;
            robotAgent.commands.elevatorMotorSpeed = elevatorMotorSpeed;

            // Send robot commands
            robotAgent.txRobotCommands();

            // Receive robot state
            robotAgent.rxRobotState();
            RobotState state = robotAgent.state;
        }
    }

    private static int wrap(int val, int min, int max)
    {
        if (val < min) { return min; }
        else if (val > max) { return max; }
        else { return val; }
    }
}
