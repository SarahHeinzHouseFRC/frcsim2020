import java.io.*;
import java.net.*;

public class RobotAgent
{
    public RobotCommands commands;
    public RobotState state;
    private UdpNode comms;

    public RobotAgent(int rxPort, String txIp, int txPort)
    {
        state = new RobotState();
        commands = new RobotCommands();
        try
        {
            comms = new UdpNode(rxPort, txIp, txPort);
        }
        catch (Exception e)
        {
            System.out.println("RobotAgent: Failed to init UDP node");
        }
    }

    /**
     * Sends the robot's current state stored in `state`
     */
    public void txRobotCommands()
    {
        // Translate commands to JSON
        String msg = commands.toJson();

        // Send string
        try
        {
            comms.send(msg);
            // System.out.println("RobotAgent: Transmit commands " + msg);
        }
        catch (IOException e)
        {
            System.out.println("RobotAgent: Failed to tx");
        }
    }

    /**
     * Receives the robot's commands and stores it into `commands`
     */
    public void rxRobotState()
    {
        try
        {
            // Receive commands
            String msg = comms.receive();

            // Parse received command from JSON to state
            state.fromJson(msg);
            // System.out.println("RobotAgent: Received " + msg);
        }
        catch (IOException e)
        {
            // System.out.println("RobotAgent: Failed to rx");
        }
    }
}
