/*
 * plc_interface.c
 *
 *  Created on: Jun 13, 2014
 *      Author: hessel
 */

#include "plc_interface.h"

struct sigaction sa;
struct itimerval receive_timer;

int timeout = 0;


int open_connection(char *portname, struct termios *tty_old) {
	//open port
	int fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if (fd == -1) {
		log_error("open_connection: Could not connect to %s.\n", portname);
		return ERROR;
	}

	//get current terminal settings
	if (tcgetattr(fd, tty_old) < 0) {
		log_error("open_connection: Cannot read current terminal settings.\n");
		return ERROR;
	}

	//set communication settings
	struct termios tty;

	tty = *tty_old;
	tty.c_lflag &= ~(ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL
			| ECHOKE);
	tty.c_iflag &= ~(BRKINT | ICRNL | IMAXBEL | IXON | IXOFF);
	tty.c_iflag |= (INPCK);
	tty.c_oflag &= (OPOST | ONLCR);
	tty.c_cflag |= (PARENB);
	tty.c_cflag &= ~(PARODD | PARMRK);

	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 1;

	if (cfsetospeed(&tty, BAUDRATE) < 0) {
		log_error("open_connection: Cannot set output baud rate.\n");
	}
	if (cfsetispeed(&tty, BAUDRATE) < 0) {
		log_error("open_connection: Cannot set input baud rate.\n");
	}
	if (tcsetattr(fd, TCSAFLUSH, &tty) < 0) {
		log_error("open_connection: Cannot set terminal attributes\n");
	}
	return fd;
}

char close_connection(int fd, struct termios *tty_old) {
	if (tcsetattr(fd, TCSAFLUSH, tty_old) < 0) {
		log_error("close_connection: Cannot reset terminal attributes.\n");
	}
	if (close(fd) == -1) {
		log_error("close_connection: Cannot close the connection.\n");
		return ERROR;
	}
	return OK;
}

int send_command(int fd, char *args, size_t count) {
	int bytes_written = write(fd, args, count);
	return bytes_written;
}

void receive_timeout_handler() {
	timeout = 1;
}

int receive(int fd, unsigned char buffer[], int number_of_bytes) {
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &receive_timeout_handler;
	sigaction(SIGALRM, &sa, NULL );

	receive_timer.it_value.tv_sec = 0;
	receive_timer.it_value.tv_usec = TIMEOUT_USEC * number_of_bytes;
	receive_timer.it_interval.tv_sec = 0;
	receive_timer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &receive_timer, NULL );
	timeout = 0;

	unsigned char onebytebuffer[1];
	int bytes_read = 0;
	while (timeout == 0) {
		if (read(fd, onebytebuffer, 1) == 1) {
			buffer[bytes_read] = onebytebuffer[0];
			if(++bytes_read == number_of_bytes){
				receive_timer.it_value.tv_usec = 0;
				setitimer(ITIMER_REAL, &receive_timer, NULL);
				return bytes_read;
			}
		}
	}
	log_error("receive: timeout at reading byte %d of %d.\n", bytes_read + 1, number_of_bytes);
	return TIMEOUT;
}

/* unused */
char is_available(int fd) {
	char isavailableparams[] = { 0x21 };
	send_command(fd, isavailableparams, 1);
	unsigned char buffer[1];
	int receive_status = 0;
	if ((receive_status = receive(fd, buffer, 4)) == 4) {
		switch (buffer[0]) {
		case 0x06:	//ACK
			log_string("PLC is available.\n");
			return OK;
		case 0x15:	//NACK
			log_string("PLC is not available.\n");
			log_error("PLC is not available.\n");
			return ERROR;
		}
	}else if(receive_status == TIMEOUT){
		log_error("Is available: Timeout.");
		return TIMEOUT;
	}
	log_error("Is available: Unknown reply.");
	return ERROR;
}

