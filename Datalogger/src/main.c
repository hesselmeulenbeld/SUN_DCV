/*
 * main.c
 *
 *  Created on: Jun 3, 2014
 *      Author: hessel
 */

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "plc_interface.h"
#include "db_interface.h"
#include "data_package.h"
#include "logger.h"

static struct termios tty_old;

int loop_timeout = 0;

void *timeout_thread(){
	usleep(800000);
	loop_timeout = 1;
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int fd;
	int status = -1;
	if(argc < 1){
		log_string("Usage: Datalogger <port> <db>\n");
		exit(-1);
	}
	if((fd = open_connection(argv[1], &tty_old)) < 0){
		log_error("cannot open port %s", argv[1]);
		exit(-1);
	}

	setvbuf(stdout, NULL, _IONBF, 0);

	while((status = get_status(fd)) == -1);
	if(status == 1){
		if(stop_program(fd) != 1){
			exit(-1);
		}
		usleep(100000);
	}

	//CLOCK TEST
	struct tm logo_datetime;
	get_time(fd, &logo_datetime);
	debug_message("DvW: %d %d:%d\n", logo_datetime.tm_wday, logo_datetime.tm_hour, logo_datetime.tm_min);
	debug_message("%d-%d-%d\n", logo_datetime.tm_year, logo_datetime.tm_mon, logo_datetime.tm_mday);

	time_t rawtime = time(NULL);
	struct tm pi_datetime = *localtime(&rawtime);
	debug_message("DvW: %d %d:%d\n", pi_datetime.tm_wday, pi_datetime.tm_hour, pi_datetime.tm_min);
	debug_message("%d-%d-%d\n", pi_datetime.tm_year, pi_datetime.tm_mon, pi_datetime.tm_mday);
	set_time(fd, &pi_datetime);
	//END CLOCK TEST

	//TODO: sync clocks
	//TODO: open DB
	int i = 0;
	for(i = 0; i < 5; i++){
		if(start_program(fd) == 1){
			usleep(2000000);
			break;
		}
	}
	status = get_status(fd);
	if(status != 1){
		exit(-1);
	}

	unsigned char data[80];
	struct data_package package;
	int count = 0;
	pthread_t tt;
	struct data_package packages[10];
	while(1){
		loop_timeout = 0;
		count = 0;
		pthread_create(&tt, NULL, timeout_thread, NULL);
		while(loop_timeout != 1){
			if(get_data(fd, data) != -1){
				fill_data_package(&packages[count++], data);
				//print_data_package(&package);
			}else{
				usleep(100000);
				clean_filedescriptor(fd);
			}
		}
		average_package(&package, packages, count);
		rrd_insert(&package);
	}

	close_connection(fd, &tty_old);
	while(1);
	return 0;
}
