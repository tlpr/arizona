/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * This is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

#include <stdlib.h>
#include <mysql.h>

const char* previous_song;
char next_song[50][100];
MYSQL *connection;

void queue_init ()
{

	char dmesg[150];

	connection = mysql_init(NULL);
	if ( !connection )
	{
		i_output( "Failed to initialize MySQL, possibly out of memory?", "error" );
		return;
	}

	char *db_user = get_value_from_json ( config, "db-user" );
	char *db_pass = get_value_from_json ( config, "db-pass" );
	char *db_host = get_value_from_json ( config, "db-host" );
	char *db_name = get_value_from_json ( config, "db-name" );
	char *db_praw = get_value_from_json ( config, "db-port" ); // port raw string

	int db_port = atoi(db_praw); // Convert char* to int

	sprintf( dmesg, "Connecting to MySQL at %s:%d", db_host, db_port );
	i_output(dmesg, "warning");

	if ( mysql_real_connect( connection, db_host, db_user, db_pass, db_name, db_port, NULL, 0 ) == NULL )
	{
		i_output("Could not connect to the MySQL server.", "error");
		exit(0);
	}

	else
		i_output("Connected to MySQL!", "ok");

}

char* get_next_song ()
{

	char dmesg[150];

	if ( !connection )
	{
		i_output( "Uninitialized MYSQL object. Use queue_init() first.", "error" );
		exit(0);
	}

	char *sql_query = "SELECT * FROM `queue` ORDER BY `id` ASC LIMIT 1";

	sprintf( dmesg, "Executing SQL query: %s", sql_query );
	i_output( dmesg, "warning" );

	mysql_query (connection, sql_query);
	MYSQL_RES *result = mysql_store_result (connection);
	if ( result == NULL )
	{

		sprintf (dmesg, "Could not retrieve data from MySQL\nError: %s", mysql_error(connection));
		i_output(dmesg, "error");
		return "error";

	}

	MYSQL_ROW row;
	int i_array = 0;

	int num_fields = mysql_num_fields (result);
	while ( (row = mysql_fetch_row (result)) )
	{
	
		for ( int i = 0; i < num_fields; i++ )
		{
			sprintf (&next_song[i_array][0], "%s", row[i]);
			i_array++;
		}

	}


	char * song_id = &next_song[0][0];

	if ( strcmp(&next_song[1][0], "") != 0 )
	{

		char sql_delete_query[ sizeof( song_id ) + 30 ] = { };
		sprintf( sql_delete_query, "DELETE FROM `queue` WHERE `id` = %s", song_id );

		sprintf( dmesg, "Executing SQL query: %s", sql_delete_query );
		i_output( dmesg, "warning" );

		mysql_query (connection, sql_delete_query);
		mysql_store_result (connection);

	}

	return &next_song[1][0];

}


void sort_array ( char** directory_listing, int file_count )
{

	i_output("Shuffling the playlist...", "warning");

	if ( file_count > 1 )
	{
		
		size_t i;
		for ( i = 0; i < (file_count - 1); i++ )
		{
			size_t j = i + rand() / (RAND_MAX / (file_count - i) + 1);
			char * t = directory_listing[j];
			directory_listing[j] = directory_listing[i];
			directory_listing[i] = t;
				
		}
		
	}
		
	else
		i_output("Directory is less than 2 files. Shuffling not required.", "warning");

	
	i_output("Sorted!", "warning");

}
