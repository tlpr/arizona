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
MYSQL *connection;

void queue_init ()
{

	connection = mysql_init(NULL);

	char *db_user = get_value_from_json ( config, "db_user" );
	char *db_pass = get_value_from_json ( config, "db_pass" );
	char *db_host = get_value_from_json ( config, "db_host" );
	char *db_name = get_value_from_json ( config, "db_name" );

	i_output("Connecting to MySQL...", "warning");

	if (mysql_real_connect( connection, db_host, db_user, db_pass, db_name, 0, NULL, 0 ) == NULL )
		i_output("Could not connect to the MySQL server.", "error");

}

char *get_next_song ()
{

	char *sql_query = "SELECT `song` FROM `queue` ORDER BY `id` DESC LIMIT 1";

	return sql_query;

}


