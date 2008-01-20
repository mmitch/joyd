/* joystick.h
 *
 * part of
 * joyd 0.2.2 2000-10-11
 *
 */

#ifndef _JOYSTICK_H_INCLUDED_
#define _JOYSTICK_H_INCLUDED_

/*
 * import the js_event structure
 */


/* machine dependent includes */

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

/* BSD */
#include <machine/joystick.h>
#define js_event joystick
#define _BSD_CODE_


#elif defined(__FakeBSDonLinuxForCompileTest__)

/* BSD on Linux (debugging only) */
#warning building BSD on Linux: this will not work!
#include <linux/joystick.h>
struct joystick {
	int x, y, b1, b2;
};
#define _BSD_CODE_


#else

/* Linux */
#include <linux/joystick.h>
#define _LINUX_CODE_


#endif

/* end of machine dependent includes */


/*
 * function prototypes
 */

void OpenJoystick();
void CloseJoystick();
void ActionLoop();
struct js_event WaitForJoystickEvent();
void ExecuteCommands(char type, unsigned int value, char action, unsigned long button_status, unsigned long axis_status);

#ifdef _BSD_CODE_
void ParseJoystickEvent_BSD(struct js_event js,long int *const axis_normal,long int *const button_normal);
#endif

#ifdef _LINUX_CODE_
void ParseJoystickEvent_Linux(struct js_event js,long int *const axis_normal,long int *const button_normal);
#endif

#endif
