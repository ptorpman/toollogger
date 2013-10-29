==========================================================
= ToolLogger version 0.1
==========================================================

Sometimes it can be useful to keep track of the usage of your
tools or programs. Is someone using this nifty tool at all??
Well, this is usually done using a licensing program or something
like that. But since these programs are quite expensive, 
the ToolLogger be a low cost (read: no cost) alternative.

The way it works is that you keep a program (the server) running
on a machine. This server accepts incoming UDP messages from your
programs around the world and stores these messages in log files.

Link your program with the ToolLogger client library (libtoollogger.a)
and then call logToolUsage() when the program is started and when it
is shutting down... 
When shutting down, a message will be sent to the server containing
the following info:
        
        * Protocol - The version of the protocol.        
        * Tool     - The program name
        * Revision - The version of the program.
        * User     - The username of the one running the program.
        * Project  - Arbitrary information.
        * RunTime  - The time the program was running.

It is possible to have different protocols, i.e message contents,
running at the same time. However, currently the server is only
supporting one protocol.

Please, read INSTALL for more information.

Privacy & Integrity
===================

I strongly suggest that you inform the users of your tools that
they are being 'monitored' and explain why you are doing this.


Reporting Bugs
==============

Report any bugs or bug fixes to peter@torpman.com


Have fun!

Best Regards,
/ Peter




