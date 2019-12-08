import java.io.*;
import java.net.*;

public class UdpNode
{
    private DatagramSocket socket;
    private InetAddress address;
    private int txport;

    public UdpNode(int rxport, int txport) throws SocketException, UnknownHostException
    {
        this.txport = txport;
        socket = new DatagramSocket(rxport);
        address = InetAddress.getByName("localhost");
        System.out.println("Launched");
    }

    public int send(String msg) throws IOException
    {
        byte[] buf = msg.getBytes();
        DatagramPacket txpacket = new DatagramPacket(buf, buf.length, address, txport);
        socket.send(txpacket);
        return txpacket.getLength();
    }

    public String receive() throws IOException
    {
        byte[] buf = new byte[256];
        DatagramPacket rxpacket = new DatagramPacket(buf, buf.length);
        socket.receive(rxpacket);
        String msg = new String(rxpacket.getData(), 0, rxpacket.getLength());
        return msg;
    }

    public void close()
    {
        socket.close();
    }
}
