#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

/*
 * function prototypes
 */

void SetDefaultValues();
void ReadConfigFile();
void ParseLine(char * const command,int *section);
void ParseGeneralLine(char * const command);
void ParseCalibrationLine(char * const command);
void ParseActionLine(char * const command);
void AddCalibration(int min,int max);
void AddAction(unsigned long axes,unsigned long buttons,const char* const command);
void ClearActions();
void AddMissingCalibrations();

#endif