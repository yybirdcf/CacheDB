/*
 * c_config.c
 *
 *  Created on: 2014年6月4日
 *      Author: chenhaipeng
 */

#include "c_config.h"

#include <stdio.h>

#define ERROR_OPEN_FILE 1000

#define DOING_HOST 1
#define DOING_CACHE 2
#define DOING_STAT 3
#define DOING_MYSQL_MASTER 4
#define DOING_MYSQL_SLAVE 5
#define DOING_DATABASE 6
#define DOING_TABLE 7

#define BUFF_SIZE 81

static char *tr(char *s) {
	int i = 0;
	int j = strlen(s) - 1;
	int k = 0;

	while (isspace(s[i]) && s[i] != '\0')
		i++;

	while (isspace(s[j]) && j >= 0)
		j--;

	while (i <= j)
		s[k++] = s[i++];

	s[k] = '\0';

	return s;
}

static char *get_val(char *buffer) {
	//找到第一个＝和＃位置
	char *eq_pos = strchr(buffer, '=');
	char *end_pos = strchr(buffer, '#');
	if (end_pos == 0) {
		end_pos = strchr(buffer, '\n');
	}

	char *val;
	strncpy(val, eq_pos + 1, end_pos - eq_pos);
	return tr(val);
}

int load_server_config(struct c_cachedb_server *server, const char *config_file,
		char *errmsg) {
	char buffer[BUFF_SIZE];

	FILE *fp = fopen("table.conf", "r");
	if (fp == NULL) {
		fprintf(errmsg, "can not open the config file %s", config_file);
		return ERROR_OPEN_FILE;
	}

	int doing = 0;

	while (fgets(buffer, BUFF_SIZE, fp) != NULL) {
		if (buffer[0] == '#' || buffer[0] == '\n')
			continue;

		if (buffer[0] == '[') {
			printf("%d", 1);
			char *pos = strchr(buffer, ']');
			char *dest;
			int n = pos - buffer - 1;
			printf("%d", n);
			strncpy(dest, buffer + 1, pos - buffer);
			dest[n] = '\0';

			if (strcmp(dest, "host")) {
				doing = DOING_HOST;
			} else if (strcmp(dest, "cache")) {
				doing = DOING_CACHE;
			} else if (strcmp(dest, "stat")) {
				doing = DOING_STAT;
			} else if (strcmp(dest, "mysql-master")) {
				doing = DOING_MYSQL_MASTER;
			} else if (strcmp(dest, "mysql-slave")) {
				doing = DOING_MYSQL_SLAVE;
			} else if (strcmp(dest, "database")) {
				doing = DOING_DATABASE;
			} else if (strcmp(dest, "table")) {
				doing = DOING_TABLE;
			}
		}

		switch (doing) {
		case DOING_HOST:
		{
			char *pos = strstr(buffer, "host_ip");
			if (pos > 0) {
				//处理host_ip
				char *val = get_val(buffer);
				printf("host ip:%s", val);
			}

			pos = strstr(buffer, "host_port");
			if (pos > 0) {
				//处理host_port
			}
		}
		break;
		case DOING_CACHE:
			break;
		case DOING_STAT:
			break;
		case DOING_MYSQL_MASTER:
			break;
		case DOING_MYSQL_SLAVE:
			break;
		case DOING_DATABASE:
			break;
		case DOING_TABLE:
			break;
		default:
			break;
		}

	}

	fclose(fp);
	return 0;
}
