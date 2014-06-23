/*
 * logger.c
 *
 *  Created on: Jun 20, 2014
 *      Author: hessel
 */

#include "logger.h"

void log_error(char* error, ...){
#ifdef ENABLE_ERROR_LOGGING
	time_t rawtime = time(NULL);
	struct tm datetime = *localtime(&rawtime);
	char buffer[24];
	strftime(buffer, sizeof(buffer), "[%F %T] ", &datetime);
#ifdef ERROR_LOGGING_FILE
	FILE *fp1;
	fp1 = fopen(ERROR_LOGGING_FILE, "w");
	fprintf(fp1, "%s", buffer);
	fclose(fp1);
#else
	fprintf(stderr, "%s", buffer);
#endif
	va_list args;
	va_start(args, error);
#ifdef ERROR_LOGGING_FILE
	FILE *fp2;
	fp2 = fopen(ERROR_LOGGING_FILE, "w");
	vfprintf(fp2, error, args);
	fclose(fp2);
#else
	vfprintf(stderr, error, args);
#endif
	va_end(args);
#endif
}

void log_string(char* string, ...){
#ifdef ENABLE_LOGGING
	time_t rawtime = time(NULL);
	struct tm datetime = *localtime(&rawtime);
	char buffer[24];
	strftime(buffer, sizeof(buffer), "[%F %T] ", &datetime);
#ifdef LOGGING_FILE
	FILE *fp1;
	fp1 = fopen(LOGGING_FILE, "w");
	fprintf(fp1, "%s", buffer);
	fclose(fp1);
#else
	fprintf(stdout, "%s", buffer);
#endif
	va_list args;
	va_start(args, string);
#ifdef LOGGING_FILE
	FILE *fp2;
	fp2 = fopen(LOGGING_FILE, "w");
	vfprintf(fp2, string, args);
	fclose(fp2);
#else
	vfprintf(stdout, string, args);
#endif
	va_end(args);
#endif
}

void debug_message(char* debug_message,...){
#ifdef ENABLE_DEBUGGING
	time_t rawtime = time(NULL);
	struct tm datetime = *localtime(&rawtime);
	char buffer[24];
	strftime(buffer, sizeof(buffer), "[%F %T] ", &datetime);
	fprintf(stdout, "%s", buffer);

	va_list args;
	va_start(args, debug_message);
	vfprintf(stdout, debug_message, args);
	va_end(args);
#endif
}
