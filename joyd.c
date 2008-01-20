/*
 *    joyd.c
 *
 *    this is the main file of:
 *
 *    joyd 0.0.7   ---   The Joystick Daemon
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

/*
 * You can execute commands by using your joystick, joypad or whatever
 * is supported by the Linux Joystick Driver. This allows you to
 * reboot your computer when your keyboard is hung or you can even use
 * your system without any keyboard at all. 
 *
 * I use one of my computers (no screen, no keyboard) for listening to
 * mp3-files - with this program I can stop/start/skip songs, change
 * the volume, shutdown the computer and much more just by using an
 * joypad (in fact, it's an original SNES joypad).
 *
 * If you use this and find it useful, please let me know!
 * If you find a bug or have any suggestions or questions: tell me!
 *
 * My mail adress is:    <mitch@uni.de>
 * My homepage is here:  http://www.h.shuttle.de/mitch/
 *
 *
 *                                    Now enjoy this.
 *                                    Christian
 */

/*
 * This program is based on jstest.c Version 1.2 by Vojtech Pavlik,
 * which "is also intended to serve as an example implementation for
 * those who wish to learn how to write their own joystick using
 * applications."
 *
 * That's just what I did. That's GPL. Thanks!
 */


/*****************************************************************************
 *                                                                           *
 *     "Even the little things make me happy. It's that... that ordinary     *
 *      life I want back... I want back."                (Tsukino Usagi)     *
 *                                                                           *
 *****************************************************************************/


/*     These are just the changes to this file.
 *     See the HISTORY file for changes on the whole package.
 */


/*  joyd 0.0.1 2000-01-09
 *
 *  first version
 *  - added signal handlers
 *  - added syslog routines
 *  - added background mode
 */

/*  joyd 0.0.2 2000-01-10
 *
 *  - the global variable config is now declared here and declared
 *    external in joyd.h
 *  - "joyd -v" prints the version number
 */

/*  joyd 0.0.3 2000-01-17
 *
 *  - changed the JOYD_VERSION macro to PROGRAM_VERSION
 */

/*  joyd 0.0.4 2000-02-09
 *  joyd 0.0.5 2000-03-19
 *  joyd 0.0.6 2000-04-06
 *  joyd 0.0.7 2000-04-13
 *
 *  (no changes)
 */

/*
 *  2do:
 *  ? supply Sys V start/stop script (for /etc/rc.d/)
 *  ? implement shift_axes like shift_keys
 *  ? autoconf support
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/


/*
 * system include files
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * my headers and definitions
 */

#include "joyd.h"
#include "daemon.h"
#include "joystick.h"
#include "log.h"
#include "options.h"
#include "signal.h"
#include "string.h"

/*
 * the global variable
 */

TCONFIG config;

/*
 * now to the functions
 */



int main (int argc, char **argv)
/* simple main program */
{
	if ((argc > 1) && (strcmp(argv[1],PRINT_VERSION) == 0)) {
		printf(PROGRAM_VERSION "\n");
		exit(0);
	};

	SetDefaultValues();

	if (config.debug > 1) {
		Print(stdout,PROGRAM_VERSION," starting up");
	}

	InstallSignalHandler();

	if (argc > 1) {
		free(config.config_file);
		config.config_file = AllocString(argv[1]);
	}
	
	ReadConfigFile();

	if (config.daemon) {
		ForkToBackground();
	}
	
	OpenJoystick();

	AddMissingCalibrations();

	ActionLoop();

	CloseJoystick();

	Print(stdout,PROGRAM_VERSION," quits now (should've never come around here!?)");

	return 0;
}
