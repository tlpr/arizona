/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * The is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

#include <string.h>
#include <stdio.h>
#include <shout/shout.h>
#include <time.h>

#include "configuration.h"
struct json_object *config;

#include "debug.h"
#include "version.h"
#include "dir.h"
#include "queue.h"
#include "stream.h"

char mesg[100];

int main ( int argc, char* argv[] )
{

	char *config_path = "";

	if ( argc > 1 )
	{
	
		if ( (strcmp( argv[1], "--use-config" ) == 0) || (strcmp( argv[1], "-c" ) == 0))
		{

			if ( argc < 3 )
			{
				printf("%s\n", "Path not specified.");
				exit(0);
			}

			else
				config_path = argv[2];
		}

		else if ( (strcmp( argv[1], "--version" ) == 0) || (strcmp( argv[1], "-v" ) == 0) )
		{
		
			printf("Arizona %s\n", VERSION);
			printf("%s\n", "https://github.com/tlpr/arizona");
			exit(0);
		
		}

	}

	config = read_configuration(config_path);
	sprintf(mesg, "Running Arizona %s", VERSION);
	i_output(mesg, "ok");	

	// Use queue in MySQL database.
	char* use_mysql_char = get_value_from_json(config, "use-mysql");
	int use_mysql = atoi(use_mysql_char);

	if ( use_mysql )
	{

		i_output("use-mysql in configuration is enabled, using MySQL queues...", "warning");
		queue_init();

	}

	else
		i_output("use-mysql in configuration is disabled, skipping MySQL init...", "warning");

	// Icecast
	stream_init();
	run_stream();

	return 0;

}

