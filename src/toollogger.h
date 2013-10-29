#ifndef _TOOLLOGGER_H
#define _TOOLLOGGER_H
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


/** Description: 
    This file is part of the client library.
    It contains the declaration of the function to use from 
    your program whenever the tool is started or stopped.
    
    Example of Usage:
    
    When the program is started:
    
    logToolUsage( "MyCoolProg", "0.1", "BetaTest", true );
    
    Before the program exits:
    
    logToolUsage( "MyCoolProg", "0.1", "BetaTest", false );
*/

/* INCLUDES */

#include "config.h"

#ifdef  __cplusplus
extern "C" {
#endif
#if 0
}
#endif

#ifndef HAVE_BOOL
/** Used in case system does not have a boolean type. */
typedef enum {
  true = 1,
  false = 0
} bool;
#endif

/** Use this function in your program to report to the
    logger server that you have either started or stopped
    execution.
    @param tool     This is the name of your program.
    @param revision This is the version string of your program.
    @param project  If you release your program to different customers
                    or release it from different parallel projects it
                    can be a good idea to differentiate between them.
                    Note: This flag can be NULL.
    @param flag     If your program is started, pass true, else pass
                    false.
*/
void 
logToolUsage(char* tool, char* revision, char* project, bool flag);

#ifdef  __cplusplus
}
#endif


#endif /* toollogger.h */
