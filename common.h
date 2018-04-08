#pragma once

namespace Common
{
	void die_error(const char *err_msg);
	void InitializeAllTables(MYSQL *conn);
	char *adher_get_time();
	void create_sectors(char *file_name);
	void InsertSectors(MYSQL *conn);
	void InsertActivities(MYSQL *conn);
	void AfficheOptions();
};
