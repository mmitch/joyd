/*
 *    joystick.c - joystick routines
 *
 *    this file is part of:
 *
 *    joyd 0.2.2   ---   The Joystick Daemon
 *
 *    2000,2001 (C) by Christian Garbs <mitch@uni.de>
 *                     Vincent Bernat <bernat@free.fr>
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
 *  - the ActionLoop() function got split up into smaller parts
 */

/*  joyd 0.0.3 2000-01-17
 *
 *  - replaced the string "joyd" by the macro PROGRAM_NAME
 *  - replaced the macro JOYD_VERSION by PROGRAM_VERSION
 */

/*  joyd 0.0.4 2000-02-09
 *
 *  (no changes)
 */

/*  joyd 0.0.5 2000-03-19
 *
 *  - axis values are compared with "<=" and ">=" instead of "<" and ">"
 */

/*  joyd 0.0.6 2000-04-06
 *  joyd 0.0.7 2000-04-13
 *  joyd 0.2.0 2000-04-22
 *  joyd 0.2.1 2000-05-25
 *  joyd 0.2.2 2000-10-11
 *
 *  (no changes)
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/


/*
 * system include files
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <string.h>
#endif

/*
 * my headers and definitions
 */

#include "joyd.h"
#include "joystick.h"
#include "log.h"

/*
 * now to the functions
 */




void OpenJoystick()
/* This initializes the joystick */
{
	char buffer[256]; /* should be long enough, we're just showing
			     integer values */

	if ((config.fd = open(config.joy_device, O_RDONLY)) < 0) {
		perror(PROGRAM_NAME);
		exit(1);
	}

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	/* BSD does not support different joysticks, there are always
	   2 axes and 2 buttons */
	config.version = 1;
	config.axes    = 2;
	config.buttons = 2;
	strcpy(config.joy_name,"Standard BSD Joystick");
#else
	ioctl(config.fd, JSIOCGVERSION, &config.version);
	ioctl(config.fd, JSIOCGAXES, &config.axes);
	ioctl(config.fd, JSIOCGBUTTONS, &config.buttons);
	ioctl(config.fd, JSIOCGNAME(NAME_LENGTH), config.joy_name);
#endif

	if (config.debug > 1) {
		sprintf(buffer,"axes: %d, buttons: %d, driver: %d.%d.%d",
			config.axes, config.buttons, config.version >> 16,
			(config.version >> 8) & 0xff, config.version & 0xff);

		Print(stdout,"joystick detected: ",config.joy_name);
		Print(stdout,buffer,NOTHING);
	}
}


void CloseJoystick()
/* This initializes the joystick */
{
	if (close(config.fd) != 0) {
		perror(PROGRAM_NAME);
		exit(1);
	}
}


