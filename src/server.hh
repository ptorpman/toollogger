#ifndef _LOGGERSERVER_HH
#define _LOGGERSERVER_HH
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

#include <poll.h>
#include "config.h"

/* MACROS */

#define SERVER_VERSION "0.1"

/* TYPES */

/* ENUMERATIONS */

/* STRUCTS */

/* CONSTANTS */

/* GLOBAL VARIABLES */

/* FUNCTION DECLARATIONS */

class LoggerServer 
{
  public:
    
    LoggerServer(char*, char*);
    ~LoggerServer();

    void PollSocket( );

  protected:

  private:

    // Attributes
    char * serviceName;
    char * loggerDir;
    struct pollfd mySocket[1];
    int    serviceSocket;
    char   currentDate[256];

    // Internal functions
    int  OpenBigBrotherSocket( );
    void HandleBigBrotherMessage( char * message, char * host );
    void SetCurrentDate( );
    char *GetCurrentDate( );
    void CheckDate();
    void UpdateLogFile( char * logString );
    void longToTime( unsigned long timeSec, char * timeStr );

};

#endif
