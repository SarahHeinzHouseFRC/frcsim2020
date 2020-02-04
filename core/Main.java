/**
 * Entry point
 */

public class Main
{
    public static void main(String[] args)
    {
        JoystickAgent joystickAgent = new JoystickAgent(4000, "localhost", 2000);
        SimAgent simAgent = new SimAgent(6000, "localhost", 8000);

        CommsThread commsThread = new CommsThread(simAgent, joystickAgent);
        commsThread.start();

        System.out.println("Core: Launched");

        int elevatorMotorSpeed = 0;
        int leftDriveMotorSpeed = 0;
        int rightDriveMotorSpeed = 0;
        int prevSelectButtonState = 0;
        Boolean isTankDrive = false;

        while (true)
        {
            //
            // Construct sim commands
            //

            // Construct elevator command
            elevatorMotorSpeed = 0;
            if (joystickAgent.commands.upDpad == 1)
            {
                elevatorMotorSpeed = 512;
            }
            else if (joystickAgent.commands.downDpad == 1)
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

            // Construct sim commands
            if (isTankDrive)
            {
                simAgent.commands.leftDriveMotorSpeed = joystickAgent.commands.yLeftJoystick;
                simAgent.commands.rightDriveMotorSpeed = joystickAgent.commands.yRightJoystick;
            }
            else
            {
                leftDriveMotorSpeed = wrap(joystickAgent.commands.yLeftJoystick + joystickAgent.commands.xRightJoystick/2, -511, 512);
                rightDriveMotorSpeed = wrap(joystickAgent.commands.yLeftJoystick - joystickAgent.commands.xRightJoystick/2, -511, 512);
                simAgent.commands.leftDriveMotorSpeed = leftDriveMotorSpeed;
                simAgent.commands.rightDriveMotorSpeed = rightDriveMotorSpeed;
            }
            simAgent.commands.elevatorMotorSpeed = elevatorMotorSpeed;
            simAgent.commands.back = joystickAgent.commands.back;
            simAgent.commands.guide = joystickAgent.commands.guide;
            simAgent.commands.start = joystickAgent.commands.start;

            try
            {
                Thread.sleep(10);
            }
            catch(Exception e)
            {

            }
        }
    }

    private static int wrap(int val, int min, int max)
    {
        if (val < min) { return min; }
        else if (val > max) { return max; }
        else { return val; }
    }
}