char start_program(int fd) {
	char startparams[] = { 0x55, 0x18, 0x18, 0xAA };
	send_command(fd, startparams, 4);
	unsigned char buffer[1];
	int receive_status = 0;
	if ((receive_status = receive(fd, buffer, 1)) == 1) {
		switch (buffer[0]) {
		case 0x06:	//ACK
			log_string("Start Program: Progam started.\n");
			return OK;
		case 0x15:	//NACK
			log_string("Start Program: Program could not be started.\n");
			receive(fd, buffer, 1);
			log_error("Start Program: Program could not be started. Error code: %d\n", buffer[0]);
			return ERROR;
		}
	}else if(receive_status == TIMEOUT){
		log_error("Start program: Timeout.");
		return TIMEOUT;
	}
	log_error("Start Program: Unknown reply.");
	return ERROR;
}

char stop_program(int fd) {
	char stopparams[] = { 0x55, 0x12, 0x12, 0xAA };
	send_command(fd, stopparams, 4);
	unsigned char buffer[1];
	int receive_status = 0;
	if ((receive_status = receive(fd, buffer, 1)) == 1) {
		switch (buffer[0]) {
		case 0x06:	//ACK
			log_string("Stop Program: Program stopped.\n");
			return OK;
		case 0x15:	//NACK
			log_string("Stop Program: Program could not be stopped.\n");
			receive(fd, buffer, 1);
			log_error("Stop Program: Program could not be stopped. Error code: %d\n", buffer[0]);
			return ERROR;
		}
	}else if(receive_status == TIMEOUT){
		log_error("Stop program: Timeout.");
		return TIMEOUT;
	}
	log_error("Stop Program: Unknown reply\n.");
	return ERROR;
}

int get_status(int fd) {
	char getstatusparams[] = { 0x55, 0x17, 0x17, 0xAA };
	send_command(fd, getstatusparams, 4);
	unsigned char buffer[1];
	int receive_status = 0;
	if ((receive_status = receive(fd, buffer, 1)) == 1) {
		if (buffer[0] == 0x06) {
			if (receive(fd, buffer, 1) == 1) {
				switch (buffer[0]) {
				case 0x01:
					return ON;
				case 0x42:
					return OFF;
				}
			}
		}else{
			receive(fd, buffer, 1);
			log_error("Could not get status. Errorcode: %d.\n", buffer);
		}
	}else if(receive_status == TIMEOUT){
		log_error("Get status: Timeout.");
		return TIMEOUT;
	}
	log_error("Could not get status.\n");
	return ERROR;
}

char get_data(int fd, unsigned char data[]) {
	char startdatatransparams[] = { 0x55, 0x13, 0x13, 0x00, 0xAA };
	char stopdatatransparams[] = { 0x06, 0x55, 0x14, 0x14, 0xAA };

	send_command(fd, startdatatransparams, 5);
	int index = 0;
	unsigned char buffer[1];
	if (receive(fd, buffer, 1) == 1) {
		if (buffer[0] == 0x06) {
			for (index = 0; index < 80; index++) {
				if (receive(fd, buffer, 1) == 1) {
					data[index] = buffer[0];
				}
			}
			send_command(fd, stopdatatransparams, 5);
			receive(fd, buffer, 1);
			if(data[0] == 0x55 && data[79] == 0xAA){
				return OK;
			}else{
				log_error("Data: not the right data format.\n");
				return ERROR;
			}
		} else if (buffer[0] == 0x15) {
			log_error("Get data: NACK.\n");
			return ERROR;
		}
	}
	log_error("Could not get any data.\n");
	return ERROR;
}

