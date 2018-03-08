/*
SMSController.cs
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Receives SMS messages and forwards them to Nimbe using UDP
*/

using Nexmo.Api;
using System.Web.Mvc;
using System.Diagnostics;
using System.Web.Http;
using Newtonsoft.Json;
using System.IO;

using System.Net;
using System.Net.Sockets;

//Run in ngrok first: ngrok http 8080 -host-header="localhost:8080"

namespace NexmoDotNetQuickStarts.Controllers
{

    public class SMSController : Controller
    {

        public ActionResult Index()
        {
            return View();
        }

        [System.Web.Mvc.HttpGet]
        public ActionResult Send()
        {
            return View();
        }

        //If an inbound message is detected
        [System.Web.Mvc.HttpGet]
        public ActionResult Recieve([FromUri]SMS.SMSInbound response)
        {
            if (null != response.to && null != response.msisdn)
            {
                //Write the incoming message
                Debug.WriteLine("From: " + response.msisdn);
                Debug.WriteLine(" Message: " + response.text);
                Debug.WriteLine(" ");

                //send the message in UDP
                string IPAd = "127.0.0.1";
                int port = 5677;

                byte[] packetData = System.Text.ASCIIEncoding.ASCII.GetBytes(response.msisdn + response.text);

                IPEndPoint ep = new IPEndPoint(IPAddress.Parse(IPAd), port);
                Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                client.SendTo(packetData, ep);

                return new HttpStatusCodeResult(200);
            }
            else
            {
                return new HttpStatusCodeResult(200);

            }
        }
    }

}