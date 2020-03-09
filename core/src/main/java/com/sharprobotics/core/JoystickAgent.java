/**
 * Copyright (c) 2020 Team 3260
 */

package com.sharprobotics.core;

import java.io.*;
import java.net.*;
import com.sharprobotics.io.UdpNode;


/**
 * Comms agent responsible for communicating with the joystick. Continuously sends a heartbeat (empty message) to the
 * joystick so it knows it's connected and continuously listens for inputs from the joystick.
 */
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
            // System.out.println("JoystickAgent: Transmit state " + msg);
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
