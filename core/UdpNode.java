/**
 * Copyright (c) 2020 Team 3260
 */

import java.io.*;
import java.net.*;

/**
 * Base UDP functionality to abstract comms layer from SimAgent and JoystickAgent
 */
public class UdpNode
{
    private DatagramSocket socket;
    private String txIp;
    private int txPort;

    public UdpNode(int rxPort, String txIp, int txPort) throws SocketException, UnknownHostException
    {
        socket = new DatagramSocket(rxPort);
        socket.setSoTimeout(10);
        this.txIp = txIp;
        this.txPort = txPort;
    }

    public int send(String msg) throws IOException
    {
        byte[] buf = msg.getBytes();
        DatagramPacket txPacket = new DatagramPacket(buf, buf.length, InetAddress.getByName(txIp), txPort);
        socket.send(txPacket);
        return txPacket.getLength();
    }

    public String receive() throws IOException
    {
        byte[] buf = new byte[1024];
        DatagramPacket rxPacket = new DatagramPacket(buf, buf.length);
        socket.receive(rxPacket);
        String msg = new String(rxPacket.getData(), 0, rxPacket.getLength());
        return msg;
    }

    public void close()
    {
        socket.close();
    }
}
