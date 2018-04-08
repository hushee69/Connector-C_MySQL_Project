#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>

#include "common.h"
#include "colors.h"
#include "str.h"
#include "mem.h"
#include "fields.h"
#include "connection.h"

static char *secteur_fields[] = {"code_secteur varchar(255) NOT NULL", "nom_secteur varchar(255) NOT NULL", "PRIMARY KEY(code_secteur)"};

static char *client_fields[] = {"code_client int NOT NULL AUTO_INCREMENT", "nom_client varchar(255)", "prenom_client varchar(255)", "ville_client varchar(255) NOT NULL", "rue_client varchar(255) NOT NULL", "num_tel_client varchar(15) NOT NULL", "PRIMARY KEY(code_client)"};

static char *adherant_fields[] = {"num_adherant int NOT NULL AUTO_INCREMENT", "nom_adherant varchar(255) NOT NULL", "prenom_adherant varchar(255) NOT NULL", "rue_adherant varchar(255) NOT NULL", "ville_adherant varchar(255) NOT NULL", "code_postal_adherant varchar(10) NOT NULL", "disponibilite int NOT NULL", "PRIMARY KEY(num_adherant)"};

static char *activite_fields[] = {"code_activite int NOT NULL AUTO_INCREMENT", "nom_activite varchar(255) NOT NULL", "PRIMARY KEY(code_activite)"};

static char *contrat_fields[] = {"num_contrat int NOT NULL AUTO_INCREMENT", "code_secteur varchar(255) NOT NULL", "num_adherant int NOT NULL", "date_debut_contrat date NOT NULL", "date_fin_contrat date NOT NULL", "tarif_ht int NOT NULL", "tarif_ttc int NOT NULL", "PRIMARY KEY(num_contrat)"};

static char *heure_rappel_fields[] = {"code_heure_rappel int NOT NULL AUTO_INCREMENT", "code_appel int NOT NULL", "heure_rappel TIME NOT NULL", "date_rappel date NOT NULL", "PRIMARY KEY(code_heure_rappel)"};

static char *appel_fields[] = {"code_appel int NOT NULL", "code_client int NOT NULL", "date_appel date NOT NULL", "heure_rappel time NOT NULL", "PRIMARY KEY(code_appel)"};

static char *etablir_fields[] = {"num_contrat int NOT NULL", "code_appel int NOT NULL", "PRIMARY KEY(num_contrat, code_appel)"};

static char *concerner_fields[] = {"code_activite int NOT NULL", "num_contrat int NOT NULL", "PRIMARY KEY(code_activite, num_contrat)"};

static char *effectuer_fields[] = {"code_appel int NOT NULL", "num_contrat int NOT NULL", "PRIMARY KEY(code_appel, num_contrat)"};

static char *appartient_fields[] = {"code_activite int NOT NULL", "num_adherant int NOT NULL", "PRIMARY KEY(code_activite, num_adherant)"};

static char *contrat_foreign_key1[] = {"FK_CONTRAT_SECTEUR", "CODE_SECTEUR", "SECTEUR", "CODE_SECTEUR"};

static char *contrat_foreign_key2[] = {"FK_CONTRAT_ADHERANT", "NUM_ADHERANT", "ADHERANT", "NUM_ADHERANT"};

static char *heure_rappel_foreign_key1[] = {"FK_HEURE_RAPPEL", "CODE_APPEL", "APPEL", "CODE_APPEL"};

static char *appel_foreign_key1[] = {"FK_APPEL_CLIENT", "CODE_CLIENT", "CLIENT", "CODE_CLIENT"};

static char *etablir_foreign_key1[] = {"FK_ETABLIR_CONTRAT", "NUM_CONTRAT", "CONTRAT", "NUM_CONTRAT"};

static char *etablir_foreign_key2[] = {"FK_ETABLIR_APPEL", "CODE_APPEL", "APPEL", "CODE_APPEL"};

static char *concerner_foreign_key1[] = {"FK_CONERNER_ACTIVITE", "CODE_ACTIVITE", "ACTIVITE", "CODE_ACTIVITE"};

static char *concerner_foreign_key2[] = {"FK_CONCERNER_CONTRAT", "NUM_CONTRAT", "CONTRAT", "NUM_CONTRAT"};

static char *effectuer_foreign_key1[] = {"FK_EFFECTUER_APPEL", "CODE_APPEL", "APPEL", "CODE_APPEL"};

static char *effectuer_foreign_key2[] = {"FK_EFFECTUER_CONTRAT", "NUM_CONTRAT", "CONTRAT", "NUM_CONTRAT"};

static char *appartient_foreign_key1[] = {"FK_APPARTIENT_ACTIVITE", "CODE_ACTIVITE", "ACTIVITE", "CODE_ACTIVITE"};

static char *appartient_foreign_key2[] = {"FK_APPARTIENT_ADHERANT", "NUM_ADHERANT", "ADHERANT", "NUM_ADHERANT"};

void Common::die_error(const char *err_msg)
{
	Colors::setTextColor(Colors::LIGHT_RED);
	printf("ADHER ERREUR : %s\n", err_msg);
	Colors::setTextColor(Colors::RESET);

	getchar();
	exit(-1);
}

