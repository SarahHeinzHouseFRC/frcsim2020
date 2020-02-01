import java.io.*;
import java.net.*;

public class JoystickAgent
{
    public JoystickHeartbeat heartbeat;
    public JoystickCommands commands;
    private UdpNode comms;

    public JoystickAgent(int rxPort, String txIp, int txPort)
    {
        heartbeat = new JoystickHeartbeat();
        commands = new JoystickCommands();
        try
        {
            comms = new UdpNode(rxPort, txIp, txPort);
        }
        catch (Exception e)
        {
            System.out.println("JoystickAgent: Failed to init UDP node");
        }
    }

    /**
     * Sends heartbeat message to the joystick stored in `heartbeat`
     */
    public void txHeartbeat()
    {
        String msg = heartbeat.toJson();

        // Send string
        try
        {
            comms.send(msg);
        }
        catch (IOException e)
        {
            System.out.println("JoystickAgent: Failed to tx");
        }
    }

    /**
     * Receives the joystick's commands and stores it into `commands`
     */
    public void rxCommands()
    {
        try
        {
            // Receive commands
            String msg = comms.receive();

            // Parse received command from JSON to state
            commands.fromJson(msg);
            // System.out.println("JoystickAgent: Received " + msg);

        }
        catch (IOException e)
        {
            // System.out.println("JoystickAgent: Failed to rx");
        }
    }
}
