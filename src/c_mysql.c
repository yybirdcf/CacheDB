#include <errmsg.h>
#include <my_global.h>
#include <mysql.h>

#include "c_mysql.h"

static MYSQL *con;

void c_mysql_init()
{
	if(!con)
	{
		con = mysql_init(NULL);
		assert(con != NULL);

		my_bool reconnect = 1;
		mysql_options(con, MYSQL_OPT_RECONNECT, &reconnect);
	}
}

int c_mysql_connect(
		const char *host,
		unsigned int port,
		const char *user,
		const char *pwd,
		const char *db)
{
	if(mysql_real_connect(con, host, user, pwd, db, port, NULL, 0) == NULL)
	{
		return mysql_errno(con);
	}
	return 0;
}

int c_mysql_query(const char *sql)
{
	if(mysql_query(con, sql) == 0)
	{
		return 0;
	}
	else if(mysql_errno(con) == CR_SERVER_GONE_ERROR | mysql_errno(con) == CR_SERVER_LOST)
	{
		//try to reconnect
		if(mysql_ping(con) == 0)
		{
			mysql_query(con, sql);
		}
	}

	return mysql_errno(con);
}

long long c_mysql_last_insert_id()
{
	return mysql_insert_id(con);
}

int c_mysql_fetch_fields(const char *sql)
{
	unsigned int num_fields;
	unsigned int i;
	MYSQL_FIELD *fields;

	if(c_mysql_query(sql) != 0)
	{
		return mysql_errno(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result != NULL)
	{
		num_fields = mysql_num_fields(con);
		fields = mysql_fetch_fields(result);
		for(i = 0; i < num_fields; i++)
		{
			//get need information
		   printf("Field %u is %s\n", i, fields[i].name);
		}
		mysql_free_result(result);
		return num_fields;
	}

	return mysql_errno(con);
}

int c_mysql_fetch_results(const char *sql)
{
	if(c_mysql_query(sql) != 0)
	{
		return mysql_errno(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		return mysql_errno(con);
	}

	int count = 0;
	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	while((field = mysql_fetch_field(result)) != NULL)
	{
		printf("%s ", field->name);
	}

	while ((row = mysql_fetch_row(result)))
	{
	  for(int i = 0; i < num_fields; i++)
	  {
		  printf("%s  ", row[i] ? row[i] : "NULL");
	  }
	  count++;
	}

	mysql_free_result(result);
	return count;
}
