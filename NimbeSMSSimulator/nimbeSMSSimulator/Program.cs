/*
Program.cs
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Creates fake users that can send text messages to Nimbe, loops to make them send messages.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;



namespace nimbeSMSSimulator
{
    class Program
    {
        //constants
        const string IPAd = "127.0.0.1";
        const int port = 5677;
        const int numberUsers = 50;
        const int amountOfMessages = 8; //recommended between 5 and 50

        //users
        static void Main(string[] args)
        {
            Random rnd = new Random();
            string regionalCode;
            string firstDigits;
            string secondDigits;
            string completeNumber = "";
            bool isUnique;
            List<user> users = new List<user>();

            //create users
            for (int i = 0; i < numberUsers; i++)
            {
                isUnique = false;
                //Loop until the generated phone number is unique
                while (!isUnique)
                {
                    if (rnd.Next(0,2) == 1)
                        regionalCode = "438";
                    else
                        regionalCode = "514";

                    firstDigits = rnd.Next(10).ToString() + rnd.Next(10).ToString() + rnd.Next(10).ToString();
                    secondDigits = rnd.Next(10).ToString() + rnd.Next(10).ToString() + rnd.Next(10).ToString() + rnd.Next(10).ToString();
                    completeNumber = "1" + regionalCode + firstDigits + secondDigits;

                    isUnique = true;
                    foreach(user currentUser in users)
                    {
                        if (currentUser._number == completeNumber)
                            isUnique = false;
                    }
                }
                //Add the new user to the list
                users.Add(new user(completeNumber));
            }

            //send texts
            {
                Thread.Sleep(20000);
                while (true)
                { 
                    foreach (user currentUser in users)
                    {
                        if (amountOfMessages > rnd.Next(0, 10000))
                        {
                            currentUser.sendMessage(IPAd, port);
                        }
                    }
                    Thread.Sleep(100);
                }

            }   
        }
    }
}
