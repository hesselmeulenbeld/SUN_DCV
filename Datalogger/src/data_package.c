/*
 * data_package.c
 *
 *  Created on: Jun 19, 2014
 *      Author: hessel
 */

#include "data_package.h"

char fill_data_package(struct data_package *package, unsigned char data[]){
	if(data[79] != 0xAA){
		log_error("fill_data_package: The given data is too small.\n");
		return -1;
	}

	//Set digital inputs
	package->DIS.I1_I8 = data[BP_I1_I8];
	package->DIS.I9_I16 = data[BP_I9_I16];
	package->DIS.I17_I24 = data[BP_I17_I24];

	//Set digital outputs
	package->DOS.Q1_Q8 = data[BP_Q1_Q8];
	package->DOS.Q9_Q16 = data[BP_Q9_Q16];

	//Set digital flags
	package->DFS.M1_M8 = data[BP_M1_M8];
	package->DFS.M9_M16 = data[BP_M9_M16];
	package->DFS.M17_M24 = data[BP_M17_M24];
	package->DFS.M25_M27 = data[BP_M25_M27];

	//Set shift register and cursors
	package->SR.S1_S8 = data[BP_S1_S8];
	package->CRS.C1_C4 = data[BP_C1_C4];

	//Set analog inputs
	package->AIS.AI1 = (data[BP_AI1_MSB] << 8) + data[BP_AI1_LSB];
	package->AIS.AI2 = (data[BP_AI2_MSB] << 8) + data[BP_AI2_LSB];
	package->AIS.AI3 = (data[BP_AI3_MSB] << 8) + data[BP_AI3_LSB];
	package->AIS.AI4 = (data[BP_AI4_MSB] << 8) + data[BP_AI4_LSB];
	package->AIS.AI5 = (data[BP_AI5_MSB] << 8) + data[BP_AI5_LSB];
	package->AIS.AI6 = (data[BP_AI6_MSB] << 8) + data[BP_AI6_LSB];
	package->AIS.AI7 = (data[BP_AI7_MSB] << 8) + data[BP_AI7_LSB];
	package->AIS.AI8 = (data[BP_AI8_MSB] << 8) + data[BP_AI8_LSB];

	//Set analog outputs
	package->AOS.AQ1 = (data[BP_AQ1_MSB] << 8) + data[BP_AQ1_LSB];
	package->AOS.AQ2 = (data[BP_AQ2_MSB] << 8) + data[BP_AQ2_LSB];

	//Set analog flags
	package->AFS.AM1 = (data[BP_AM1_MSB] << 8) + data[BP_AM1_LSB];
	package->AFS.AM2 = (data[BP_AM2_MSB] << 8) + data[BP_AM2_LSB];
	package->AFS.AM3 = (data[BP_AM3_MSB] << 8) + data[BP_AM3_LSB];
	package->AFS.AM4 = (data[BP_AM4_MSB] << 8) + data[BP_AM4_LSB];
	package->AFS.AM5 = (data[BP_AM5_MSB] << 8) + data[BP_AM5_LSB];
	package->AFS.AM6 = (data[BP_AM6_MSB] << 8) + data[BP_AM6_LSB];
	return 1;
}

void print_data_package(struct data_package *package){
	debug_message("Digital inputs: I1-I8: 0x%02x "
			"I9-I16: 0x%02x I17-I24: 0x%02x\n",
			package->DIS.I1_I8, package->DIS.I9_I16, package->DIS.I17_I24);
	debug_message("Digital outputs: Q1-Q8: 0x%02x "
			"Q9-Q16: 0x%02x\n",
			package->DOS.Q1_Q8, package->DOS.Q9_Q16);
	debug_message("Digital flags: M1-M8: 0x%02x "
			"M9-M16: 0x%02x M17-M24: 0x%02x\n",
			package->DIS.I1_I8, package->DIS.I9_I16, package->DIS.I17_I24);
	debug_message("Shift register: S1-S8: 0x%02x\n", package->SR.S1_S8);
	debug_message("Cursors: C1-C4: 0x%02x\n", package->CRS.C1_C4);
	debug_message("Analog inputs: AI1: %d AI2: %d AI3: %d "
			"AI4: %d AI5: %d AI6: %d AI7: %d AI8: %d\n",
			package->AIS.AI1, package->AIS.AI2, package->AIS.AI3, package->AIS.AI4,
			package->AIS.AI5, package->AIS.AI6, package->AIS.AI7, package->AIS.AI8);
	debug_message("Analog outputs: AQ1: %d AQ2: %d\n", package->AOS.AQ1, package->AOS.AQ2);
	debug_message("Analog flags: AM1: %d AM2: %d AM3: %d "
			"AM4: %d AM5: %d AM6: %d\n",
			package->AFS.AM1, package->AFS.AM2, package->AFS.AM3, package->AFS.AM4,
			package->AFS.AM5, package->AFS.AM6);
	debug_message("\n");
}

