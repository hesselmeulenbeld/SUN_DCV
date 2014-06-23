/*
 * data_package.h
 *
 *  Created on: Jun 19, 2014
 *      Author: hessel
 */

#ifndef DATA_PACKAGE_H_
#define DATA_PACKAGE_H_

#include "logger.h"

/*
 * Struct containing fields for the digital inputs.
 */
struct digital_inputs{
	unsigned char I1_I8;
	unsigned char I9_I16;
	unsigned char I17_I24;
};

/*
 * Struct containing fields for the digital outputs.
 */
struct digital_outputs{
	unsigned char Q1_Q8;
	unsigned char Q9_Q16;
};

/*
 * Struct containing fields for the digital flags.
 */
struct digital_flags{
	unsigned char M1_M8;
	unsigned char M9_M16;
	unsigned char M17_M24;
	unsigned char M25_M27;
};

/*
 * Struct containing fields for the shift register.
 */
struct shift_register{
	unsigned char S1_S8;
};

/*
 * Struct containing fields for the cursors.
 */
struct cursors{
	unsigned char C1_C4;
};

/*
 * Struct containing fields for the analog inputs.
 */
struct analog_inputs{
	int AI1;
	int AI2;
	int AI3;
	int AI4;
	int AI5;
	int AI6;
	int AI7;
	int AI8;
};

/*
 * Struct containing fields for the analog outputs.
 */
struct analog_outputs{
	int AQ1;
	int AQ2;
};

/*
 * Struct containing fields for the analog flags.
 */
struct analog_flags{
	int AM1;
	int AM2;
	int AM3;
	int AM4;
	int AM5;
	int AM6;
};

/*
 * Struct containing all the structs for the data package.
 */
struct data_package{
	struct digital_inputs DIS;
	struct digital_outputs DOS;
	struct digital_flags DFS;
	struct shift_register SR;
	struct cursors CRS;
	struct analog_inputs AIS;
	struct analog_outputs AOS;
	struct analog_flags AFS;
};

//Byte positions digital inputs
#define BP_I1_I8 35
#define BP_I9_I16 36
#define BP_I17_I24 37

//Byte positions digital outputs
#define BP_Q1_Q8 39
#define BP_Q9_Q16 40

//Byte positions digital flags
#define BP_M1_M8 41
#define BP_M9_M16 42
#define BP_M17_M24 43
#define BP_M25_M27 44

//Byte positions shift register and cursors
#define BP_S1_S8 45
#define BP_C1_C4 46

//Byte positions analog inputs
#define BP_AI1_LSB 47
#define BP_AI1_MSB 48
#define BP_AI2_LSB 49
#define BP_AI2_MSB 50
#define BP_AI3_LSB 51
#define BP_AI3_MSB 52
#define BP_AI4_LSB 53
#define BP_AI4_MSB 54
#define BP_AI5_LSB 55
#define BP_AI5_MSB 56
#define BP_AI6_LSB 57
#define BP_AI6_MSB 58
#define BP_AI7_LSB 59
#define BP_AI7_MSB 60
#define BP_AI8_LSB 61
#define BP_AI8_MSB 62

//Byte positions analog outputs
#define BP_AQ1_LSB 63
#define BP_AQ1_MSB 64
#define BP_AQ2_LSB 65
#define BP_AQ2_MSB 66

//Byte positions analog flags
#define BP_AM1_LSB 67
#define BP_AM1_MSB 68
#define BP_AM2_LSB 69
#define BP_AM2_MSB 70
#define BP_AM3_LSB 71
#define BP_AM3_MSB 72
#define BP_AM4_LSB 73
#define BP_AM4_MSB 74
#define BP_AM5_LSB 75
#define BP_AM5_MSB 76
#define BP_AM6_LSB 77
#define BP_AM6_MSB 78

//Bit positions digital inputs
#define B_I1	0b00000001
#define B_I2 	0b00000010
#define B_I3 	0b00000100
#define B_I4 	0b00001000
#define B_I5 	0b00010000
#define B_I6 	0b00100000
#define B_I7 	0b01000000
#define B_I8 	0b10000000
#define B_I9 	0b00000001
#define B_I10	0b00000010
#define B_I11 	0b00000100
#define B_I12 	0b00001000
#define B_I13 	0b00010000
#define B_I14 	0b00100000
#define B_I15 	0b01000000
#define B_I16 	0b10000000
#define B_I17 	0b00000001
#define B_I18 	0b00000010
#define B_I19 	0b00000100
#define B_I20 	0b00001000
#define B_I21 	0b00010000
#define B_I22 	0b00100000
#define B_I23 	0b01000000
#define B_I24 	0b10000000

//Bit positions digital outputs
#define B_Q1 	0b00000001
#define B_Q2 	0b00000010
#define B_Q3 	0b00000100
#define B_Q4 	0b00001000
#define B_Q5 	0b00010000
#define B_Q6 	0b00100000
#define B_Q7 	0b01000000
#define B_Q8 	0b10000000
#define B_Q9 	0b00000001
#define B_Q10 	0b00000010
#define B_Q11 	0b00000100
#define B_Q12 	0b00001000
#define B_Q13 	0b00010000
#define B_Q14 	0b00100000
#define B_Q15 	0b01000000
#define B_Q16 	0b10000000

//Bit positions digital flags
#define B_M1	0b00000001
#define B_M2 	0b00000010
#define B_M3 	0b00000100
#define B_M4 	0b00001000
#define B_M5 	0b00010000
#define B_M6 	0b00100000
#define B_M7 	0b01000000
#define B_M8 	0b10000000
#define B_M9 	0b00000001
#define B_M10	0b00000010
#define B_M11 	0b00000100
#define B_M12 	0b00001000
#define B_M13 	0b00010000
#define B_M14 	0b00100000
#define B_M15 	0b01000000
#define B_M16 	0b10000000
#define B_M17 	0b00000001
#define B_M18 	0b00000010
#define B_M19 	0b00000100
#define B_M20 	0b00001000
#define B_M21 	0b00010000
#define B_M22 	0b00100000
#define B_M23 	0b01000000
#define B_M24 	0b10000000
#define B_M25	0b00000001
#define B_M26	0b00000010
#define B_M27	0b00000100

/*
 * Fill the given package with the given data
 * Returns -1 when the given data is too small. Otherwise returns 1;
 */
char fill_data_package(struct data_package *package, unsigned char data[]);

/*
 * Print the given package with debug_message defined in logger.h.
 */
void print_data_package(struct data_package *package);

void average_package(struct data_package *package, struct data_package *packages, int count);

#endif /* DATA_PACKAGE_H_ */
