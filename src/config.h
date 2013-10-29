#ifndef _CONFIG_H
#define _CONFIG_H
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

/*  Edit below to suit your setup.*/

#ifdef  __cplusplus
extern "C" {
#endif 

/* HAVE_BOOL
   If you do not have the boolean type 'bool' defined,
   uncomment the line below.
*/
/*#define  HAVE_BOOL 1*/
  
/* LOGGER_DIR
   This variable points to a directory where the log files
   should be kept... (Should not end with a slash ('/')!)
*/
#define LOGGER_DIR "/tmp"
  
/* LOGGER_PROTOCOL_VERSION
   This is a numeric value to allow you to handle several
   protocols at the same time in your LoggerServer.
*/
#define LOGGER_PROTOCOL_VERSION 1 

/* LOGGER_SERVICE
   This is the name of your UDP logging service that should be
   located in the /etc/servicesn file.
*/
#define LOGGER_SERVICE          "YOUR_SERVICE_NAME"

/* LOGGER_HOST
   This is the hostname of where your LoggerServer is running.
   This macro is used in the client only.
*/
#define LOGGER_HOST             "YOUR_LOGGER_SERVER_NAME"

/* LOGGER_PORT
   This is the port number of your UDP logging service.
   This macro is used in the client only.
*/
#define LOGGER_PORT             11111

#ifdef  __cplusplus
}
#endif
#endif
