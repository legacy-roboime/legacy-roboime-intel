using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

public class UDPServer
{
    public int recv;
    public byte[] receivedData;
    public byte[] sendData;

    IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 9050);

    Socket newsock = new Socket(AddressFamily.InterNetwork,
                    SocketType.Dgram, ProtocolType.Udp);

    public UDPServer(int port)
    {
        receivedData = new byte[1024];
        sendData = new byte[1024];
        ipep = new IPEndPoint(IPAddress.Any, port);

        newsock = new Socket(AddressFamily.InterNetwork,
                        SocketType.Dgram, ProtocolType.Udp);
        newsock.Bind(ipep);
    }

    public void Execute()
    {
        //Console.WriteLine("Waiting for a client...");
        IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
        EndPoint tmpRemote = (EndPoint)(sender);
        while (true)
        {
            recv = newsock.ReceiveFrom(receivedData, ref tmpRemote);
            newsock.SendTo(sendData, sendData.Length, SocketFlags.None, tmpRemote);
        }
    }
}

