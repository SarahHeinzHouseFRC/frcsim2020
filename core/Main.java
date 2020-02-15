/**
 * Copyright (c) 2020 Team 3260
 */

/**
 * Entry point
 */
public class Main
{
    public static void main(String[] args)
    {
        int joystickRxPort = 4000;
        int joystickTxPort = 2000;
        String joystickTxIp = "localhost";
        int simRxPort = 6000;
        int simTxPort = 8000;
        String simTxIp = "localhost";

        JoystickAgent joystickAgent = new JoystickAgent(joystickRxPort, joystickTxIp, joystickTxPort);
        SimAgent simAgent = new SimAgent(simRxPort, simTxIp, simTxPort);

        CommsThread commsThread = new CommsThread(simAgent, joystickAgent);
        commsThread.start();

        System.out.println("Rx joystick at " + joystickTxIp + ":" + joystickTxPort);
        System.out.println("Tx joystick at localhost:" + joystickRxPort);
        System.out.println("Rx sim at " + simTxIp + ":" + simTxPort);
        System.out.println("Tx sim at localhost:" + simRxPort);
        System.out.println("Core: Launched");

        int prevSelectButtonState = 0;
        Boolean isTwoHandDrive = true;

        while (true)
        {
            //
            // Construct sim commands
            //

            // Toggle tank drive
            int currBackButtonState = joystickAgent.commands.back;
            if (currBackButtonState == 0 && prevSelectButtonState == 1)
            {
                isTwoHandDrive = !isTwoHandDrive;
                if (isTwoHandDrive)
                {
                    System.out.println("Two-handed drive enabled");
                }
                else
                {
                    System.out.println("One-handed drive enabled");
                }
            }
            prevSelectButtonState = currBackButtonState;

            // Construct intake motor commands
            int intakeMotorSpeed = 0;
            if (joystickAgent.commands.a == 1)
            {
                intakeMotorSpeed = 512;
            }
            else if (joystickAgent.commands.b == 1)
            {
                intakeMotorSpeed = -512;
            }

            // Construct tube motor commands
            int tubeMotorSpeed = 0;
            if (joystickAgent.commands.x == 1)
            {
                tubeMotorSpeed = 512;
            }
            else if (joystickAgent.commands.y == 1)
            {
                tubeMotorSpeed = -512;
            }

            // Construct drivetrain commands
            int leftDriveMotorSpeed = 0;
            int rightDriveMotorSpeed = 0;
            if (isTwoHandDrive)
            {
                leftDriveMotorSpeed = wrap(joystickAgent.commands.yLeftJoystick + joystickAgent.commands.xRightJoystick/2, -512, 512);
                rightDriveMotorSpeed = wrap(joystickAgent.commands.yLeftJoystick - joystickAgent.commands.xRightJoystick/2, -512, 512);
            }
            else
            {
                leftDriveMotorSpeed = wrap(joystickAgent.commands.yLeftJoystick + joystickAgent.commands.xLeftJoystick/2, -512, 512);
                rightDriveMotorSpeed = wrap(joystickAgent.commands.yLeftJoystick - joystickAgent.commands.xLeftJoystick/2, -512, 512);
            }

            simAgent.commands.intakeCenterMotorSpeed = intakeMotorSpeed;
            simAgent.commands.intakeLeftMotorSpeed = intakeMotorSpeed;
            simAgent.commands.intakeRightMotorSpeed = intakeMotorSpeed;
            simAgent.commands.tubeMotorSpeed = tubeMotorSpeed;
            simAgent.commands.leftDriveMotorSpeed = leftDriveMotorSpeed;
            simAgent.commands.rightDriveMotorSpeed = rightDriveMotorSpeed;
            simAgent.commands.timerStartStop = joystickAgent.commands.start;
            simAgent.commands.reset = joystickAgent.commands.guide;

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
