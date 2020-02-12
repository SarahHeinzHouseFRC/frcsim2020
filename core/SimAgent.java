/**
 * Copyright (c) 2020 Team 3260
 */

import java.io.*;
import java.net.*;

/**
 * Comms agent responsible for communicating with the sim. Continuously sends commands to the sim to update the vehicle
 * and continuously listens for vehicle state back from the sim.
 */
public class SimAgent
{
    public SimCommands commands;
    public SimState state;
    private UdpNode comms;

    public SimAgent(int rxPort, String txIp, int txPort)
    {
        state = new SimState();
        commands = new SimCommands();
        try
        {
            comms = new UdpNode(rxPort, txIp, txPort);
        }
        catch (Exception e)
        {
            System.out.println("SimAgent: Failed to init UDP node");
        }
    }

    /**
     * Sends the sim's current state stored in `state`
     */
    public void txSimCommands()
    {
        // Translate commands to JSON
        String msg = commands.toJson();

        // Send string
        try
        {
            comms.send(msg);
            // System.out.println("SimAgent: Transmit commands " + msg);
        }
        catch (IOException e)
        {
            System.out.println("SimAgent: Failed to tx");
        }
    }

    /**
     * Receives the sim's commands and stores it into `commands`
     */
    public void rxSimState()
    {
        try
        {
            // Receive commands
            String msg = comms.receive();

            // Parse received command from JSON to state
            state.fromJson(msg);
            // System.out.println("SimAgent: Received " + msg);
        }
        catch (IOException e)
        {
            // System.out.println("SimAgent: Failed to rx");
        }
    }
}
