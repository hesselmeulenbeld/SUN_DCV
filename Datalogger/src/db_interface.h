/*
 * db_interface.h
 *
 *  Created on: Jun 19, 2014
 *      Author: hessel
 */

#ifndef DB_INTERFACE_H_
#define DB_INTERFACE_H_

#include <rrd.h>

#include "data_package.h"

#define DB_LOCATION "/home/hessel/Afstuderen/db/data.rrd"

char rrd_insert(struct data_package *package);


#endif /* DB_INTERFACE_H_ */
