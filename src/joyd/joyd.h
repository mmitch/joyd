/* joyd.h
 *
 * part of
 * joyd 0.2.2 2000-10-11
 *
 */

#ifndef _JOYD_H_INCLUDED_
#define _JOYD_H_INCLUDED_

/*
 *
 *   These are the default settings for joyd
 *   Feel free to edit them to your needs.
 *
 *   After changing this file use "make clean",
 *   "make all" to let the changes become active.
 *
 *   If you change these default settings, then
 *   please also edit the "default values" section
 *   of the joydrc(5) manpage (see file joydrc.5).
 *   Otherwise you will confuse your users.
 *
 */

/* default joystick device (different for BSD and Linux) */
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#define JOY_DEVICE "/dev/joy0"
#else
#define JOY_DEVICE "/dev/js0"
#endif

/* default configuration file */
#define CONFIG_FILE "~/.joydrc"

/* show debug information?
   (0=be quiet, 1=some messages, 2=more messages, 3=full debug) */
#define SHOW_DEBUG 1

/* show messages on stdout? (0=no, 1=yes) */
#define SHOW_STDOUT 1

/* show messages in syslog? (0=no, 1=yes) */
#define SHOW_SYSLOG 0

/* syslog facility that joyd should use (see syslog.h) */
#define SYSLOG_FACILITY LOG_DAEMON

/* syslog priority used for informational messages (see syslog.h) */
#define SYSLOG_PRIO_INFO LOG_INFO

/* syslog priority used for error messages (see syslog.h) */
#define SYSLOG_PRIO_ERROR LOG_ERR

/* start in daemon mode? (0=foreground, 1=background) */
#define DAEMON_MODE 0

/* default min calibration value */
#define CALIBRATION_MIN -16384

/* default max calibration value */
#define CALIBRATION_MAX 16383

/* default action for exiting joyd */
#define ACTION_EXIT "$exit$"

/* default action for rereading configuration file */
#define ACTION_REREAD "$reread$"


/* BSD only variables BEGIN */
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

/* the waiting time after reading the joystick port (in microseconds) */
#define TIMEOUT_JS 50000

#endif
/* BSD only variables END */


/*
 *
 *   STOP!
 *   Please don't edit anything beyond this line.
 *
 */


/* program name */
#define PROGRAM_NAME "joyd"

/* program version */
#define PROGRAM_VERSION PROGRAM_NAME " 0.3.0 2001-01-04"

/* length of buffer for joystick name */
#define NAME_LENGTH 128

/* an empty string */
#define NOTHING "\0"

/* this is the comment sign in the configuration file */
#define COMMENT_SIGN '#'

/* the command line argument used to show the version of joyd */
#define PRINT_VERSION "-v"

/* the long command line argument used to show the version of joyd */
#define PRINT_VERSION_LONG "--version"

/* the long command line argument used to show the help page of joyd */
#define PRINT_HELP_LONG "--help"

/* the informative text to show with the version number */
#define PROGRAM_INFOTEXT "usage: " PROGRAM_NAME " {configuration file}\n" \
                         "documentation is installed under " DOCDIR

/* the filename that means stdin */
#define CONFIG_FROM_STDIN "-"

/*
 * The structure for calibration values
 */
typedef struct scalibration {
	int min;
	int max;
} TCALIBRATION;

/*
 * The structure for an action
 */
typedef struct saction {
	char type;                 /* 'B'utton or 'A'xis                    */
	unsigned int value;        /* Which button/axis?                    */
	char action;               /* '+'==press, '-'==release              */
	unsigned int buttons_yes;  /* other buttons that must be pressed    */
	unsigned int buttons_no;   /* other buttons that mustn't be pressed */
	unsigned int axes_yes;     /* other axes that must be "on"          */
	unsigned int axes_no;      /* other axes that mustn't be "on"       */
	char *command;             /* the command to execute                */
} TACTION;

/*
 * The structure for the configuration variables
 */
typedef struct sconfig {
	int debug;
	int syslog;
	int std_out;
	int daemon;
	char *config_file;

	char *action_exit;
	char *action_reread;
	int action_count;
	TACTION *action;

	int cal_count;
	TCALIBRATION *cal;
	int cal_min_default;
	int cal_max_default;

	unsigned char axes;
	unsigned char buttons;
	int version;
	char *joy_device;
	char joy_name[NAME_LENGTH];

	int fd;
} TCONFIG;

/*
 * Here we define a global variable - sorry, but I don't see
 * another way to let the signal handlers use the configuration
 * variables...
 */

extern TCONFIG config;

#endif
