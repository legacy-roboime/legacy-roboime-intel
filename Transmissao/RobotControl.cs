using System;
using System.IO;
using System.Collections.Generic;
using System.Timers;
using System.Text;
using System.Threading;
using System.Net.Sockets;
using LR;
using ProtoBuf;

namespace ControleRobo
{
    class RobotControl
    {
        #region Variables
        private int numRobots;

        private int serverPortTeambots = 9000;
        private int serverPortIntelligence = 9050;
        private int clientPort = 9876;

        private UDPServer serverIntelligence;
        private UDPServer serverTeambots;

        private TransmissorRFM12USB transmitter;
        private UdpClient simulationClient;

        private string intelData;
        private byte[] teambotsTranslatedData;
        private byte[] intelTranslatedData;
        
        private byte[] robotData;
        //private string robotTranslatedBuffer;

        //private int priorityRobot;

        private static System.Timers.Timer sendTimer;

        #endregion 

        public void UDPSend(object sender, ElapsedEventArgs e)
        {
            this.serverIntelligence.sendData = Encoding.ASCII.GetBytes(" ");
            if (serverIntelligence.receivedData != null)
            {
                MemoryStream messageStream = new MemoryStream();
                messageStream.Write(serverIntelligence.receivedData, 0, serverIntelligence.receivedData.Length);
                ToTransmission receivedMessage = Serializer.Deserialize<ToTransmission>(messageStream);
                this.intelTranslatedData = Protocols.TranslateProtocolIntelligence(receivedMessage, false);
            }
            if (intelTranslatedData != null)
            {
                simulationClient.Send(intelTranslatedData, intelTranslatedData.Length);
            }
        }
        //SendReceive bonito pra n robôs omni
        public void TxSendReceive(object sender, ElapsedEventArgs e)
        {
            if (serverIntelligence.receivedData.Length > 2)
            {
                MemoryStream messageStream = new MemoryStream();
                messageStream.Write(serverIntelligence.receivedData, 0, serverIntelligence.receivedData.Length);
                ToTransmission receivedMessage = Serializer.Deserialize<ToTransmission>(messageStream);
                this.intelTranslatedData = Protocols.TranslateProtocolIntelligence(receivedMessage, true);
            }

            if (intelTranslatedData != null)
            {
                transmitter.Transmitir(intelTranslatedData);
            }
            MemoryStream sendStream = new MemoryStream();
            ToIntelligence message = new ToIntelligence();
            List<ReceiveRobot> robots = new List<ReceiveRobot>();
            List<int> receivedRobots = new List<int>();

            bool stopTrying = false;
            DateTime time = DateTime.Now;
            
            while (!stopTrying)
            {
                robotData = Encoding.ASCII.GetBytes(transmitter.Receber());
                ReceiveRobot robot = Protocols.ReverseTranslateProtocol(robotData);
                
                if (!receivedRobots.Contains(robotData[2]))
                {
                    receivedRobots.Add(robotData[2]);
                    robots.Add(robot);
                }

                if (DateTime.Now.Subtract(time).Milliseconds > 50)
                    stopTrying = true;  
            }
            Serializer.Serialize<ToIntelligence>(sendStream, message);
            sendStream.Seek(0, SeekOrigin.Begin);

            StreamReader rdr = new StreamReader(sendStream);
            serverIntelligence.sendData = Encoding.ASCII.GetBytes(rdr.ReadToEnd());
        }

