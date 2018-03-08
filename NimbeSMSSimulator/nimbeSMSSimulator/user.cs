/*
user.cs
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Fake user that sends SMS messages to Nimbe. Also responsible for sending the messages to Nimbe using the UDP protocol.
*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace nimbeSMSSimulator
{
    public class user
    {
        //variables
        public string _number;
        bool isOn;

        //constructor
        public user(string number)
        {
            _number = number;
            isOn = false;
        }

        //fuctions
        public void sendMessage(string IPAd, int port)
        {
            string message = chooseMessage();
            byte[] packetData = System.Text.ASCIIEncoding.ASCII.GetBytes(_number + message);
            Console.WriteLine(_number + " " + message);
            IPEndPoint ep = new IPEndPoint(IPAddress.Parse(IPAd), port);
            Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            client.SendTo(packetData, ep);
        }

        private string chooseMessage()
        {
            if (isOn == false)
            {
                //open track
                isOn = true;
                return "on"; 
            }
            else
            {
                Random rnd = new Random();
                int choice = rnd.Next(0, 8);
                //close track
                if (choice == 0)
                {
                    isOn = false;
                    return "off";
                }
                //Change pitch
                else if (choice == 2 || choice == 3 || choice == 4)
                {
                    if (rnd.Next(0, 2) == 1)
                        return "-";
                    else
                        return "+";
                }
                //Change location
                else if (choice == 5 || choice == 6 || choice == 7)
                {
                    int angle = rnd.Next(0, 8);
                    switch (angle)
                    {
                        case 0:
                            return "Nord";
                        case 1:
                            return "Sud";
                        case 2:
                            return "Est";
                        case 3:
                            return "Ouest";
                        case 4:
                            return "Nord-Est";
                        case 5:
                            return "Nord-Ouest";
                        case 6:
                            return "Sud-Est";
                        case 7:
                            return "Sud-Ouest";
                    }
                }
            }
            return "off";
        }
    }
}
