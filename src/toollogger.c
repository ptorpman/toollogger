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

#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>                               /* memcpy() */
#include <time.h>                                 /* time() */
#include "toollogger.h"

/* GLOBAL VARIABLES */

/* LOCAL VARIABLES */

/* FUNCTION DEFINITIONS */

static time_t startTime;
static time_t stopTime;

void 
logToolUsage(char* tool, char* revision, char* project, bool  flag)
{
  int    sockfd;
  struct sockaddr_in serv_addr;
  struct servent *udpServer;
  struct hostent *hp;
  char   *hostIP = NULL;
  int    portNumber = LOGGER_PORT;
  int    result = 0;
  char   *user;
  char   serverMsg[4096] = "\0";
  long   runTime;

  /* Check Parameters */
  if (!tool) {
    /* Tool name cannot be empty. */
    return;
  }

  if (!revision) {
    /* Revision cannot be empty. */
    return;
  }
  
  if (flag) { 
    /* The program is started. Get system time. */
    time(&startTime);
    runTime = 0;

    /* We do not send a message when starting... */
    return;
  }
  else {
    /* The program is stopped. 
       Get system time and calculate total running time. */
    time(&stopTime);
    runTime = stopTime - startTime;

    if (runTime < 0) {
      /* Something wrong here */
      return;
    }
  }

  /* Get a hold of the ToolLogger service. */
  udpServer = getservbyname(LOGGER_SERVICE, "udp");

  if (udpServer) {
    /* Could find the port of the service */
    portNumber = udpServer->s_port;
  }
  else {
    /* Problems... */
    return;
  }

  hp = gethostbyname(LOGGER_HOST);

  if (!hp) {
    return;
  }

  if (hp->h_addr_list[0]) {
    struct in_addr in;
    memcpy(&in.s_addr, hp->h_addr_list[0], sizeof (in.s_addr));
    hostIP = inet_ntoa(in);
  }
  else {
    return;
  }

  if (!hostIP) {
    /* Could not find the server to register. */
    return;
  }
  
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(hostIP);
  serv_addr.sin_port = portNumber;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <0) {
    /* Problems opening socket */
    return;
  }

  user = getenv("USER");

  /* Send the message to the server */

  /*
    "ProtVer Tool Rev User Project RunTime"

    # is used to separate the strings.
   */
  sprintf(serverMsg,
          "%d#%s#%s#%s#%s#%ld",
          LOGGER_PROTOCOL_VERSION,
          tool,
          revision,
          ((user != NULL) ? user : "Unknown"),
          ((project != NULL) ? project : "Standard"),
          runTime);

  result = sendto(sockfd, 
                  serverMsg,
                  strlen(serverMsg), 
                  0, 
                  (struct sockaddr*) &serv_addr, 
                  sizeof(serv_addr));

  close(sockfd);

  return;
}
