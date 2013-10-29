/****************************************************************************
 * Copyright (c) 2005-2012 Peter R. Torpman.                                *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Peter R. Torpman <peter at torpman dot se> 2005-2012            *
 ****************************************************************************/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Text.RegularExpressions;
using System.IO;


namespace ToolLogger
{
    static class Constants 
    {
        public const string ProtocolVersion = "1";
    }

    public class Server
    {
        private string _logFileName = "toollogger.log";
        private int _serverPort = 11111;
        private StreamWriter _logFile;

        // Constructor
        public Server(string logFileName, int serverPort = 0)
        {
            if (logFileName != null)
            {
                _logFileName = logFileName;
            }

            if (serverPort != 0) {
                _serverPort = serverPort;
            }

            // Open the log file
            try
            {
                if (!File.Exists("logfile.txt"))
                {
                    _logFile = new StreamWriter(_logFileName);
                }
                else
                {
                    _logFile = File.AppendText(_logFileName);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("{0} Exception caught.", e);
            }


            try
            {
                openBigBrotherSocket();
            }
            catch (Exception e) 
            {
                Console.WriteLine("{0} Exception caught.", e);
            }

        }

        private void openBigBrotherSocket()
        {
            Socket sock    = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint iep = new IPEndPoint(IPAddress.Any, _serverPort);
            EndPoint ep    = (EndPoint)iep;
            sock.Bind(iep);

            while (true)
            {
                byte[] data = new byte[4096];

                Console.WriteLine("Waiting for client input...\n");
                // Blocking read until something is received.
                int recv          = sock.ReceiveFrom(data, ref ep);
                string stringData = Encoding.ASCII.GetString(data, 0, recv);

                handleBigBrotherMessage(stringData, ep.ToString());
            }
        }
      
        private void handleBigBrotherMessage(string message, string host)
        {
            // Protocol version 1 is:
            // "ProtVer Tool Rev User Project RunTime"
            char[] delimiters = new char[] { '\n', '\t', '#' };
            string[] parts = message.Split(delimiters, StringSplitOptions.RemoveEmptyEntries);

            if (parts.Length < 6)
            { 
                // Not enough arguments
                Console.WriteLine("Not enough arguments " + parts.Length.ToString());
                return;
            }

            if (parts[0] != "1") {
                Console.WriteLine("Incorrect protocol version. (%s)", parts[0]);
            }

            // Convert runtime to string
            TimeSpan ts = TimeSpan.FromTicks(Convert.ToInt32(parts[5]));
            double runTime = ts.TotalMinutes;

            // Build the string to send to the log
            // Format:
            //   Protocol Tool Revision User HostName Project NumMinutes
            //   1 myTool 1.0.0 peter toollogger 143
            string logString = parts[0] + " " + parts[1] + " " + parts[2] + " " + parts[3] + " " + host + " " + parts[4];
            logString += " " + runTime.ToString();

            Console.WriteLine("DEBUG: Writing to log:\n" + logString + "\n");

            // Write to the file:
            _logFile.WriteLine(logString);
        }



    }

    public class Client 
    {
        private string _serverHostName;
        private int _serverPort;
        private long _startTick;
        private string _tool;
        private string _version;
        private string _user;
        private string _project;

        public Client(string hostName, int serverPort)
        {
            _serverHostName = hostName;
            _serverPort = serverPort;
        }

        // This function is used to register when the tool started...
        public void logToolUsage(string tool, string version, string user, string project)
        {
            _startTick = DateTime.Now.Ticks;
            _tool = tool;
            _version = version;
            _user = user;
            _project = project;
        }

        // This function is used to register when the tool stopped...
        public void logToolUsage()
        {
            long runTicks = DateTime.Now.Ticks - _startTick;

            sendToServer(runTicks.ToString());
        }

        private void sendToServer(string runTime)
        {
            string logString = "";
            byte[] data = new byte[4096];

            // "ProtVer Tool Rev User Project RunTime"
            // # is used to separate the strings.

            logString += Constants.ProtocolVersion + "#" + _tool + "#" + _version + "#" + _user + "#" + _project + "#" + runTime;
            data = Encoding.ASCII.GetBytes(logString);

            IPEndPoint ip = new IPEndPoint(IPAddress.Parse(_serverHostName), _serverPort);
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            server.SendTo(data, data.Length, SocketFlags.None, ip);
        }

    }

}
