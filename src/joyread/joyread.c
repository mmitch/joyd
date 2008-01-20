/*
 *    joyread.c - get the current state of a joystick axis or button
 *
 *    this program is part of:
 *
 *    joyd 0.2.0   ---   The Joystick Daemon
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
 * This small piece of code is again based on:
 * jstest.c Version 1.2 - Copyright (c) 1996-1999 Vojtech Pavlik
 */

/*  joyd 0.0.6 2000-04-06
 *
 *  merged joyreadaxis.c and joyreadbutton.c into this file
 */

/*  joyd 0.0.7 2000-04-13
 *
 *  changed some C++ style comments to C style comments
 */

/*  joyd 0.2.0 2000-04-22
 *
 *  (no changes)
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <linux/joystick.h>

#define NAME_LENGTH 128

/*
 * << NOTE >>
 *
 * You must either compile this file with -DCHECK_AXIS or
 * -DCHECK_BUTTON to select whether to show the axis or button
 * status. This makes the code somewhat ugly but changes apply to
 * both programs at the same time.
 *
 */

#ifdef CHECK_AXIS
/* __axis__ */
#ifdef CHECK_BUTTON
/* axis && button */
#error You cannot use both -DCHECK_AXIS and -DCHECK_BUTTON
#else
/* axis && !button */
#define PROGRAM_NAME "joyreadaxis"
#endif
#else
/* __!axis__ */
#ifdef CHECK_BUTTON
/* !axis && button */
#define PROGRAM_NAME "joyreadbutton"
#else
/* !axis && !button */
#error You must either use -DCHECK_AXIS or -DCHECK_BUTTON
#endif
#endif


int main (int argc, char **argv)
{
	int fd;
	unsigned char axes;
	unsigned char buttons;

	int i;
	struct js_event js;

	if (argc != 3 ) {
		fprintf(stderr,"\nUsage: " PROGRAM_NAME " <device> <axis>\n\n");
		exit(1);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror(PROGRAM_NAME);
		exit(1);
	}

	ioctl(fd, JSIOCGAXES, &axes);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	
	/* Just read the JS_EVENT_INIT events to see what the current state
           of the joystick is, then exit */

	for (i=0;i<axes+buttons;i++) {
		if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
			perror("\n"PROGRAM_NAME": error reading");
			exit (1);
		}
		
		switch(js.type & ~JS_EVENT_INIT) {
		case JS_EVENT_BUTTON:
#ifdef CHECK_BUTTON
			if (js.number == atoi(argv[2])) {
				printf("%d\n",js.value);
			}
#endif
			break;
		case JS_EVENT_AXIS:
#ifdef CHECK_AXIS
			if (js.number == atoi(argv[2])) {
				printf("%d\n",js.value);
			}
#endif
			break;
		}
	}
	
	return 0;
}


