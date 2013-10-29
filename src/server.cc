/****************************************************************************
 * Copyright (c) 2005 Peter R. Torpman.                                     *
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
 *  Author: Peter R. Torpman <peter@torpman.com> 2005                       *
 ****************************************************************************/

/* INCLUDES */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <alloca.h>

#include "server.hh"

LoggerServer::LoggerServer(char* servName,
                           char* lDir )
{

  // See if compiled in values should be overridden
  if (servName) {
    serviceName = strdup(servName);
  }    
  else {
    serviceName = strdup(LOGGER_SERVICE);
  }

  if (lDir) {
    loggerDir = strdup(lDir);
  }    
  else {
    loggerDir = strdup(LOGGER_DIR);
  }

  if (OpenBigBrotherSocket() == -1) {
    fprintf(stderr,"Error: Could not open big brother socket!\n");
    exit(1);
  }

  // Initialize
  SetCurrentDate();
}

LoggerServer::~LoggerServer( )
{
  // Close Big Broter socket
  close(mySocket[0].fd);

  // Clear some memory.
  free(serviceName);
  free(loggerDir);
}



int LoggerServer::OpenBigBrotherSocket( )
{
  int sockfd, bindno;
  struct sockaddr_in serv_addr;
  struct servent *logserver;
  
  // Get the logger server.
  logserver = getservbyname(serviceName, "udp");

  if (logserver) {
    // Could find the port of the service
    serviceSocket = logserver->s_port;
  }
  else {
    fprintf(stderr,"Error: Could not get service socket!\n");
    return -1;
  }

  if ((sockfd=socket(AF_INET, SOCK_DGRAM,0)) < 0) {
    printf("server: errno = %d\n", errno);
    printf("server: can't open datagram socket\n");
    return -1;
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = serviceSocket;

  bindno = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (bindno < 0) {
    fprintf(stderr,"Could  not bind socket!(Error: %d)\n", errno);
    return -1;
  }

  mySocket[0].fd = sockfd;
  mySocket[0].events = POLLIN;

  fprintf(stderr,
          "ToolLogger Server %s Started (Service:%s Port:%d)\n", 
          SERVER_VERSION,
          logserver->s_name,
          ntohs(serviceSocket));

  return 1;
}

void LoggerServer::PollSocket( )
{
  int result;
  char mesg[4096];
  struct sockaddr_in cli_addr;
  size_t len = sizeof(cli_addr);
  int n;


  result = poll(mySocket, 1, -1);

  // Poll from running tools around the world
  if (mySocket[0].revents == POLLIN) {
    char *useMsg;
    // Stuff on the Big Brother socket
    n = recvfrom(mySocket[0].fd, 
                 mesg, 
                 4095, 
                 0, 
                 (struct sockaddr *) &cli_addr, 
                 &len);

    useMsg = (char*) malloc( (n + 1) * sizeof(char));

    memcpy(useMsg, mesg, n);

    /* NULL terminate string */
    useMsg[n] = '\0';

    HandleBigBrotherMessage(useMsg, inet_ntoa(cli_addr.sin_addr));

    free(useMsg);
  }
}

void LoggerServer::HandleBigBrotherMessage( char * message, char *host )
{
  char *buffPtr;
  char logString[8192];
  int  protVersion = -1;

  // Check if we have a new day.
  CheckDate();

  // Protocol Version
  buffPtr = strtok(message,"#\n\t");
  if (buffPtr) {
    if (sscanf( buffPtr, "%d", &protVersion ) != 1) {
      fprintf(stderr,"Error: Could not extract protocol version!\n");
      return;
    }
  }


  if (protVersion == 1) {
    // Protocol 1 is:
    // "ProtVer Tool Rev User Project RunTime"

    char tool[256] = "\0";
    char rev[256]  = "\0";
    char user[256] = "\0";
    char proj[256] = "\0";

    // Tool
    buffPtr=strtok(NULL,"#\n\t");
    if (buffPtr) {
      strcpy(tool,buffPtr);
    }
    
    // Revision
    buffPtr=strtok(NULL,"#\n\t");
    if (buffPtr) {
      strcpy(rev,buffPtr);
    }

    // User
    buffPtr=strtok(NULL,"#\n\t");
    if (buffPtr) {
      strcpy(user,buffPtr);
    }

    // Project
    buffPtr=strtok(NULL,"#\n\t");
    if (buffPtr) {
      strcpy(proj,buffPtr);
    }

    char * timeStr = NULL;

    // RunTime
    buffPtr=strtok(NULL,"\n");
    if (buffPtr) {
      unsigned long t;
      
      if (sscanf(buffPtr,"%ld", &t) != -1 ) {
        timeStr = (char*)alloca(1024 * sizeof(char));
        
        longToTime(t, timeStr);
      }
    }

    // Build the log string
    sprintf(logString,
            "%-4d \"%-32s\" %-7s %-10s %-18s \"%-15s\" %s",
            protVersion,
            (tool ? tool : "Unknown"),
            (rev  ? rev  : "Unknown"),
            (user ? user : "Unknown"),
            (host ? host : "Unknown"),
            (proj ? proj : "Unknown"),
            (timeStr ? timeStr: "Unknown"));

  }
  else {
    fprintf(stderr,"Error: Unhandled protocol - %d\n",
            protVersion);
    return;
  }

  // Update the log file ...
  UpdateLogFile(logString);
}

// Sets the current date
void LoggerServer::SetCurrentDate( )
{
  char * theDate;

  theDate = GetCurrentDate( );

  strcpy(currentDate, theDate);

  free(theDate); 

}

// Checks if we have a new day
void LoggerServer::CheckDate( )
{
  char *tmpDate;

  tmpDate = GetCurrentDate();

  if (strcmp(currentDate,tmpDate) == 0) {
    // It is still the same day

    free(tmpDate);
    return;
  }
  else {
    // It is a new day. Update the current date
    strcpy(currentDate,tmpDate);
  }

  free(tmpDate);
}

char* LoggerServer::GetCurrentDate( )
{
  time_t t;
  char * theTime;
  struct tm *tp;
  char *tmpDate = (char*) malloc(sizeof(char) * 256);

  t = time(&t);
  theTime = ctime(&t);
  tp = gmtime(&t);
  
  sprintf( tmpDate,
           "%4d%02d%02d",
           (1900+tp->tm_year),
           (1+tp->tm_mon),
           tp->tm_mday );
  
  return tmpDate;
}

void LoggerServer::UpdateLogFile( char * logString )
{
  FILE *fp;
  char tmpFileName[1024];

  sprintf(tmpFileName,
          "%s/toollog.%s",
          loggerDir,
          currentDate);

  fp = fopen(tmpFileName, "a");

  if (!fp) {
    // Could not create new file!
    fprintf(stderr, "Error opening log file!\n");
    exit(1);
  }

  // Data to write for current date
  char tmpString[16384];
  
  sprintf(tmpString,
          "%s %s\n",
          currentDate,
          logString );

  fputs( tmpString, fp );
  fclose(fp);
}

void LoggerServer::longToTime( unsigned long timeSec, char * timeStr )
{
  unsigned long  days;
  unsigned long  hours;
  unsigned long  minutes;
  unsigned long  seconds;
  unsigned long  i;

  // Days
  days = timeSec / ( 24 * 60 * 60);

  // Get what's left
  i = timeSec % ( 24 * 60 * 60);

  // Hours
  hours   = i / (60 * 60);

  // Get what's left
  i = i % (60 * 60);

  // Minutes
  minutes = i / 60;

  // Seconds
  seconds = i % 60;

  sprintf(timeStr,
          "%04ld:%02ld:%02ld:%02ld",
          days,
          hours,
          minutes,
          seconds );

  return;
}

// Prints the usage string
void printUsage( ) 
{
  fprintf( stdout,
           "loggerserver: ToolLogger Server %s\n"
           "Usage: loggerserver\n"
           "  [-h]     [--help]       Shows this help text\n"
           "  [-s]     [--service]    Set service name\n"
           "  [-d]     [--logdir]     Set directory for log files\n\n"
           "Report bugs to peter@torpman.com\n" ,
           SERVER_VERSION );
}

// This is the main function... It just starts
// up the polling of the UDP socket...
int main( int argc, char ** argv)
{
  int currArg = 1;
  char * serviceName = NULL;
  char * logDir = NULL;

  while (currArg < argc) {

    if (strcmp( argv[currArg], "-h" ) == 0 ||
        strcmp( argv[currArg], "--help" ) == 0) {
      printUsage( );
      exit(0);
    }

    if (strcmp( argv[currArg], "-s" ) == 0 ||
        strcmp( argv[currArg], "--service" ) == 0) {
      if (argv[currArg+1] == NULL) {
        printUsage( );
        exit(1);
      }

      serviceName = strdup( argv[currArg+1] );
      currArg += 2;
    }
    else if(strcmp( argv[currArg], "-d" ) == 0 ||
            strcmp( argv[currArg], "--logdir" ) == 0) {
      if (argv[currArg+1] == NULL) {
        printUsage( );
        exit(1);
      }

      logDir = strdup( argv[currArg+1] );
      currArg += 2;
    }
    else {
      printUsage( );
      exit(1);
    }
  }
  
  // Create our server...
  LoggerServer theUDP(serviceName,logDir);

  // Clean up some memory...
  free(serviceName);
  free(logDir);
  
  // Eternal loop...
  while(1) {
    theUDP.PollSocket();
  }
  
  return 0;
}