struct js_event WaitForJoystickEvent()
/* Wait for joystick event */
{
	struct js_event js;

	if (read(config.fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
		perror(PROGRAM_NAME ": error reading");
		exit (1);
	}

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	/* On BSD the previous read() is non-blocking. We don't
	   want to consume 100% CPU time so we wait a bit until the
	   next read */
	usleep(TIMEOUT_JS);
#endif

	return js;
}


void ExecuteCommands(char type, unsigned int value, char action, unsigned long button_status,unsigned long axis_status)
/* check which actions match and execute them */
{
	char buffer[256]; /* should be enough for three long integers */
	TACTION a;
	int i;
	int rc;

	value = 1 << value;

	for (i=0;i<config.action_count;i++) {
		a = config.action[i];
		if (a.type == type && a.value == value && a.action == action &&
		    ((a.buttons_yes & (unsigned int) button_status) == a.buttons_yes) &&
		    ((a.buttons_no  & (unsigned int) button_status) == 0) &&
		    ((a.axes_yes    & (unsigned int) axis_status)   == a.axes_yes) &&
		    ((a.axes_no     & (unsigned int) axis_status)   == 0)) {
			/* first check for internal commands */
			if (strcmp(a.command, config.action_exit) == 0) {
				if (config.debug > 0) {
					Print(stdout,"executing internal command: exit",NOTHING);
				}
				raise(SIGTERM);
			} else if (strcmp(a.command, config.action_reread) == 0) {
				if (config.debug > 0) {
					Print(stdout,"executing internal command: reread configuration",NOTHING);
				}
				raise(SIGHUP);
			} else {
				if (config.debug > 0) {
					Print(stdout,"executing external command: ",config.action[i].command);
				}
				rc=system(a.command);
				if (config.debug > 1) {
					sprintf(buffer,"%i",rc);
					Print(stdout,"return code was: ",buffer);
				}
			}
		}
	}
}


void ParseJoystickEvent(struct js_event js, long int *const axis_status, long int *const button_status)
/* Store a joystick event */
{
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

	/* BSD event handling */

	int js_number;
	int js_value;
   
	/* With BSD model of event, many buttons can be pressed at the
	   same time */

	/* check button 0 */
	js_number = 0;
	if (((unsigned long) js.b1 ^ (unsigned long) button_status) & (1 << js_number)) {
		js_value = js.b1;
		/* first mask the button bit out */
		*button_status=(~((unsigned long) (1 << js_number))) & *button_status;
		/* then add the button status */
		*button_status=*button_status | ((unsigned long) (js_value <<js_number));

		/* check for commands to be executed */
		ExecuteCommands('B', js_number, (js_value) ? '+' : '-', *button_status, *axis_status);
	}

	/* check button 1 */
	js_number = 1;
	if (((unsigned long) js.b2 ^ (unsigned long) button_status) & (1 << js_number)) {
		js_value = js.b2;
		/* first mask the button bit out */
		*button_status=(~((unsigned long) (1 << js_number))) & *button_status;
		/* then add the button status */
		*button_status=*button_status | ((unsigned long) (js_value <<js_number));

		/* check for commands to be executed */
		ExecuteCommands('B', js_number, (js_value) ? '+' : '-', *button_status, *axis_status);
	}

	/* check movement on X axis */
	js_number = 0;
	if ((js.x <= config.cal[js_number].min) && ((1 << (js_number*2)) & *axis_status) == 0) {
		/* Movement to the left */
		*axis_status = *axis_status | (1 << (js_number*2));
		*axis_status=(~((unsigned long) (1 << js_number*2+1))) & *axis_status;
		ExecuteCommands('A', js_number*2, '+', *button_status, *axis_status);
	} else if ((js.x >= config.cal[js_number].max) && ((1 << (js_number*2+1)) & *axis_status) == 0) {
		/* Movement to the right */
		*axis_status = *axis_status | (1 << (js_number*2+1));
		*axis_status=(~((unsigned long) (1 << js_number*2))) & *axis_status;
		ExecuteCommands('A', js_number*2+1, '+', *button_status, *axis_status);
	} else if ((1 << (js_number*2)) & *axis_status) {
		/* Joystick centered (coming back from left) */
		*axis_status=(~((unsigned long) (1 << js_number*2))) & *axis_status;
		*axis_status=(~((unsigned long) (1 << js_number*2+1))) & *axis_status;
		ExecuteCommands('A', js_number*2, '-', *button_status, *axis_status);
	} else if ((1 << (js_number*2+1)) & *axis_status) {
		/* Joystick centered (coming back from right) */
		*axis_status=(~((unsigned long) (1 << js_number*2))) & *axis_status;
		*axis_status=(~((unsigned long) (1 << js_number*2+1))) & *axis_status;
		ExecuteCommands('A', js_number*2+1, '-', *button_status, *axis_status);
	} else {
		/* No changes on X axis */
	}
    
	/* check movement on Y axis */
	js_number = 1;
	if ((js.y <= config.cal[js_number].min) && ((1 << (js_number*2)) & *axis_status) == 0) {
		/* Movement to the top */
		/* set high bit, unset low bit */
		*axis_status = *axis_status | (1 << (js_number*2+1));
		*axis_status=(~((unsigned long) (1 << js_number*2))) & *axis_status;
		ExecuteCommands('A', js_number*2+1, '-', *button_status, *axis_status);
	} else if ((js.x >= config.cal[js_number].max) && ((1 << (js_number*2+1)) & *axis_status) == 0) {
		/* Movement to the bottom */
		/* set low bit, unset high bit */
		*axis_status = *axis_status | (1 << (js_number*2));
		*axis_status=(~((unsigned long) (1 << js_number*2+1))) & *axis_status;
		ExecuteCommands('A', js_number*2, '+', *button_status, *axis_status);
	} else if ((1 << (js_number*2)) & *axis_status) {
		/* Joystick centered (coming back from bottom) */
		/* unset low+high bit */
		*axis_status=(~((unsigned long) (1 << js_number*2))) & *axis_status;
		*axis_status=(~((unsigned long) (1 << js_number*2+1))) & *axis_status;
		ExecuteCommands('A', js_number*2, '-', *button_status, *axis_status);
	} else if ((1 << (js_number*2+1)) & *axis_status) {
		/* Joystick centered (coming back from top) */
		/* unset low+high bit: */
		*axis_status=(~((unsigned long) (1 << js_number*2))) & *axis_status;
		*axis_status=(~((unsigned long) (1 << js_number*2+1))) & *axis_status;
		ExecuteCommands('A', js_number*2+1, '-', *button_status, *axis_status);
	} else {
		/* No changes on Y axis */
	}
    
#else

	unsigned long axis_status_old;
	char buffer[256];

	/* Linux event handling */

	switch(js.type & ~JS_EVENT_INIT) {
	case JS_EVENT_BUTTON:

		/* first mask the button bit out */
		*button_status=(~((unsigned long) (1 << js.number))) & *button_status;
		/* then add the button status */
		*button_status=*button_status | ((unsigned long) (js.value <<js.number));

		/* check for commands to be executed */
		ExecuteCommands('B', js.number, (js.value <<js.number) ? '+' : '-', *button_status, *axis_status);

		break;

	case JS_EVENT_AXIS:

		/* remember the old status */
		axis_status_old = * axis_status;
		/* first mask both axis bits out */
		*axis_status=(~((unsigned long) (1 << (js.number*2)))) & *axis_status;
		*axis_status=(~((unsigned long) (1 << ((js.number*2)+1)))) & *axis_status;
		/* now see whether its a high or a low value and set the bits */
		if (js.value <= config.cal[js.number].min) {
			/* low value, low bit */
			*axis_status=*axis_status | ((unsigned long) (1 << (js.number*2)));
		}
		if (js.value >= config.cal[js.number].max) {
			/* high value, high bit */
			*axis_status=*axis_status | ((unsigned long) (1 << ((js.number*2+1))));
		}

		/* find out what has changed and execute the corresponding command(s) */
		if ((*axis_status ^ axis_status_old) & (1 << (js.number*2))) {
			/* the low bit has changed */
			ExecuteCommands('A', js.number*2, (*axis_status & (1 <<js.number*2)) ? '+' : '-', *button_status, *axis_status);
		}
		if ((*axis_status ^ axis_status_old) & (1 << (js.number*2+1))) {
			/* the high bit has changed */
			ExecuteCommands('A', js.number*2+1, (*axis_status & (1 <<js.number*2+1)) ? '+' : '-', *button_status, *axis_status);
		}

		break;
	}
#endif
}

	
void ActionLoop()
/* Here we listen to the joystick and start the actions */
{
	unsigned long axis_status=0;
	unsigned long button_status=0;
	struct js_event js;
	char buffer[256]; /* should be enough for two long integers */
	
	Print(stdout, PROGRAM_VERSION, " is running");
	
	while (1) {

		js = WaitForJoystickEvent();

		ParseJoystickEvent(js, &axis_status, &button_status);

		if (config.debug > 2) {
			sprintf(buffer, "axes=%li buttons=%li", axis_status, button_status);
			Print(stdout, "joy status: ", buffer);
		}

	}
	
}
