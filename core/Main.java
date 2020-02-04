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
        int prevSelectButtonState = 0;
        Boolean isTankDrive = false;

        JoystickAgent joystickAgent = new JoystickAgent(4000, "localhost", 2000);
        RobotAgent robotAgent = new RobotAgent(6000, "localhost", 8000);

        System.out.println("Core: Launched");

        while (true)
        {
            // Send joystick heartbeat
            joystickAgent.txHeartbeat();

            // Receive joystick commands
            joystickAgent.rxCommands();
            JoystickCommands commands = joystickAgent.commands;

            elevatorMotorSpeed = 0;
            if (commands.upDpad == 1)
            {
                elevatorMotorSpeed = 512;
            }
            else if (commands.downDpad == 1)
            {
                elevatorMotorSpeed = -511;
            }

            // Toggle tank drive
            int currBackButtonState = joystickAgent.commands.back;
            if (currBackButtonState == 0 && prevSelectButtonState == 1)
            {
                isTankDrive = !isTankDrive;
                if (isTankDrive)
                {
                    System.out.println("Tank drive enabled");
                }
                else
                {
                    System.out.println("Tank drive disabled");
                }
            }
            prevSelectButtonState = currBackButtonState;

            // Construct robot commands
            if (isTankDrive)
            {
                robotAgent.commands.leftDriveMotorSpeed = commands.yLeftJoystick;
                robotAgent.commands.rightDriveMotorSpeed = commands.yRightJoystick;
            }
            else
            {
                leftDriveMotorSpeed = wrap(commands.yLeftJoystick + commands.xRightJoystick/2, -511, 512);
                rightDriveMotorSpeed = wrap(commands.yLeftJoystick - commands.xRightJoystick/2, -511, 512);
                robotAgent.commands.leftDriveMotorSpeed = leftDriveMotorSpeed;
                robotAgent.commands.rightDriveMotorSpeed = rightDriveMotorSpeed;
            }
            robotAgent.commands.elevatorMotorSpeed = elevatorMotorSpeed;
            robotAgent.commands.back = joystickAgent.commands.back;
            robotAgent.commands.guide = joystickAgent.commands.guide;
            robotAgent.commands.start = joystickAgent.commands.start;

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
