/*
 *    options.c - joyd configuration
 *
 *    this file is part of:
 *
 *    joyd 0.2.2   ---   The Joystick Daemon
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
 *  - renamed this file to options.c
 */

/*  joyd 0.0.3 2000-01-17
 *  joyd 0.0.4 2000-02-09
 *  joyd 0.0.5 2000-03-19
 *
 *  (no changes)
 */

/*  joyd 0.0.6 2000-04-06
 *
 *  - type cast of GetLine() to signed char (otherwise won't compile on PPC)
 */

/*  joyd 0.0.7 2000-04-13
 *
 *  - type cast of GetLine() removed, EOF is cast to char instead
 */

/*  joyd 0.2.0 2000-04-22
 *
 *  (no changes)
 */

/*  joyd 0.2.1 2000-05-25
 *
 *  - config.stdout renamed to config.std_out
 */

/*  joyd 0.2.2 2000-10-11
 *
 *  (no changes)
 */

/*****************************************************************************
 * now goes it loose...                                                      *
 *****************************************************************************/


/*
 * system include files
 */

#include <stdlib.h>
#include <stdio.h>

/*
 * my headers and definitions
 */

#include "joyd.h"
#include "parse_home.h"
#include "options.h"
#include "string.h"
#include "log.h"

/*
 * now to the functions
 */



void SetDefaultValues()
/* This uses the defaults from joyd.h */
{
	config.debug = SHOW_DEBUG;
	config.syslog = SHOW_SYSLOG;
	config.std_out = SHOW_STDOUT;
	config.config_file = AllocString(CONFIG_FILE);
	config.daemon = DAEMON_MODE;
	
	config.axes=0;
	config.buttons=0;
	config.version=0;
	config.joy_device=AllocString(JOY_DEVICE);
	config.joy_name[0]='\0';

	config.action_exit=AllocString(ACTION_EXIT);
	config.action_reread=AllocString(ACTION_REREAD);
	config.action=NULL;
	config.action_count=0;

	config.cal_count=0;
	config.cal=NULL;
	config.cal_min_default=CALIBRATION_MIN;
	config.cal_max_default=CALIBRATION_MAX;
}


void AddMissingCalibrations()
/* Set the default calibration for axis that don't have one already */
{
	if (config.axes > config.cal_count) { /* add some more */
		if (config.debug > 0) {
			Print(stdout,"more axes than calibrations, using defaults",NOTHING);
		}
		while (config.cal_count < config.axes) {
			AddCalibration(config.cal_min_default,config.cal_max_default);
		}
	} else if (config.axes < config.cal_count) { /* remove some */
		if (config.debug > 0) {
			Print(stdout,"more calibrations than axes, cleaning up",NOTHING);
		}
		config.cal_count=config.axes;
		config.cal=(TCALIBRATION*) realloc(config.cal,config.cal_count * sizeof(TCALIBRATION));
		if (config.cal == NULL) {
			Print(stderr,"exit: error: could allocate calibration memory",NOTHING);
			exit(1);
		}
	}
}


void AddCalibration(int min,int max)
/* This stores the calibration values for an axis */
{
	char buffer[200]; /* should be sufficient for three integer values */

	config.cal_count++;
	config.cal=(TCALIBRATION*) realloc(config.cal,config.cal_count * sizeof(TCALIBRATION));
	
	if (config.cal == NULL) {
		Print(stderr,"exit: error: could allocate calibration memory",NOTHING);
		exit(1);
	}

	config.cal[config.cal_count-1].min=min;
	config.cal[config.cal_count-1].max=max;

	if (config.debug > 1) {
		sprintf(buffer,"%i is %i , %i",config.cal_count,min,max);
		Print(stdout,"calibration for axis ",buffer);
	}
	if (config.debug > 0) {
		if ((min == 0) || (max == 0)) {
			sprintf(buffer,"%i",config.cal_count);
			Print(stdout,"warning: zero calibration value in axis ",buffer);
		}
		if (min >= max) {
			Print(stdout,"warning: min value is not less than max value",NOTHING);
		}
	}
}


void ClearActions()
/* This removes all the actions */
{
	while(config.action_count > 0) {
		config.action_count--;
		free(config.action[config.action_count].command);
		config.action=(TACTION*) realloc(config.action,config.action_count * sizeof(TACTION));
	}
}


