#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mysql.h>

#include "colors.h"
#include "str.h"
#include "mem.h"
#include "fields.h"
#include "connection.h"
#include "common.h"

int main()
{
	srand(time(NULL));

	Mem::Init();
	Colors::Init();
	
	Colors::setTextColor(Colors::DARK_PURPLE);
	
	MYSQL *conn = Connection::initialize_connection(NULL, NULL);
	
	// ##### INIT ALL TABLES ######
	Common::InitializeAllTables(conn);
	Common::create_sectors("sectors.txt");
	Common::InsertSectors(conn);
	Common::InsertActivities(conn);
	Colors::setTextColor(Colors::DARK_GREEN);
	
	fprintf(stderr, "successfully created all tables\n\n");

	char *response = NULL;
	int response_len = 0;

	do
	{
		Colors::setTextColor(Colors::DARK_PURPLE);
		printf("\n\nCurrent time is %s\n", Common::adher_get_time());
		printf("\n");

		Common::AfficheOptions();

		Mem::_free(response);
		response_len = Str::_getdelimitedinput(&response, stdin, (int)'\n');
		
		switch( response[0] )
		{
			case '1':
			{
				char *table_name = NULL;
				int name_len = 0;

				printf("Entrer le nom du table que vous voulez afficher : ");
				name_len = Str::_getdelimitedinput(&table_name, stdin, (int)'\n');
				
				Colors::setTextColor(Colors::LIGHT_CYAN);
				Connection::SelectFrom(conn, table_name, 0, -1);
				Colors::setTextColor(Colors::DARK_PURPLE);
				printf("\n\n");
				break;
			}
			case '2':
			{
				printf("Appel en cours....");
				Connection::IncomingCall();
				printf("Vous etes connecte\n");

				// ##### insert into ANY TABLE ####
				Connection::InsertInto(conn, NULL, (int)'\n', "client", 0);

				break;
			}
			default:
			{
				break;
			}
		}
	}while( Str::_CompareA(response, "/adher_quit", 11, 11) != 0 );
	
	mysql_close(conn);

	getchar();
	return 0;
}
