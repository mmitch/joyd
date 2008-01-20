/*
 *    log.c - logging functionality
 *
 *    this file is part of:
 *
 *    joyd 0.2.2   ---   The Joystick Daemon
 *
 *    2000 (C) by Christian Garbs <mitch@uni.de>
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*  joyd 0.0.1 2000-01-09
 *
 *  initial version
 */

/*  joyd 0.0.2 2000-01-10
 *  joyd 0.0.3 2000-01-17
 *  joyd 0.0.4 2000-02-09
 *  joyd 0.0.5 2000-03-19
 *  joyd 0.0.6 2000-04-06
 *  joyd 0.0.7 2000-04-13
 *  joyd 0.2.0 2000-04-22
 *
 *  (no changes)
 */

/*  joyd 0.2.1 2000-05-25
 *
 *  - config.stdout renamed to config.std_out
 */

/*  joyd 0.2.2 2000-10-11
 *
 *  (no changes)
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/


/*
 * system include files
 */

#include <stdio.h>
#include <syslog.h>

/*
 * my headers and definitions
 */

#include "joyd.h"
#include "log.h"

/*
 * now to the functions
 */


void Print(FILE *const fhandle,const char *const string1,const char *const string2)
/* This Logs a message to a file and syslog */
{
	if ((fhandle!=stdout) || (config.std_out)) {
		fprintf(fhandle,"%s%s\n",string1,string2);
		fflush(fhandle);
	}
	
	/* This might be slow, but I'm lazy: open, log, close - all in one place! */
	if (config.syslog) {
		openlog(PROGRAM_NAME, LOG_CONS | LOG_PID, SYSLOG_FACILITY);
		if (fhandle == stderr) {
			syslog(SYSLOG_PRIO_ERROR,"%s%s",string1,string2);
		} else {
			syslog(SYSLOG_PRIO_INFO,"%s%s",string1,string2);
		}
		closelog();
	}
}
