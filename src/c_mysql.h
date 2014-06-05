#ifndef C_MYSQL_H_
#define C_MYSQL_H_

void c_mysql_init();

int c_mysql_connect(
		const char *host,
		unsigned int port,
		const char *user,
		const char *pwd,
		const char *db);

int c_mysql_query(const char *sql);

long long c_mysql_last_insert_id();

int c_mysql_fetch_fields(const char *sql);

int c_mysql_fetch_results(const char *sql);

#endif /* C_MYSQL_H_ */
