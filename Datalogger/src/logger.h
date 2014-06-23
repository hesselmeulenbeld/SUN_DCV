/*
 * logger.h
 *
 *  Created on: Jun 20, 2014
 *      Author: hessel
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ENABLE_ERROR_LOGGING
#define ENABLE_LOGGING
#define ENABLE_DEBUGGING

#ifdef ENABLE_ERROR_LOGGING
//define ERROR_LOGGIG_FILE for logging to a file.
//or alternatively, redirect stderr to a file.
//#define ERROR_LOGGING_FILE "/home/hessel/Afstuderen/logs/error"
#endif

#ifdef ENABLE_LOGGING
//define LOGGING_FILE for logging to a file.
//or alternatively, redirect stdout to a file.
//#define LOGGING_FILE "/home/hessel/Afstuderen/logs/log"
#endif

/*
 *	Function for logging to stderr.
 *	Adds timestamp to front of error.
 *	Enabled by defining ENABLE_ERROR_LOGGING
 */
void log_error(char* error, ...);

/*
 *	Function for logging to stdout.
 *	Adds timestamp to front of string.
 *	Enabled by defining ENABLE_LOGGING
 */
void log_string(char* string, ...);

/*
 *	Function for loggin to stdout.
 *	Adds timestamp to front of string.
 *	Enabled by defining ENABLE_DEBUGGING
 */
void debug_message(char* debug_string, ...);

#endif /* LOGGER_H_ */
