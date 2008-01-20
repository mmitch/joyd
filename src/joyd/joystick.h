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

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

/* BSD */

#ifdef __FakeBSDonLinuxForCompileTest__
struct joystick {
	int x, y, b1, b2;
};
#else
#include <machine/joystick.h>
#endif

#define js_event joystick

#else

/* Linux */
#include <linux/joystick.h>

#endif

/*
 * function prototypes
 */

void OpenJoystick();
void CloseJoystick();
void ActionLoop();
struct js_event WaitForJoystickEvent();
void ExecuteCommands(char type, unsigned int value, char action, unsigned long button_status, unsigned long axis_status);
void StoreJoystickEvent(struct js_event js,long int *const axis_normal,long int *const button_normal,long int *const button_shift);

#endif
