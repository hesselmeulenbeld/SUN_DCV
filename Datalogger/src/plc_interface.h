/*
 * plc_interface.h
 *
 *  Created on: Jun 16, 2014
 *      Author: hessel
 */

#ifndef PLC_INTERFACE_H_
#define PLC_INTERFACE_H_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include "logger.h"

#define BAUDRATE B9600
#define TIMEOUT_USEC 250000

/*** Return values ***/
#define TIMEOUT -2
#define OK 1
#define ERROR -1

#define ON 1
#define OFF 0
/*** End of return values ***/

/*
 *	Start a connection with the port given.
 *	-Open a connection with the port.
 *	-Save the old settings of the port.
 *	-Create new settings for the port.
 *
 *	Input parameters:
 *	char *portname 			: The path to the port. example: /dev/ttyUSB0.
 *	struct termios *tty_old : Old settings of the port.
 *
 *	Returns the value of the filedescriptor created or -1
 *	when creation is not possible.
 */
int open_connection(char *portname, struct termios *tty_old);

/*
 * 	Close the given connection.
 *	-Reset the port settings
 *	-Close the connection.
 *
 * 	Input parameters:
 * 	int fd 					: The filedescriptor to be closed.
 * 	struct termios *tty_old : Old settings for the connection.
 *
 *	Returns 1 when all actions are completed without errors or -1
 *	when it's not possible to close the connection
 */
char close_connection(int fd, struct termios *tty_old);

/*
 *	Checks if the PLC is available for commands.
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *
 *	Returns 1 when available, otherwise -1.
 *	Returns -2 in case of a timeout.
 */
char is_available(int fd);

/*
 * 	Sends the command for starting the program on the PLC.
 *	-Send the command
 *	-Receive the reply
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *
 *	Returns 1 when the program is started, and -1
 *	when the program could not be started
 *	or a wrong reply is received.
 *	Returns -2 in case of a timeout.
 */
char start_program(int fd);

/*
 * 	Sends the command for stopping the program on the PLC.
 *	-Send the command
 *	-Receive the reply
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *
 *	Returns 1 when the program is stopped, and -1
 *	when the program could not be stopped
 *	or a wrong reply is received.
 *	Returns -2 in case of a timeout
 */
char stop_program(int fd);

/*
 * 	Sends the command for getting the status of the program on the PLC.
 *	-Send the command
 *	-Receive the reply
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *
 *	Returns 1 when the program is running, and 0
 *	when not. Returns -1 when the status is not returned.
 *	Returns -2 in case of a timeout
 */
int get_status(int fd);

/*
 *	Sends the command for getting the data of the program on the PLC.
 *	-Send the command to start periodic data reports.
 *	-Process the received data.
 *	-Send the command to stop periodic data reports.
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *	unsigned char data[]	: The array to put the data in.
 *
 *	Returns 1 when the data is received, otherwise -1.
 */
char get_data(int fd, unsigned char data[]);

/*
 *	Sends the commands to set the time on the PLC.
 *	-Check if the given time is compatible with the time on the LOGO.
 *	-Send the commands for setting the day/month/year/hour/minute/day of the week.
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *	struct tm *time			: The object containing the time.
 *
 *	Returns 1 when the time is set, returns -1 when the time could not be set.
 */
char set_time(int fd, struct tm *time);

/*
 *	Sends the commands to get the time from the PLC.
 *	-Send the commands for getting the day/month/year/hour/minute/day of the week.
 *
 *	Input parameters:
 *	int fd					: The filedescriptor for the connection with the PLC.
 *	struct tm *time			: The object to put the time in.
 *
 *	Returns 1 when the time is received, returns -1 when the time is not received.
 */
char get_time(int fd, struct tm *time);

/*
 *	Cleans the read buffer from the filedescriptor.
 */
void clean_filedescriptor(int fd);

#endif /* PLC_INTERFACE_H_ */
