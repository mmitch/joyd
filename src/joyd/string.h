/* string.h
 *
 * part of
 * joyd 0.2.2 2000-10-11
 *
 */

#ifndef _STRING_H_INCLUDED_
#define _STRING_H_INCLUDED_

/*
 * function prototypes
 */

char *GetLine(FILE *const fhandle);
char *Skip(const char *const input);
void Split(char *const input,char **begin,char **end);
char *AllocString(const char *const string_constant);

#endif
