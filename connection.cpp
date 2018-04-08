#include <stdio.h>
#include <mysql.h>

#include "connection.h"
#include "common.h"
#include "fields.h"
#include "mem.h"
#include "str.h"
#include "colors.h"

struct Connection::connection_params *Connection::allocation_conn_params(unsigned int taille)
{
	if( taille < 1 )
	{
		return NULL;
	}
	
	struct Connection::connection_params *ret_val = (struct Connection::connection_params*)Mem::_alloc(taille);

	ret_val->server = (char*)Mem::_alloc(TAILLE_SERVER);
	ret_val->user = (char*)Mem::_alloc(TAILLE_USER);
	ret_val->pass = (char*)Mem::_alloc(TAILLE_PASS);
	ret_val->db = (char*)Mem::_alloc(TAILLE_DB);

	return ret_val;
}

void Connection::free_connection_params(struct Connection::connection_params **mem)
{
	struct Connection::connection_params *cp = *mem;
	if( cp != NULL )
	{
		free(cp->server);
		free(cp->user);
		free(cp->pass);
		free(cp->db);
		free(cp);
	}

	return;
}

MYSQL *Connection::initialize_connection(MYSQL *arg, struct Connection::connection_params *conn_params)
{
	MYSQL *conn;
	struct Connection::connection_params *c_params = conn_params;

	if( c_params == NULL )
	{
		c_params = allocation_conn_params(1);

		Str::_CopyA(c_params->server, "localhost", -1);
		Str::_CopyA(c_params->user, "root", -1);
		Str::_CopyA(c_params->pass, "", -1);
		Str::_CopyA(c_params->db, "adher_project", -1);
	}

	conn = mysql_init(NULL);

	if( !mysql_real_connect(conn, c_params->server, c_params->user, c_params->pass, c_params->db, c_params->port, NULL, 0) )
	{
		Common::die_error(mysql_error(conn));
	}

	return conn;
}

MYSQL_RES *Connection::generate_mysql_result(MYSQL *conn, char *query)
{
	if( mysql_query(conn, query) != 0 )
	{
		Common::die_error(mysql_error(conn));
	}

	MYSQL_RES *result;
	result = mysql_store_result(conn);

	return result;
}

int Connection::CreateTable(MYSQL *conn, const char *table_name, char **table_fields, int fields_len)
{
	char *query = (char*)Mem::_alloc(QUERY_SIZE);
	int i = 0;

	Str::_CatA(query, "CREATE TABLE IF NOT EXISTS ", -1);
	Str::_CatA(query, table_name, -1);
	Str::_CatA(query, "(", -1);
	for( i = 0; i < fields_len; ++i )
	{
		Str::_CatA(query, table_fields[i], -1);
		if( i != (fields_len - 1) )
		{
			Str::_CatA(query, ", ", -1);
		}
	}
	Str::_CatA(query, ")", -1);
	printf("query : %s\n\n\n", query);
	
	if( mysql_query(conn, query) != 0 )
	{
		Common::die_error(mysql_error(conn));
	}

	Mem::_free(query);

	return 0;
}

int Connection::AlterTable(MYSQL *conn, const char *table_name, char **table_fields)
{
	char *query = (char*)Mem::_alloc(QUERY_SIZE);
	int i = 0;

	Str::_CatA(query, "ALTER TABLE ", -1);
	Str::_CatA(query, table_name, -1);
	Str::_CatA(query, " ADD CONSTRAINT ", -1);
	Str::_CatA(query, table_fields[0], -1);
	Str::_CatA(query, " FOREIGN KEY (", -1);
	Str::_CatA(query, table_fields[1], -1);
	Str::_CatA(query, ") REFERENCES ", -1);
	Str::_CatA(query, table_fields[2], -1);
	Str::_CatA(query, " (", -1);
	Str::_CatA(query, table_fields[3], -1);
	Str::_CatA(query, ");", -1);

	if( mysql_query(conn, query) != 0 )
	{
		char *err = (char*)mysql_error(conn);

		if( Str::_CompareA(err, "Can't write", 11, 11) != 0 )
		{
			Common::die_error(err);
		}
	}

	Mem::_free(query);

	return 0;
}

