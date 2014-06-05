/*
 * c_config.h
 *
 *  Created on: 2014年6月4日
 *      Author: chenhaipeng
 */

#ifndef C_CONFIG_H_
#define C_CONFIG_H_

#include "c_cachedb.h"

int load_server_config(struct c_cachedb_server *server, const char *config_file, char *errmsg);

#endif /* C_CONFIG_H_ */
