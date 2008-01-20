#ifndef PARSE_HOME_INCLUDED

/*
// $Id: parse_home.h,v 1.2 2001/01/08 19:01:18 mitch Exp $
//
// 1999 (C) by Christian Garbs <mitch@uni.de>
// This file is licensed under GNU GPL.
//
// Get the newest version at http://www.cgarbs.de/header.en.html
//
// ------------------------------------------------
//
// This function expands the $HOME-directory within a string.
// Example: "~/my_file" becomes "/home/foo/my_file" when the
//          current user is "foo"
//
// You might want to change the behaviour when the shell
// variable $HOME is not defined:
//
// return the original string:
// #define	PARSE_HOME_ERROR	0
//
// return the original string without the "~":
// #define	PARSE_HOME_ERROR	1
//
// return a NULL pointer:
// #define	PARSE_HOME_ERROR	2
//
// print an error message and exit the program:
// #define	PARSE_HOME_ERROR	3
//
// execute your own error handling routine: (insert your code!)
// #define	PARSE_HOME_ERROR	4
//
// Choose one of the possibilities from above:
*/
#define         PARSE_HOME_ERROR        0
/*
//
// ------------------------------------------------
*/

#include <stdio.h>
#include <string.h>

char *parse_home(char *pch_old)

/*
// Expands ~/ to $HOME/
//
// INPUT:
// The address of the string that is to be expanded
//
// OUTPUT:
// The address of the expanded string
// This string is allocated dynamically, you should
// free the used memory with free()
*/

{
	char ch_found=0;
	char *pch_new=NULL;
	char *pch_home=NULL;
	
	if (strlen(pch_old)>0) {
		if ((pch_old[0]=='~') && (pch_old[1]=='/')) {
			ch_found=1;
		};
	};
	
	if (ch_found) {
		pch_old++;
		pch_home=(char *) getenv("HOME");
		if (pch_home!=NULL) {
			pch_new=(char *) malloc(strlen(pch_home)+strlen(pch_old)+1);
			strcpy(pch_new,pch_home);
			strcat(pch_new,pch_old);
			
		} else {
			#if PARSE_HOME_ERROR == 0
			pch_old--;
			pch_new=(char *) malloc(strlen(pch_old)+1);
			strcpy(pch_new,pch_old);
			#endif
			
			#if PARSE_HOME_ERROR == 1
			pch_new=(char *) malloc(strlen(pch_old)+1);
			strcpy(pch_new,pch_old);
			#endif
			
			#if PARSE_HOME_ERROR == 2
			#endif
			
			#if PARSE_HOME_ERROR == 3
			fprintf(stderr,"\nERROR: Shell variable $HOME undefined.\n");
			fprintf(stderr,"       Could not expand \"~%s\"\n",pch_old);
			exit(1);
			#endif
			
			#if PARSE_HOME_ERROR == 4
			// INSERT YOUT CODE HERE
			
			//
			#endif
		};
	} else {
		pch_new=(char *) malloc(strlen(pch_old)+1);
		strcpy(pch_new,pch_old);
	};

	return pch_new;
};

#endif