void AddAction(char type, unsigned int value, char action, unsigned int buttons_yes, unsigned int buttons_no, unsigned int axes_yes, unsigned int axes_no, char * command)
/* This stores an action */
{
	char buffer[200]; /* should be sufficient for two long integer values */
	char *temp;

	config.action_count++;
	config.action=(TACTION*) realloc(config.action,config.action_count * sizeof(TACTION));
	
	if (config.action == NULL) {
		Print(stderr,"exit: error: could allocate action memory",NOTHING);
		exit(1);
	}

	/* copy char values */
	if ((type == 'B') || (type == 'b')) {
		config.action[config.action_count-1].type='B';
	} else {
		config.action[config.action_count-1].type='A';
	}
	if (action == '+') {
		config.action[config.action_count-1].action='+';
	} else {
		config.action[config.action_count-1].action='-';
	}

	/* copy unsigned int values */
	config.action[config.action_count-1].value=value;
	config.action[config.action_count-1].buttons_yes=buttons_yes;
	config.action[config.action_count-1].buttons_no=buttons_no;
	config.action[config.action_count-1].axes_yes=axes_yes;
	config.action[config.action_count-1].axes_no=axes_no;

	/* copy char* values */
	config.action[config.action_count-1].command=AllocString(command);

	if (config.debug > 1) {
		if (type == 'B') {
			sprintf(buffer,"%c button %i (%i %i %i %i) bound to ",action, value, buttons_yes, buttons_no, axes_yes, axes_no);
		} else {
			sprintf(buffer,"%c axis %i (%i %i %i %i) bound to ",action, value, buttons_yes, buttons_no, axes_yes, axes_no);
		}
		Print(stdout,buffer,command);
	}
	if (config.debug > 0) {
		if (value == 0) {
			Print(stdout,"warning: zero value",NOTHING);
		}
		if (value < 0) {
			Print(stdout,"warning: negative value",NOTHING);
		}
		if (buttons_yes < 0) {
			Print(stdout,"warning: negative buttons_yes value",NOTHING);
		}
		if (buttons_no < 0) {
			Print(stdout,"warning: negative buttons_yes value",NOTHING);
		}
		if (axes_yes < 0) {
			Print(stdout,"warning: negative axes_yes value",NOTHING);
		}
		if (axes_no < 0) {
			Print(stdout,"warning: negative axes_no value",NOTHING);
		}
		temp=Skip(command);
		if (*temp == '\0') {
			Print(stdout,"warning: no command given",NOTHING);
		}
	}
}


void ReadConfigFile()
/* Read a configuration file */
{
	FILE *fhandle;
	short int eof;
	char *line, *cursor;
	int section;

	char *temp;
	
	/* Expand ~/ to home directory */
	temp = config.config_file;
	config.config_file = parse_home(temp);
	if ((config.debug > 1) && (strcmp(temp, config.config_file) != 0)) {
		Print(stdout, "~/ expanded to: ", config.config_file);
	}
	free(temp);

	if (config.debug > 1) {
		Print(stdout,"reading configuration file: ",config.config_file);
	}

	if (strcmp(config.config_file, CONFIG_FROM_STDIN) == 0) {
		fhandle = stdin;
	} else {
		if (! (fhandle = fopen(config.config_file,"r"))) {
			Print(stderr,"exit: error: could not open configuration file: ",config.config_file);
			exit(1);
		}
	}
	  
	config.cal_count=0;
	ClearActions();

	eof=0;
	section=0;
	while (!eof) {
		line=GetLine(fhandle);
		if (*line == (char) EOF) {
			eof=1;
		} else {
			cursor=Skip(line);
			if ((*cursor != COMMENT_SIGN) && (*cursor != '\0')) {
				ParseLine(cursor,&section);
			}
		}
		free(line);
	}

	if (fclose(fhandle)) {
		Print(stderr,"exit: error: could not close configuration file: ",config.config_file);
		exit(1);
	}
}


void ParseCalibrationLine(char * const command)
/* This parses a line from the [calibration] section */
{
	char *min;
	char *max;

	Split(command,&min,&max);

	AddCalibration(atoi(min),atoi(max));

	free(min);
	free(max);
}


