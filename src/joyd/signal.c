/*
 *    signal.c - signal handling routines
 *
 *    this file is part of:
 *
 *    joyd 0.2.1   ---   The Joystick Daemon
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
 *
 *  - changed #include "config.h" to #include "options.h"
 */

/*  joyd 0.0.3 2000-01-17
 *  joyd 0.0.4 2000-02-09
 *  joyd 0.0.5 2000-03-19
 *  joyd 0.0.6 2000-04-06
 *  joyd 0.0.7 2000-04-13
 *  joyd 0.2.0 2000-04-22
 *  joyd 0.2.1 2000-05-25
 *
 *  (no changes)
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/


/*
 * system include files
 */

#include <signal.h>
#include <stdio.h>

/*
 * my headers and definitions
 */

#include "joyd.h"
#include "signal.h"
#include "log.h"
#include "options.h"
#include "joystick.h"

/*
 * now to the functions
 */


void InstallSignalHandler()
/* This installs our custom signal handler */
{
	if( signal(SIGTERM, SignalHandler) == SIG_ERR) {
		Print(stderr,"exit: error: unable to attach signal handler to TERM",NOTHING);
		exit(2);
	}
	if (config.debug > 2) {
		Print(stdout,"signal handler attached to TERM",NOTHING);
	}

	if( signal(SIGABRT, SignalHandler) == SIG_ERR) {
		Print(stderr,"exit: error: unable to attach signal handler to ABRT",NOTHING);
		exit(2);
	}
	if (config.debug > 2) {
		Print(stdout,"signal handler attached to ABRT",NOTHING);
	}

	if( signal(SIGINT, SignalHandler) == SIG_ERR) {
		Print(stderr,"exit: error: unable to attach signal handler to INT",NOTHING);
		exit(2);
	}
	if (config.debug > 2) {
		Print(stdout,"signal handler attached to INT",NOTHING);
	}

	if( signal(SIGHUP, SignalHandler) == SIG_ERR) {
		Print(stderr,"exit: error: unable to attach signal handler to HUP",NOTHING);
		exit(2);
	}
	if (config.debug > 2) {
		Print(stdout,"signal handler attached to HUP",NOTHING);
	}

}


void SignalHandler(int sig)
/* react to signals */
{
	switch (sig) {
	case SIGTERM:
		Print(stdout,"caught a TERM signal, exiting...",NOTHING);
		signal(sig,SIG_DFL);
		raise(sig);
		break;
	case SIGABRT:
		Print(stdout,"caught a ABRT signal, exiting...",NOTHING);
		signal(sig,SIG_DFL);
		raise(sig);
		break;
	case SIGINT:
		Print(stdout,"caught a INT signal, exiting...",NOTHING);
		signal(sig,SIG_DFL);
		raise(sig);
		break;
	case SIGHUP:
		Print(stdout,"caught a HUP signal, rereading configuration file",NOTHING);
		CloseJoystick();
		ReadConfigFile();
		OpenJoystick();
		AddMissingCalibrations();
		if (signal(SIGHUP, SignalHandler) == SIG_ERR) {
			Print(stderr,"exit: error: unable to reattach signal handler to HUP",NOTHING);
			exit(2);
		}
		if (config.debug > 2) {
			Print(stdout,"signal handler reattached to HUP",NOTHING);
		}
		break;
	default:
		Print(stderr,"exit: error: signal handler never wanted to handle this signal",NOTHING);
		exit(2);
	}
}
