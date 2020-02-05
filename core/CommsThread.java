/**
 * Copyright (c) 2020 Team 3260
 */

/**
 * Background thread for handling rx/tx with the joystick and sim
 */
public class CommsThread extends Thread
{
    public SimAgent simAgent;
    public JoystickAgent joystickAgent;

    public CommsThread(SimAgent simAgent, JoystickAgent joystickAgent)
    {
        this.simAgent = simAgent;
        this.joystickAgent = joystickAgent;
    }

    public void run()
    {
        while (true)
        {
            // Receive sim state
            simAgent.rxSimState();

            // Send sim commands
            simAgent.txSimCommands();

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