void ParseActionLine(char * const command)
/* This parses a line from the [action] section */
{
	char *type, *value, *action;
	char *buttons_yes, *buttons_no;
	char *axes_yes, *axes_no;
	char *cmd;
	char *temp;

	Split(command,&type,&temp);
	Split(temp,&value,&temp);
	Split(temp,&action,&temp);
	Split(temp,&buttons_yes,&temp);
	Split(temp,&buttons_no,&temp);
	Split(temp,&axes_yes,&temp);
	Split(temp,&axes_no,&cmd);
	free(temp);

	if (type == NULL) {
		Print(stderr,"exit: wrong format in action line: ", command);
		exit(1);
	}
	if (action == NULL) {
		Print(stderr,"exit: wrong format in action line: ", command);
		exit(1);
	}
	if (cmd == NULL) {
		Print(stderr,"exit: wrong format in action line: ", command);
		exit(1);
	}
	AddAction(type[0], atol(value), action[0],
		  atol(buttons_yes), atol(buttons_no),
		  atol(axes_yes), atol(axes_no), cmd);

	free(type);
	free(value);
	free(action);
	free(buttons_yes);
	free(buttons_no);
	free(axes_yes);
	free(axes_no);
	free(cmd);
}


void ParseGeneralLine(char * const command)
/* This parses a line from the [general] section */
{
	char *begin;
	char *end;

	char buffer[20]; /* should be sufficient for an integer */
	
	Split(command,&begin,&end);
	
/* String values */
	if (strcmp(begin,"device") == 0) {
		free(config.joy_device);
		config.joy_device=AllocString(end);
		if (config.debug > 1) {
			Print(stdout,"device  set to: ",config.joy_device);
		}
	} else if (strcmp(begin,"config") == 0) {
		free(config.config_file);
		config.config_file=AllocString(end);
		if (config.debug > 1) {
			Print(stdout,"config  set to: ",config.config_file);
		}
	} else if (strcmp(begin,"exit") == 0) {
		free(config.action_exit);
		config.action_exit=AllocString(end);
		if (config.debug > 1) {
			Print(stdout,"exit    set to: ",config.action_exit);
		}
	} else if (strcmp(begin,"reread") == 0) {
		free(config.action_reread);
		config.action_reread=AllocString(end);
		if (config.debug > 1) {
			Print(stdout,"reread  set to: ",config.action_reread);
		}
/* Integer values */
	} else if (strcmp(begin,"debug") == 0) {
		config.debug=atoi(end);
		if (config.debug > 1) {
			sprintf(buffer,"%i",config.debug);
			Print(stdout,"debug   set to: ",buffer);
		}
	} else if (strcmp(begin,"syslog") == 0) {
		config.syslog=atoi(end);
		if (config.debug > 1) {
			sprintf(buffer,"%i",config.syslog);
			Print(stdout,"syslog  set to: ",buffer);
		}
	} else if (strcmp(begin,"stdout") == 0) {
		config.std_out=atoi(end);
		if (config.debug > 1) {
			sprintf(buffer,"%i",config.std_out);
			Print(stdout,"stdout  set to: ",buffer);
		}
	} else if (strcmp(begin,"daemon") == 0) {
		config.daemon=atoi(end);
		if (config.debug > 1) {
			sprintf(buffer,"%i",config.daemon);
			Print(stdout,"daemon  set to: ",buffer);
		}
	} else if (strcmp(begin,"calmin") == 0) {
		config.cal_min_default=atoi(end);
		if (config.debug > 1) {
			sprintf(buffer,"%i",config.cal_min_default);
			Print(stdout,"calmin  set to: ",buffer);
		}
	} else if (strcmp(begin,"calmax") == 0) {
		config.cal_max_default=atoi(end);
		if (config.debug > 1) {
			sprintf(buffer,"%i",config.cal_max_default);
			Print(stdout,"calmax  set to: ",buffer);
		}
	} else {
		Print(stderr,"exit: wrong command in configfile: ",command);
		exit(1);
	}
	
	free(begin);
	free(end);
}



void ParseLine(char * const command,int *section)
/* This parses a line from the configuration file */
{
	if (strcmp(command,"[general]") == 0) {
		*section=1;
	} else if (strcmp(command,"[calibration]") == 0) {
		*section=2;
	} else if (strcmp(command,"[actions]") == 0) {
		*section=3;
	} else if (*section==0) {
		Print(stderr,"exit: error: [section] misssing in configfile: ",command);
		exit(1);
	} else {
		switch (*section) {
		case 1:
			ParseGeneralLine(command);
			break;
		case 2:
			ParseCalibrationLine(command);
			break;
		case 3:
			ParseActionLine(command);
			break;
		}
	}
}
