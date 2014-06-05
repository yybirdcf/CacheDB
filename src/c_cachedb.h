#ifndef C_CACHEDB_H_
#define C_CACHEDB_H_

#include "c_list.h"
#include "c_dict.h"

#define PRIMARY 0x01
#define AUTO_INCEMENT 0x02

typedef char** ROW;

struct c_cachedb_tb
{
	dict *kv_dict;
	dict *k_stat;
};

struct c_cachedb_db
{
	struct c_cachedb_tb *tbs;
};

struct c_mysql_db
{
	char *ip;
	unsigned short port;
	char *user;
	char *pwd;
};

struct c_slowlog
{
	int slowlog_size;
	int slowlog_max_time;
	list *slowlog_list;
};

struct c_slowlog_record
{
	time_t exec_at;
	int exec_time;
	char *query;
};

struct c_cachedb_stat
{
	time_t start_at;
	long long mem_used;
	long long cached_keys;

	long long select_times;
	long long insert_times;
	long long update_times;
	long long delete_times;
};

struct c_cachedb_table_field
{
	char *field;
	char *type;
	unsigned short size;
	char *default_val;
	char role;
};

struct c_cachedb_table_conf
{
	int num_fields;
	struct c_cachedb_table_field **table_fields;
};

union c_condition_where_val
{
	long long l_val;
	char *str_val;
	struct {
		int *i_arr;
		int i_num;
	} *i_arr_val;

	struct {
		char **str_arr;
		int str_num;
	} *str_arr_val;
};

struct c_condition_where
{
	char *field;
	char *op;
	union c_condition_where_val *where_val;
};

struct c_cmd_select
{
	char **fields;
	int num_fields;
    char *key_val;
    char *sort_by;
    struct c_condition_where **where_conditions;
    int num_conditions;
    int limit;
};

struct c_cmd_insert
{
	char **fields;
	char **values;
	int num_fields;
};

struct c_cmd_update
{
	char **fields;
	char **values;
	int num_fields;
	char *key_val;
	struct c_condition_where **where_conditions;
	int num_conditions;
};

struct c_cmd_delete
{
	char *key_val;//keyֵ
	struct c_condition_where **where_conditions;
	int num_conditions;
};

struct c_cachedb_server
{
	int port;
	char *bind_addr;

	long long mem_max;
	char *cache_key;

	int sharding_db;
	int sharding_tb;

	struct c_cachedb_db *dbs;

	struct c_mysql_db *master;
	struct c_mysql_db **slave;
	int slave_db_num;

	struct c_slowlog *slowlog;
	int tcp_sock_fd;//tcp socket

	list *clients;
	list *clients_to_close;

	struct c_cachedb_stat *stat;

	struct c_cachedb_table_conf *table_conf;
};

#endif /* C_CACHEDB_H_ */
