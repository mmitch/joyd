/* signal.h
 *
 * part of
 * joyd 0.2.2 2000-10-11
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
