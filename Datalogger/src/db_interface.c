/*
 * db_interface.c
 *
 *  Created on: Jun 19, 2014
 *      Author: hessel
 */

#include "db_interface.h"
#include "data_package.h"

char rrd_insert(struct data_package *package){
	char buffer[100];
	sprintf(buffer, "N:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",
			package->AFS.AM1, //Tcollector
			package->AFS.AM2, //Ta
			package->AFS.AM3, //Tb
			package->AFS.AM4, //Text
			(package->DIS.I1_I8 & B_I5) >> 4, //BewoningA
			(package->DIS.I1_I8 & B_I6) >> 5, //BewoningB
			(package->DOS.Q1_Q8 & B_Q3) >> 2, //HeaterA
			(package->DOS.Q1_Q8 & B_Q4) >> 3, //HeaterB
			(package->DOS.Q1_Q8 & B_Q2) >> 1, //Valve
			(package->DOS.Q1_Q8 & B_Q1), //Pump
			package->AFS.AM5, //ThresholdA
			package->AFS.AM6);//ThresholdB

	char *updateparams[] = {
			"update",
			DB_LOCATION,
			buffer
	};

	optind = opterr = 0;
	rrd_clear_error();
	int status = 0;
	status = rrd_update(3, updateparams);
	debug_message("%s\n", rrd_get_error());
	debug_message("Update status: %d.\n", status);
	return 1;
}