        //SendReceive feio pra 1 robô omni
        public void TxSendReceiveCompetition(object sender, ElapsedEventArgs e)
        {
            if (serverIntelligence.receivedData.Length > 2)
            {
                    MemoryStream messageStream = new MemoryStream();
                    messageStream.Write(serverIntelligence.receivedData, 0, serverIntelligence.receivedData.Length);
                    ToTransmission receivedMessage = Serializer.Deserialize<ToTransmission>(messageStream);
                    this.intelTranslatedData = Protocols.TranslateProtocolIntelligence(receivedMessage, true);   
            }

            if (intelTranslatedData != null)
            {
                transmitter.Transmitir(intelTranslatedData);
            }
            if (serverTeambots.receivedData.Length > 2)
            {
                this.teambotsTranslatedData = Protocols.TranslateProtocolTeambots(Encoding.ASCII.GetString(serverTeambots.receivedData, 0, serverTeambots.receivedData.Length), true);
            }
            if (teambotsTranslatedData != null)
            {
                transmitter.Transmitir(teambotsTranslatedData);
            }

            MemoryStream sendStream = new MemoryStream();
            ToIntelligence message = new ToIntelligence();
            List<uint> receivedRobots = new List<uint>();
            
            bool stopTrying = false;
            DateTime time = DateTime.Now;

            while (!stopTrying)
            {
                robotData = Encoding.ASCII.GetBytes(transmitter.Receber());
                try
                {
                    ReceiveRobot robot = Protocols.ReverseTranslateProtocol(robotData);
                    if (!receivedRobots.Contains(robot.id))
                    {
                        receivedRobots.Add(robot.id);
                        message.robots.Add(robot);
                    }
                }
                //catch (IndexOutOfRangeException NullReferenceException)
                catch
                {
                    break;
                }
                if (DateTime.Now.Subtract(time).Milliseconds > 50)
                    stopTrying = true;
            }

            Serializer.Serialize<ToIntelligence>(sendStream, message);
            sendStream.Seek(0, SeekOrigin.Begin);
            StreamReader rdr = new StreamReader(sendStream);

            serverIntelligence.sendData = Encoding.ASCII.GetBytes(rdr.ReadToEnd());
        }

        public void StartCommunication(bool realTransmitter)
        {
          
            serverIntelligence = new UDPServer(serverPortIntelligence);
            serverTeambots = new UDPServer(serverPortTeambots);
            Thread UDPServerTeambotsThread = new Thread(serverTeambots.Execute);
            Thread UDPServerIntelligenceThread = new Thread(serverIntelligence.Execute);
            
            UDPServerTeambotsThread.Start();
            while (!UDPServerTeambotsThread.IsAlive) ;
            Thread.Sleep(1);
            
            UDPServerIntelligenceThread.Start();
            while (!UDPServerIntelligenceThread.IsAlive) ;
            Thread.Sleep(1);

            if (realTransmitter)
            {
                transmitter = new LR.TransmissorRFM12USB();
                transmitter.Inicializar("VIVATxRx", "IME");
                sendTimer = new System.Timers.Timer(70); //1000.0 / 60.0);//frequencia sensor camera
                sendTimer.Elapsed += new ElapsedEventHandler(TxSendReceiveCompetition);
                sendTimer.Enabled = true;

                string blah = "";
                while (blah != "quit")
                {
                    blah = Console.ReadLine();
                }

            }
            else
            {
                this.simulationClient = new UdpClient("127.0.0.1", clientPort);
                sendTimer = new System.Timers.Timer(70); //1000.0 / 60.0);//frequencia sensor camera
                sendTimer.Elapsed += new ElapsedEventHandler(UDPSend);
                sendTimer.Enabled = true;

                string blah = "";
                while (blah != "quit")
                {
                    blah = Console.ReadLine();
                }
            }
        }

        static void Main(string[] args)
        {
            
            Console.WriteLine("Módulo de Transmissão - Emissao");
            RobotControl comm = new RobotControl();
            /*
            ConsoleKey[] keys = {ConsoleKey.W, ConsoleKey.A, ConsoleKey.S, ConsoleKey.D, ConsoleKey.Q, ConsoleKey.E};
            KeyboardController controller = new KeyboardController(2, keys);
            controller.Start();
            
            Controller[] controllers = {controller};
            */
            comm.StartCommunication(false);
            

            /*
            Console.WriteLine("Módulo de Transmissão - Recepcao");
            RobotControl comm = new RobotControl();

            while (true)
            {
                TransmissorRFM12USB transmitter = new LR.TransmissorRFM12USB();
                transmitter.Inicializar("VIVATxRx", "IME");
                string data;
                byte[] bytes;
                System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
                while (true)
                {
                    data = transmitter.Receber();
                    bytes = encoding.GetBytes(data);
                    foreach(byte i in bytes)
                        Console.Write(i.ToString());
                    Console.WriteLine();
                }
            }
            */
        }
    }
}
