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

#include "configuration.h"
struct json_object *config;

#include "debug.h"
#include "version.h"
#include "queue.h"
#include "stream.h"

char mesg[100];

int main ()
{

	config = read_configuration();
	
	// Use queue in MySQL database.
	char* use_mysql_char = get_value_from_json(config, "use-mysql");
	int use_mysql = atoi(use_mysql_char);

	sprintf(mesg, "Running Arizona version %s", VERSION);
	i_output(mesg, "ok");

	if ( use_mysql )
	{

		i_output("use-mysql in cfg.json is enabled, using MySQL queues...", "warning");
	
		queue_init();

		char *next_song;
		next_song = get_next_song();

		sprintf (mesg, "Next song in database queue appears to be: \"%s\"", next_song);
		i_output(mesg, "ok");

	}

	else
		i_output("use-mysql in cfg.json is disabled, skipping MySQL init...", "warning");

	return 0;

}