/*
 * ONLY USABLE FOR 0BA6 without expansions.
 * Only averages for inputs I1-I8.
 */
void average_digital_inputs(struct data_package *package, struct data_package *packages, int count){
	double I1 = 0, I2 = 0, I3 = 0, I4 = 0,
			I5 = 0, I6 = 0, I7 = 0, I8 = 0;

	int i = 0;
	for(i = 0; i < count; i++){
		I1 += packages[i].DIS.I1_I8 & B_I1;
		I2 += (packages[i].DIS.I1_I8 & B_I2) >> 1;
		I3 += (packages[i].DIS.I1_I8 & B_I3) >> 2;
		I4 += (packages[i].DIS.I1_I8 & B_I4) >> 3;
		I5 += (packages[i].DIS.I1_I8 & B_I5) >> 4;
		I6 += (packages[i].DIS.I1_I8 & B_I6) >> 5;
		I7 += (packages[i].DIS.I1_I8 & B_I7) >> 6;
		I8 += (packages[i].DIS.I1_I8 & B_I8) >> 7;
	}
	if(I1 / count > 0.5){
		package->DIS.I1_I8 |= B_I1;
	}
	if(I2 / count > 0.5){
		package->DIS.I1_I8 |= B_I2;
	}
	if(I3 / count > 0.5){
		package->DIS.I1_I8 |= B_I3;
	}
	if(I4 / count > 0.5){
		package->DIS.I1_I8 |= B_I4;
	}
	if(I5 / count > 0.5){
		package->DIS.I1_I8 |= B_I5;
	}
	if(I6 / count > 0.5){
		package->DIS.I1_I8 |= B_I6;
	}
	if(I7 / count > 0.5){
		package->DIS.I1_I8 |= B_I7;
	}
	if(I8 / count > 0.5){
		package->DIS.I1_I8 |= B_I8;
	}
}

/*
 * ONLY USABLE FOR 0BA6 without expansions.
 * Only averages for outputs Q1-Q4.
 */
void average_digital_outputs(struct data_package *package, struct data_package *packages, int count){
	double Q1 = 0, Q2 = 0, Q3 = 0, Q4 = 0;

	int i = 0;
	for(i = 0; i < count; i++){
		Q1 += packages[i].DOS.Q1_Q8 & B_Q1;
		Q2 += (packages[i].DOS.Q1_Q8 & B_Q2) >> 1;
		Q3 += (packages[i].DOS.Q1_Q8 & B_Q3) >> 2;
		Q4 += (packages[i].DOS.Q1_Q8 & B_Q4) >> 3;
	}
	if(Q1 / count > 0.5){
		package->DOS.Q1_Q8 |= B_Q1;
	}
	if(Q2 / count > 0.5){
		package->DOS.Q1_Q8 |= B_Q2;
	}
	if(Q3 / count > 0.5){
		package->DOS.Q1_Q8 |= B_Q3;
	}
	if(Q4 / count > 0.5){
		package->DOS.Q1_Q8 |= B_Q4;
	}
}

void average_package(struct data_package *package, struct data_package *packages, int count){
	int i = 0;

	average_digital_inputs(package, packages, count);
	average_digital_outputs(package, packages, count);

	for(i = 0; i < count; i++){
		package->AFS.AM1 += packages[i].AFS.AM1;
		package->AFS.AM2 += packages[i].AFS.AM2;
		package->AFS.AM3 += packages[i].AFS.AM3;
		package->AFS.AM4 += packages[i].AFS.AM4;
		package->AFS.AM5 += packages[i].AFS.AM5;
		package->AFS.AM6 += packages[i].AFS.AM6;

		package->AIS.AI1 += packages[i].AIS.AI1;
		package->AIS.AI2 += packages[i].AIS.AI2;
		package->AIS.AI3 += packages[i].AIS.AI3;
		package->AIS.AI4 += packages[i].AIS.AI4;
	}
	package->AFS.AM1 /= count;
	package->AFS.AM2 /= count;
	package->AFS.AM3 /= count;
	package->AFS.AM4 /= count;
	package->AFS.AM5 /= count;
	package->AFS.AM6 /= count;

	package->AIS.AI1 /= count;
	package->AIS.AI2 /= count;
	package->AIS.AI3 /= count;
	package->AIS.AI4 /= count;
}
