/*
 *    joystick.c - joystick routines
 *
 *    this file is part of:
 *
 *    joyd 0.0.2   ---   The Joystick Daemon
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
 *  - the ActionLoop() function got split up into smaller parts
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
	char buffer[256]; /* should be long enough, we're just showing integer values */

        if ((config.fd = open(config.joy_device, O_RDONLY)) < 0) {
		perror("joyd");
		exit(1);
	}

   	ioctl(config.fd, JSIOCGVERSION, &config.version);
	ioctl(config.fd, JSIOCGAXES, &config.axes);
	ioctl(config.fd, JSIOCGBUTTONS, &config.buttons);
	ioctl(config.fd, JSIOCGNAME(NAME_LENGTH), config.joy_name);


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
		perror("joyd");
		exit(1);
	}
}


struct js_event WaitForJoystickEvent()
/* Wait for joystick event */
{
	struct js_event js;

	if (read(config.fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
		perror("\njoyd: error reading");
		exit (1);
	}
	return js;
}


void ExecuteCommands(long int axis_max,long int button_max)
/* check which actions match and execute them */
{
	char buffer[256]; /* should be enough for three long integers */
	int i;
	int rc;

	for (i=0;i<config.action_count;i++) {
		if ((config.action[i].axes==axis_max) && (config.action[i].buttons==button_max)) {
			/* first check for internal commands */
			if (strcmp(config.action[i].command,config.action_exit) == 0) {
				if (config.debug > 0) {
					Print(stdout,"executing internal command: exit",NOTHING);
				}
				raise(SIGTERM);
			} else if (strcmp(config.action[i].command,config.action_reread) == 0) {
				if (config.debug > 0) {
					Print(stdout,"executing internal command: reread configuration",NOTHING);
				}
				raise(SIGHUP);
			} else {
				if (config.debug > 0) {
					Print(stdout,"executing external command: ",config.action[i].command);
				}
				rc=system(config.action[i].command);
				if (config.debug > 1) {
					sprintf(buffer,"%i",rc);
					Print(stdout,"return code was: ",buffer);
				}
			}
		}
	}
}


void StoreJoystickEvent(struct js_event js,long int *const axis_normal,long int *const button_normal,long int *const button_shift)
/* Store the joystick event */
{
	switch(js.type & ~JS_EVENT_INIT) {
	case JS_EVENT_BUTTON:
		/* shift or normal button? */
		if ((1 << js.number) & config.shiftkeys) {
			/* its a shift button */
			/* first mask the button bit out */
			*button_shift=(~((unsigned long) (1 << js.number))) & *button_shift;
			/* then add the button status */
			*button_shift=*button_shift | ((unsigned long) (js.value <<js.number));
		} else {
			/* its a normal button */
			/* first mask the button bit out */
			*button_normal=(~((unsigned long) (1 << js.number))) & *button_normal;
			/* then add the button status */
			*button_normal=*button_normal | ((unsigned long) (js.value <<js.number));
		}
		break;
	case JS_EVENT_AXIS:
		/* first mask both axis bits out */
		*axis_normal=(~((unsigned long) (1 << (js.number*2)))) & *axis_normal;
		*axis_normal=(~((unsigned long) (1 << ((js.number*2)+1)))) & *axis_normal;
		/* now see whether its a high or a low value and set the bits */
		if (js.value < config.cal[js.number].min) {
			/* low value, low bit */
			*axis_normal=*axis_normal | ((unsigned long) (1 << (js.number*2)));
		}
		if (js.value > config.cal[js.number].max) {
			/* high value, high bit */
			*axis_normal=*axis_normal | ((unsigned long) (1 << ((js.number*2+1))));
		}
		break;
	}
}

	
void ActionLoop()
/* Here we listen to the joystick and start the actions */
{
	unsigned long axis_max=0;
	unsigned long axis_normal=0;
	unsigned long button_max=0;
	unsigned long button_normal=0;
	unsigned long button_shift=0;
	struct js_event js;
	char buffer[256]; /* should be enough for three long integers */
	
	Print(stdout,JOYD_VERSION," is running");
	
	while (1) {

		js=WaitForJoystickEvent();

		StoreJoystickEvent(js,&axis_normal,&button_normal,&button_shift);

		/* if a new button is pressed, remember this */
		button_max = button_max | button_normal;
		axis_max=axis_max | axis_normal;
		
		if (config.debug > 2) {
			sprintf(buffer,"axis_normal: %li button_normal: %li button_shift: %li",axis_normal,button_normal,button_shift);
			Print(stdout,"joy status: ",buffer);
		}

		/* execute commands only when all normal buttons/axes are released
		   and one of them was pressed before */
		if ((button_normal == 0) && (axis_normal == 0)
		    && ((button_max > 0) || (axis_max > 0))) {
			/* add the status of the shift buttons */
			button_max=button_max | button_shift;

			ExecuteCommands(axis_max,button_max);
			
			button_max=0;
			axis_max=0;
		}
	}
	
}
