/* joystick.h
 *
 * part of
 * joyd 0.0.5 2000-03-19
 *
 */

#ifndef _JOYSTICK_H_INCLUDED_
#define _JOYSTICK_H_INCLUDED_

/*
 * import the js_event structure
 */

#include <linux/joystick.h>

/*
 * function prototypes
 */

void OpenJoystick();
void CloseJoystick();
void ActionLoop();
struct js_event WaitForJoystickEvent();
void ExecuteCommands(long int axis_max,long int button_max);
void StoreJoystickEvent(struct js_event js,long int *const axis_normal,long int *const button_normal,long int *const button_shift);

#endif