char check_compatible_time(struct tm *time){
	int no_error = 1;
	if(time->tm_mday > 31 || time->tm_mday < 1){
		log_error("The day is not in the right range: 1-31");
		no_error = -1;
	}
	if((time->tm_mon +1) > 12 || time->tm_mon + 1 < 1){
		log_error("The month is not in the right range: 0-11");
		no_error = -1;
	}
	if(time->tm_year < 100 || time->tm_year > 199){
		log_error("The year is not in the right range: 100-199");
		no_error = -1;
	}
	if(time->tm_min < 0 || time->tm_min > 59){
		log_error("The minutes is not in the right range: 0-59");
		no_error = -1;
	}
	if(time->tm_hour < 0 || time->tm_hour > 23 ){
		log_error("The hour is not in the right range: 0-23");
		no_error = -1;
	}
	if(time->tm_wday < 0 || time->tm_wday > 6){
		log_error("The weekday is not in the right range: 0-6");
		no_error = -1;
	}
	return no_error;
}

char set_time(int fd, struct tm *time){
	if(!check_compatible_time(time)){
		return -2;
	}

	char setdayparams[] = { 0x01, 0x00, 0xFF, 0xFB, 0x00, time->tm_mday };
	char setmonthparams[] = { 0x01, 0x00, 0xFF, 0xFB, 0x01, time->tm_mon + 1 };
	char setyearparams[] = { 0x01, 0x00, 0xFF, 0xFB, 0x02, time->tm_year - 100 };
	char setminuteparams[] = { 0x01, 0x00, 0xFF, 0xFB, 0x03, time->tm_min };
	char sethourparams[] = { 0x01, 0x00, 0xFF, 0xFB, 0x04, time->tm_hour };
	char setdayofweekparams[] = { 0x01, 0x00, 0xFF, 0xFB, 0x05, time->tm_wday };
	char activateiclockparams[] = { 0x01, 0x00, 0xFF, 0x43, 0x00, 0x00 };

	unsigned char buffer[1];

	send_command(fd, setdayparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Set time: Day set.\n");
		}else{
			receive(fd, buffer, 1);
			log_error("Unable to set day. Errorcode: %d.\n", buffer[0]);
			return -1;
		}
	}

	buffer[0] = 0;
	send_command(fd, setmonthparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Set time: Month set.\n");
		}else{
			receive(fd, buffer, 1);
			log_error("Unable to set month. Errorcode: %d.\n", buffer[0]);
			return -1;
		}
	}

	buffer[0] = 0;
	send_command(fd, setyearparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Set time: Year set.\n");
		}else{
			receive(fd, buffer, 1);
			log_error("Unable to set year. Errorcode: %d.\n", buffer[0]);
			return -1;
		}
	}

	buffer[0] = 0;
	send_command(fd, setminuteparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Set time: Minutes set.\n");
		}else{
			receive(fd, buffer, 1);
			log_error("Unable to set minutes. Errorcode: %d.\n", buffer[0]);
			return -1;
		}
	}

	buffer[0] = 0;
	send_command(fd, sethourparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			//clean_filedescriptor(fd);
			debug_message("Set time: Hour set.\n");

		}else{
			receive(fd, buffer, 1);
			log_error("Unable to set hour. Errorcode: %d.\n", buffer[0]);
			return -1;
		}
	}

	//clean_filedescriptor(fd);

	buffer[0] = 0;
	send_command(fd, setdayofweekparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Set time: Day of the week set.\n");
		}else{
			receive(fd, buffer, 1);
			log_error("Unable to set day of the week. Errorcode: %d.\n", buffer[0]);
			return -1;
		}

	}

	buffer[0] = 0;
	send_command(fd, activateiclockparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Set time: activated internal clock.\n");
		}else{
			receive(fd, buffer, 1);
			log_error("Unable to activate the internal clock. Errorcode: %d.\n", buffer[0]);
			return -1;
		}
	}

	log_string("Set time: Clock set.\n");
	return 1;
}

