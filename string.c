/*
 *    string.c - string routines
 *
 *    this file is part of:
 *
 *    joyd 0.0.5   ---   The Joystick Daemon
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
 *  joyd 0.0.3 2000-01-17
 *  joyd 0.0.4 2000-02-09
 *  joyd 0.0.5 2000-03-19
 *
 *  (no changes)
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/


/*
 * system include files
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
 * my headers and definitions
 */

#include "joyd.h"
#include "string.h"

/*
 * now to the functions
 */


char *GetLine(FILE *const fhandle)
/* This reads a line from a given file */
{
	char *tmp,*tmp2;
	long i=0;
	char c;
	
	tmp=(char *) calloc(1,sizeof(char));

	while (((c=fgetc(fhandle)) != 10) && (c != EOF)) {
		tmp2=tmp;
		tmp=(char *) calloc(i+2,sizeof(char));
		strcpy(tmp,tmp2);
		free(tmp2);
		tmp[i]=c;
		i++;
		tmp[i]='\0';
	}

	if (c == EOF) {
		*tmp = EOF;
	}
	
	return tmp;
}


char *Skip(const char *const input)
/* Skips the next whitespace characters in a string */
{
	char *cursor;

	cursor=(char *const)input;

	if (cursor != NULL) {
		while (isspace(*cursor)) {
			cursor++;
		}
	}		
	
	return cursor;
}


char *AllocString(const char *const string_constant)
/* This allocates a dynamic copy of a constant string */
{
	char *tmp;

	tmp=(char*) calloc(strlen(string_constant)+1,sizeof(char));
	strcpy(tmp,string_constant);
	
	return(tmp);
}


void Split(char *const input,char **begin,char **end)
/* This splits a string into two substrings at the first whitespace(s) */
{
	char *cursor;

	if (input == NULL) {
		*begin=NULL;
		*end=NULL;
	} else {
		/* find splitpoint */
		for (cursor=input;(*cursor!='\0') && (!isspace(*cursor));cursor++);
		
		/* copy left part to begin */
		if (cursor-input > 0) {
			*begin=(char*)calloc(cursor-input+1,sizeof(char));
			strncpy(*begin,input,cursor-input);
		} else {
			*begin=NULL;
		}
		
		/* find the first non-whitespace character */
		cursor=Skip(cursor);
		
		/* copy right part to end */
		if (*cursor != '\0') {
			*end=(char*)calloc(strlen(cursor)+1,sizeof(char));
			strcpy(*end,cursor);
		} else {
			*end=NULL;
		}
	}
}
