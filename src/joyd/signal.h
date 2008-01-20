/* signal.h
 *
 * part of
 * joyd 0.2.1 2000-05-25
 *
 */

#ifndef _SIGNAL_H_INCLUDED_
#define _SIGNAL_H_INCLUDED_
/*
 * function prototypes
 */

void InstallSignalHandler();
void SignalHandler(int sig);

#endif