int Connection::InsertInto(MYSQL *conn, FILE *src_file, int delim, char *table, int show_all)
{
	int start_from = 1;
	if( show_all )
	{
		start_from = 0;
	}

	char *table_name = NULL;
	int bytes_read = 0;

	if( table != NULL && Str::_LengthA(table) > 0 )
	{
		table_name = (char*)Mem::_alloc(sizeof(char) * TAILLE_TABLE);
		Str::_CopyA(table_name, table, -1);
	}
	else
	{
		printf("Enter the table you would like to insert values in : ");
		Colors::setTextColor(Colors::DARK_WHITE);

		bytes_read = Str::_getdelimitedinput(&table_name, stdin, (char)'\n');
		if( Str::_LengthA(table_name) == 0 )
		{
			Common::die_error("table name can't be empty");
		}
	}

	FILE *fp = NULL;
	if( src_file == NULL )
	{
		fp = stdin;
	}
	else
	{
		fp = src_file;
	}
	
	Colors::setTextColor(Colors::DARK_PURPLE);

	char *query = (char*)Mem::_alloc(QUERY_SIZE);

	Str::_CatA(query, "SELECT * FROM ", -1);
	Str::_CatA(query, table_name, -1);
	
	MYSQL_RES *result = generate_mysql_result(conn, query);

	Mem::_free(query);

	int i = 0;

	query = (char*)Mem::_alloc(QUERY_SIZE);

	Str::_CatA(query, "INSERT INTO `", -1);
	Str::_CatA(query, table_name, -1);
	Str::_CatA(query, "` ", -1);
	
	MYSQL_FIELD *field;
	char *user_input = NULL;

	field = mysql_fetch_field(result);
	
	unsigned int num_cols = mysql_field_count(conn);

	if( num_cols > 0 )
	{

		for( i = start_from; i < num_cols; ++i )
		{
			if( i == start_from )
			{
				Str::_CatA(query, "(", -1);
			}
			Str::_CatA(query, field[i].name, -1);
			if( i != num_cols - 1 )
			{
				Str::_CatA(query, ", ", -1);
			}
			else
			{
				Str::_CatA(query, ") ", -1);
			}
		}
		
		Str::_CatA(query, "VALUES ", -1);
		for( i = start_from; i < num_cols; ++i )
		{
			printf("Enter value for %s : ", field[i].name);
			
			if( i == start_from )
			{
				Str::_CatA(query, "(", -1);
			}
			Str::_CatA(query, "'", -1);

			Colors::setTextColor(Colors::DARK_WHITE);

			Str::_getdelimitedinput(&user_input, fp, (char)delim);
			if( !Str::_LengthA(user_input) )
			{
				Common::die_error("Values can not be empty\n");
			}

			Colors::setTextColor(Colors::DARK_PURPLE);

			Str::_CatA(query, user_input, -1);
			if( i != num_cols - 1 )
			{
				Str::_CatA(query, "', ", -1);
			}
			else
			{
				Str::_CatA(query, "'", -1);
			}

			Mem::_free(user_input);
			user_input = NULL;
		}
	}
	Str::_CatA(query, ")", -1);

	printf("query = %s\n\n\n", query);

	if( mysql_query(conn, query) != 0 )
	{
		char *err = (char*)mysql_error(conn);
		if( Str::_CompareA(err, "Duplicate", 9, 9) != 0 )
		{
			Common::die_error(mysql_error(conn));
		}
	}

	mysql_free_result(result);
	Mem::_free(query);

	return 0;
}

int Connection::SelectFrom(MYSQL *conn, char *table, int begin, int end)
{
	int status;
	char *table_name;

	char *query = (char*)Mem::_alloc(QUERY_SIZE);

	Str::_CatA(query, "SELECT * FROM ", -1);
	Str::_CatA(query, table, -1);
	
	MYSQL_RES *result = generate_mysql_result(conn, query);

	Mem::_free(query);
	
	MYSQL_FIELD *field = mysql_fetch_field(result);
	MYSQL_ROW row = mysql_fetch_row(result);

	unsigned int field_count = mysql_field_count(conn);
	long long row_count = mysql_num_rows(result);
	long long upto = row_count;

	if( end < 1 )
	{
		upto = mysql_num_rows(result);
	}
	else
	{
		upto = end;
	}

	int offset = begin >= 0 ? begin : (row_count + begin);
	mysql_data_seek(result, offset);

	int i = 0;
	while( ( row = mysql_fetch_row(result) ) && upto > 0 )
	{
		for( i = 0; i < field_count; ++i )
		{
			printf("%s  ", row[i]);
		}
		printf("\n");
		upto--;
	}

	mysql_free_result(result);

	return 0;
}

void Connection::IncomingCall()
{
	int max_ring = 5, min_ring = 2;
	int ring_time = rand() % (max_ring - min_ring) + min_ring;

	Sleep(max_ring * 700);

	return;
}
