public class CommsThread extends Thread
{
    public RobotAgent robotAgent;
    public JoystickAgent joystickAgent;

    public CommsThread(RobotAgent robotAgent, JoystickAgent joystickAgent)
    {
        this.robotAgent = robotAgent;
        this.joystickAgent = joystickAgent;
    }

    public void run()
    {
        while (true)
        {
            // Receive robot state
            robotAgent.rxRobotState();

            // Send robot commands
            robotAgent.txRobotCommands();

            // Send joystick heartbeat
            joystickAgent.txHeartbeat();

            // Receive joystick commands
            joystickAgent.rxCommands();

            try
            {
                Thread.sleep(10);
            }
            catch(Exception e)
            {

            }
        }
    }
}