void Common::InitializeAllTables(MYSQL *conn)
{
	int status;
	char *table = (char*)Mem::_alloc(TAILLE_TABLE);

	// #### create table SECTEUR ####
	Str::_CopyA(table, "secteur", -1);
	status = Connection::CreateTable(conn, table, secteur_fields, SECTEUR_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table CLIENT #####
	Str::_CopyA(table, "client", -1);
	status = Connection::CreateTable(conn, table, client_fields, CLIENTS_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table ADHERANT #####
	Str::_CopyA(table, "adherant", -1);
	status = Connection::CreateTable(conn, table, adherant_fields, ADHERANT_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table ACTIVITE #####
	Str::_CopyA(table, "activite", -1);
	status = Connection::CreateTable(conn, table, activite_fields, ACTIVITE_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table CONTRAT #####
	Str::_CopyA(table, "contrat", -1);
	status = Connection::CreateTable(conn, table, contrat_fields, CONTRAT_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table HEURE_RAPPEL #####
	Str::_CopyA(table, "heure_rappel", -1);
	status = Connection::CreateTable(conn, table, heure_rappel_fields, HEURE_RAPPEL_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table APPEL #####
	Str::_CopyA(table, "appel", -1);
	status = Connection::CreateTable(conn, table, appel_fields, APPEL_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table ETABLIR #####
	Str::_CopyA(table, "etablir", -1);
	status = Connection::CreateTable(conn, table, etablir_fields, ETABLIR_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table CONCERNER #####
	Str::_CopyA(table, "concerner", -1);
	status = Connection::CreateTable(conn, table, concerner_fields, CONCERNER_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table EFFECTUER #####
	Str::_CopyA(table, "effectuer", -1);
	status = Connection::CreateTable(conn, table, effectuer_fields, EFFECTUER_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### create table APPARTIENT #####
	Str::_CopyA(table, "appartient", -1);
	status = Connection::CreateTable(conn, table, appartient_fields, APPARTIENT_FIELDS_SIZE);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// ALTERING TABLE STARTS HERE
	/*
	// #### alter table CONTRAT #####
	Str::_CopyA(table, "contrat", -1);
	status = Connection::AlterTable(conn, table, contrat_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table CONTRAT #####
	Str::_CopyA(table, "contrat", -1);
	status = Connection::AlterTable(conn, table, contrat_foreign_key2);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table HEURE_RAPPEL #####
	Str::_CopyA(table, "heure_rappel", -1);
	status = Connection::AlterTable(conn, table, heure_rappel_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table APPEL #####
	Str::_CopyA(table, "appel", -1);
	status = Connection::AlterTable(conn, table, appel_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table ETABLIR #####
	Str::_CopyA(table, "etablir", -1);
	status = Connection::AlterTable(conn, table, etablir_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table ETABLIR #####
	Str::_CopyA(table, "etablir", -1);
	status = Connection::AlterTable(conn, table, etablir_foreign_key2);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table CONCERNER #####
	Str::_CopyA(table, "concerner", -1);
	status = Connection::AlterTable(conn, table, concerner_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table CONCERNER #####
	Str::_CopyA(table, "concerner", -1);
	status = Connection::AlterTable(conn, table, concerner_foreign_key2);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table EFFECTUER #####
	Str::_CopyA(table, "effectuer", -1);
	status = Connection::AlterTable(conn, table, effectuer_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table EFFECTUER #####
	Str::_CopyA(table, "effectuer", -1);
	status = Connection::AlterTable(conn, table, effectuer_foreign_key2);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	// #### alter table APPARTIENT #####
	Str::_CopyA(table, "appartient", -1);
	status = Connection::AlterTable(conn, table, appartient_foreign_key1);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	
	
	// #### alter table APPARTIENT #####
	Str::_CopyA(table, "appartient", -1);
	status = Connection::AlterTable(conn, table, appartient_foreign_key2);
	if( status != 0 )
	{
		Common::die_error(mysql_error(conn));
	}
	Mem::_zero(table, Str::_LengthA(table));
	*/

	return;
}

char *Common::adher_get_time()
{
	int time_len = 26;
	time_t now;
	struct tm time_struct;
	char *time_str = (char*)Mem::_alloc(sizeof(char) * time_len);

	time(&now);
	localtime_s(&time_struct, &now);

	if( asctime_s(time_str, time_len, &time_struct) != 0 )
	{
		Common::die_error("error getting time");
	}

	return time_str;
}

void Common::create_sectors(char *file_name)
{
	int bytes_written = 0, sector_num = 0, i = 0;
	FILE *fp = fopen(file_name, "w");
	if( fp == NULL )
	{
		die_error("couldn't create sectors");
	}

	for( i = 0; i < 20; ++i )
	{
		fprintf(fp, "%02d\nAubiere\n", (i + 1));
	}

	for( i = 20; i < 40; ++i )
	{
		fprintf(fp, "%02d\nClermont\n", (i + 1));
	}

	for( i = 40; i < 60; ++i )
	{
		fprintf(fp, "%02d\nVichy\n", (i + 1));
	}

	for( i = 60; i < 80; ++i )
	{
		fprintf(fp, "%02d\nRiom\n", (i + 1));
	}

	fclose(fp);

	return;
}

void Common::InsertSectors(MYSQL *conn)
{
	FILE *sector_file = fopen("sectors.txt", "r");
	int i = 0;

	for( i =0; i < NUMBER_OF_SECTORS; ++i )
	{
		Connection::InsertInto(conn, sector_file, (int)'\n', "secteur", 1);
	}

	fclose(sector_file);

	return;
}

void Common::InsertActivities(MYSQL *conn)
{
	FILE *activities_file = fopen("activities.txt", "r");
	if( activities_file == NULL )
	{
		die_error("couldn't open activities file");
	}

	int i = 0;

	for( i = 0; i < NUMBER_OF_ACTIVITIES; ++i )
	{
		Connection::InsertInto(conn, activities_file, (int)'\n', "activite", 1);
	}

	fclose(activities_file);

	return;
}

void Common::AfficheOptions()
{
	printf("Qu'est-ce que vous voulez faire ?\n");
	printf("OPTIONS\n");
	printf("1. afficher tableau [nom]\n");
	printf("2. appeler adher\n");
	printf("3. quitter (entrer `/adher_quit` pour quitter\n");

	return;
}