char get_time(int fd, struct tm *time){
	char updateirparams[] = { 0x01, 0x00, 0xFF, 0x44, 0x00, 0x00 };
	char getdayparams[] = { 0x02, 0x00, 0xFF, 0xFB, 0x00 };
	char getmonthparams[] = { 0x02, 0x00, 0xFF, 0xFB, 0x01 };
	char getyearparams[] = { 0x02, 0x00, 0xFF, 0xFB, 0x02 };
	char getminuteparams[] = { 0x02, 0x00, 0xFF, 0xFB, 0x03 };
	char gethourparams[] = { 0x02, 0x00, 0xFF, 0xFB, 0x04 };
	char getdayofweekparams[] = { 0x02, 0x00, 0xFF, 0xFB, 0x05 };

	unsigned char buffer[6];

	send_command(fd, updateirparams, 6);
	if (receive(fd, buffer, 1) == 1) {
		if(buffer[0] == 0x06){
			debug_message("Get time: Updated internal register.\n");
		}else{
			log_error("Get time: Unable to update the internal register.\n");
			return -1;
		}
	}

	send_command(fd, getdayparams, 5);
	if(receive(fd, buffer, 1) == 1){
		if(buffer[0] == 0x06){
			if(receive(fd, buffer, 6) == 6){
				debug_message("Get time: Day is: %d\n", buffer[5]);
				time->tm_mday = buffer[5];
			}else{
				log_error("Get time: Unable to get the day.\n");
			}
		}else{
			log_error("Get time: Unable to get the day.\n");
			return -1;
		}
	}

	send_command(fd, getmonthparams, 5);
	if(receive(fd, buffer, 1) == 1){
		if(buffer[0] == 0x06){
			if(receive(fd, buffer, 6) == 6){
				debug_message("Get time: Month is: %d\n", buffer[5]);
				time->tm_mon = buffer[5] - 1;
			}else{
				log_error("Get time: Unable to get the month.\n");
			}
		}else{
			log_error("Get time: Unable to get the month.\n");
			return -1;
		}
	}

	send_command(fd, getyearparams, 5);
	if(receive(fd, buffer, 1) == 1){
		if(buffer[0] == 0x06){
			if(receive(fd, buffer, 6) == 6){
				debug_message("Get time: Year is: %d\n", 2000 + buffer[5]);
				time->tm_year = 100 + buffer[5];
			}else{
				log_error("Get time: Unable to get the year.\n");
			}
		}else{
			log_error("Get time: Unable to get the year.\n");
			return -1;
		}
	}

	send_command(fd, getminuteparams, 5);
	if(receive(fd, buffer, 1) == 1){
		if(buffer[0] == 0x06){
			if(receive(fd, buffer, 6) == 6){
				debug_message("Get time: Minutes is: %d\n", buffer[5]);
				time->tm_min = buffer[5];
			}else{
				log_error("Get time: Unable to get the minutes.\n");
			}
		}else{
			log_error("Get time: Unable to get the minutes.\n");
			return -1;
		}
	}

	send_command(fd, gethourparams, 5);
	if(receive(fd, buffer, 1) == 1){
		if(buffer[0] == 0x06){
			if(receive(fd, buffer, 6) == 6){
				debug_message("Get time: Hour is: %d\n", buffer[5]);
				time->tm_hour = buffer[5];
			}else{
				log_error("Get time: Unable to get the hour.\n");
				return -1;
			}
		}else{
			log_error("Get time: Unable to get the hour.\n");
			return -1;
		}
	}

	send_command(fd, getdayofweekparams, 5);
	if(receive(fd, buffer, 1) == 1){
		if(buffer[0] == 0x06){
			if(receive(fd, buffer, 6) == 6){
				debug_message("Get time: DvW is: %d\n", buffer[5]);
				time->tm_wday = buffer[5];
			}else{
				log_error("Get time: Unable to get the day of the week.\n");
				return -1;
			}
		}else{
			log_error("Get time: Unable to get the day of the week.\n");
			return -1;
		}
	}
	log_string("Get time: All clock data received.\n");
	return 1;
}

void clean_filedescriptor(int fd){
	unsigned char buffer[1];
	while(receive(fd, buffer, 1) == 1){
		debug_message("Cleaned buffer: %d.\n", buffer[0]);
	}
}
