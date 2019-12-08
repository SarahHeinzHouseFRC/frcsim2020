import java.io.*;
import java.net.*;

public class RobotAgent
{
    public RobotCommands commands;
    public RobotState state;
    private UdpNode comms;

    public RobotAgent(int rxport, int txport)
    {
        state = new RobotState();
        commands = new RobotCommands();
        try
        {
            comms = new UdpNode(rxport, txport);
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
        // Translate 'commands' to JSON...
        String commandsStr = commands.toJson();

        // Send string
        try
        {
            comms.send(commandsStr);
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
            System.out.println("Node 8000: Received state " + msg + " -> " + state.elevatorEncoderPos);

        }
        catch (IOException e)
        {
            System.out.println("RobotAgent: Failed to rx");
        }
    }
}
