using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ControleRobo
{
    class Protocols
    {
        private static float kickerVelocity = 60;
        private static float dribblerVelocity = 60;
        private static float wheelVelocity = 60;

        public static byte ScaleVelocity(double realVelocity, double minVelocity, double maxVelocity)
        {
            double scaledspeed;
            if (realVelocity >= 0)
                scaledspeed = (realVelocity - minVelocity) * 127 / (maxVelocity - minVelocity);
            else
                scaledspeed = 127 + (realVelocity - minVelocity) * 127 / (maxVelocity - minVelocity);
            int scaledint = (int)scaledspeed;
			if (scaledint > 127 || scaledint < -127) scaledint = 0;//GFTW
            byte scaled = Byte.Parse(scaledint.ToString());
            return scaled;
        }
        public static double ReverseScaleVelocity(byte scaled, double minVelocity, double maxVelocity)
        {
            double realVelocity, scaledFloat;
            if (scaled > 127)
            {
                scaled -= 127;
                scaledFloat = -1*((float) scaled);
            }
            else
                scaledFloat = (float) scaled;
            realVelocity = minVelocity + scaledFloat * (maxVelocity - minVelocity) / 127;
            return realVelocity;
        }
        public static ReceiveRobot ReverseTranslateProtocol(byte[] robotData)
        {
            try
            {
                string[] splitData = new String[robotData.Length];
                ReceiveRobot robot = new ReceiveRobot();
                ReceiveWheel[] wheels = new ReceiveWheel[4];

                if (robotData[2] == (byte)0)
                {
                    robot.id = ((uint)robotData[1]);
                    robot.has_ball = (-Math.Sign(ReverseScaleVelocity(robotData[3], 0, 100))); //Returns -1 if no ball, 1 if has ball
                    robot.battery_level = (Math.Abs(ReverseScaleVelocity(robotData[3], 0, 100))); //Battery level
                    
                    wheels[0].speed = ReverseScaleVelocity(robotData[3], 0, wheelVelocity);
                    wheels[1].speed = ReverseScaleVelocity(robotData[4], 0, wheelVelocity);
                    wheels[2].speed = ReverseScaleVelocity(robotData[5], 0, wheelVelocity);
                    wheels[3].speed = ReverseScaleVelocity(robotData[6], 0, wheelVelocity);
                    foreach(ReceiveWheel wheel in wheels)
                    {
                        robot.wheels.Add(wheel);
                    }
                }
                if (robotData[2] == (byte)1)
                {
                    robot.id = ((uint)robotData[1]);
                    robot.has_ball = (-Math.Sign(ReverseScaleVelocity(robotData[3], 0, 100))); //Returns -1 if no ball, 1 if has ball
                    robot.battery_level = (Math.Abs(ReverseScaleVelocity(robotData[3], 0, 100))); //Battery level
                }
                return robot;
            }
            catch
            {
                return null;
            }
        }
        /*
        public static string ReverseTranslateProtocol(byte[] robotData)
        {
            string[] splitData = new String[robotData.Length];
            string temp = "";
            if (robotData[2] == (byte)0)
            {
                splitData[0] = ((int)robotData[1]).ToString();
                splitData[1] = (-Math.Sign(ReverseScaleVelocity(robotData[3], 0, 100))).ToString(); //Returns -1 if no ball, 1 if has ball
                splitData[2] = (Math.Abs(ReverseScaleVelocity(robotData[3], 0, 100))).ToString(); //Battery level
                splitData[3] = ReverseScaleVelocity(robotData[3], 0, wheelVelocity).ToString();
                splitData[4] = ReverseScaleVelocity(robotData[4], 0, wheelVelocity).ToString();
                splitData[5] = ReverseScaleVelocity(robotData[5], 0, wheelVelocity).ToString();
                splitData[6] = ReverseScaleVelocity(robotData[6], 0, wheelVelocity).ToString();
            }
            if (robotData[2] == (byte)1)
            {
                splitData[0] = ((int)robotData[1]).ToString();
                splitData[1] = (-Math.Sign(ReverseScaleVelocity(robotData[3], 0, 100))).ToString(); //Returns -1 if no ball, 1 if has ball
                splitData[2] = (Math.Abs(ReverseScaleVelocity(robotData[3], 0, 100))).ToString(); //Battery level
            }
            foreach (string subString in splitData)
            {
                temp = String.Concat(temp, " ");
                temp = String.Concat(temp, subString);
            }
            temp = String.Concat(temp, '\n');
            return temp;
        }
        */

        public void salvarPacote(byte[] translated, byte velocidade0, byte velocidade1, byte velocidade2, byte velocidade3, byte velocidade4, byte velocidade5, byte velocidade6, byte velocidade7, byte velocidade8, byte velocidade9)
        {
            translated[0] = 0;
            translated[1] = 0x22;//poacote com todos os paramentros dos robos
            translated[2] = 0;
            translated[3] = velocidade0;
            translated[4] = velocidade1;
            translated[5] = 1;
            translated[6] = velocidade2;
            translated[7] = velocidade3;
            translated[8] = 2;
            translated[9] = velocidade4;
            translated[10] = velocidade5;
            translated[11] = 3;
            translated[12] = velocidade6;
            translated[13] = velocidade7;
            translated[14] = 4;
            translated[15] = velocidade8;
            translated[16] = velocidade9;
        }
        
        public static byte[] TranslateProtocolTeambots(String message, bool realTransmitter)
        {
            try
            {
                byte[] translated = new byte[18];
                string[] splitData = message.Split(new Char[] { ' ', '\n' });

                translated[0] = 0;
                translated[1] = 0x22;
                for (int i = 2; i < 16; i++)
                {
                    translated[i] = byte.Parse(splitData[i + 1]);
                }
                return translated;
            }
            catch(IndexOutOfRangeException)
            {
                return null;
            }
        }
        

        public static byte[] TranslateProtocolIntelligence(ToTransmission message, bool realTransmitter)
        {
            try
            {
                byte[] translated = new byte[39];
                // Protocolo para o robô
                #region realTransmitter

                if (realTransmitter)
                {
                    int i = 0;
                    translated[i] = Byte.Parse(44.ToString()); i++;
                    foreach (CommandRobot robot in message.robots)
                    {
                        translated[i] = (byte)robot.id; i++;
                        foreach (CommandWheel wheel in robot.wheels)
                        {
                            translated[i] = ScaleVelocity(wheel.speed, 0, 100); i++;
                        }
                        translated[i] = ScaleVelocity(robot.dribbler.speed, 0, 120); i++;
                        translated[i] = ScaleVelocity(robot.kicker.speed, 0, 130); i++;
                    }
                    translated[i] = 0; i++;
                    //foreach (byte b in translated)
                    //    Console.Write(b.ToString() + " ");
                }
                #endregion
                //Protocolo para o simulador
                #region simulation
                else
                {
                    List<string> splitStrings = new List<string>();
                    foreach (CommandRobot robot in message.robots)
                    {
                        foreach (CommandWheel wheel in robot.wheels)
                        {
                            splitStrings.Add(wheel.speed.ToString());
                        }
                        splitStrings.Add(robot.dribbler.speed.ToString());
                        splitStrings.Add(robot.kicker.speed.ToString());
                    }
                    string appendString = "15 0 0 ";
                    translated = Encoding.ASCII.GetBytes(string.Concat(appendString, string.Join(" ", splitStrings)));
                }
                #endregion
                return translated;
            }
            catch (IndexOutOfRangeException)
            {
                return null;
            }
            catch (NullReferenceException)
            {
                return null;
            }
        }
        /*
        public static byte[] TranslateProtocol(string intelData, bool realTransmitter)
        {
            byte[] translated;
            // Protocolo para o robô
            #region realTransmitter
            if (realTransmitter)
            {
                translated = PreliminaryTranslation(intelData); 
                
                foreach (byte b in translated)
                    Console.Write(b.ToString() + " ");
                Console.WriteLine();
                Console.WriteLine(intelData);
                
            }
            #endregion
            //Protocolo para o simulador
            #region simulation
            else
            {
                string appendString = "15 0 0 ";
                intelData = string.Concat(appendString, intelData);
                translated = Encoding.ASCII.GetBytes(intelData);
            }
            #endregion
            return translated;
        }
        
        private static byte[] PreliminaryTranslation(string intelData)
        {
            byte[] translated;
            translated = new byte[36];
            int j = 0;
            translated[j] = 0x22;
            j++;
            byte k = 1;
            translated[j] = k;
            j++;
            k++;
            Console.WriteLine(k);
            string[] splitData = intelData.Split(new Char[] { ' ', '\n' });
            for (int i = 0; (i < splitData.Length) && j < 36; i++)
            {
                if (splitData[i] == "")
                    // Kicker
                    continue;
                else if ((j) % 7 == 6)
                {
                    // Dribbler
                    translated[j] = ScaleVelocity(float.Parse(splitData[i]), 0, kickerVelocity);
                    j++;
                }
                else if ((j) % 7 == (7) % 7)
                {
                    translated[j] = ScaleVelocity(float.Parse(splitData[i]), 0, dribblerVelocity);
                    j++;
                    if (k != 6)
                        translated[j] = k++;

                    j++;
                }
                else
                {
                    // Wheels
                    translated[j] = ScaleVelocity(float.Parse(splitData[i]), 0, wheelVelocity);
                    j++;
                }
                //Console.WriteLine(splitData[i] + " "+i.ToString());
                //Console.WriteLine(translated[j].ToString() + " " + j.ToString());                   
            }

            return translated;
        }*/
    }
}
