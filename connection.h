#pragma once

namespace Connection
{
	struct connection_params
	{
		char *server;
		char *user;
		char *pass;
		char *db;
		int port;
	};
	
	struct connection_params *allocation_conn_params(unsigned int taille);
	void free_connection_params(struct connection_params **mem);
	MYSQL *initialize_connection(MYSQL *arg, struct Connection::connection_params *conn_params);
	MYSQL_RES *generate_mysql_result(MYSQL *conn, char *query);
	int CreateTable(MYSQL *conn, const char *table_name, char **table_fields, int fields_len);
	int AlterTable(MYSQL *conn, const char *table_name, char **table_fields);
	int InsertInto(MYSQL *conn, FILE *fp, int delim, char *table, int show_all);
	int SelectFrom(MYSQL *conn, char *table, int begin, int end);
	void IncomingCall();
};
