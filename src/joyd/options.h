/* options.h
 *
 * part of
 * joyd 0.2.2 2000-10-11
 *
 */

#ifndef _OPTIONS_H_INCLUDED_
#define _OPTIONS_H_INCLUDED_

/*
 * function prototypes
 */

void SetDefaultValues();
void ReadConfigFile();
void ParseLine(char * const command, int *section);
void ParseGeneralLine(char * const command);
void ParseCalibrationLine(char * const command);
void ParseActionLine(char * const command);
void AddCalibration(int min, int max);
void AddAction(char type, unsigned int value, char action, unsigned int buttons_yes, unsigned int buttons_no, unsigned int axes_yes, unsigned int axes_no, char * command);
void ClearActions();
void AddMissingCalibrations();

#endif
