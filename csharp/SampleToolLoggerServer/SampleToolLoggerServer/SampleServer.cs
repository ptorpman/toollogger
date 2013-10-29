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
            new ToolLogger.Server(null, 11111);
        }
    }
}
