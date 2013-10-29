using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ToolLogger;
using System.Threading;

namespace SampleToolLoggerClient
{
    class Program
    {
        static void Main(string[] args)
        {
            ToolLogger.Client c = new ToolLogger.Client("127.0.0.1", 11111);

            // Send a message when starting...
            c.logToolUsage("sampleTool", "0.1", "peter", "ToolLoggerProj");
            
            // Simulate tool executing...
            Thread.Sleep(30000);

            // Send a message when exiting...
            c.logToolUsage();
 
        }
    }
}
