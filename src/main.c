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


int main ()
{

	config = read_configuration();

	char mesg[30];
	sprintf(mesg, "Running Arizona version %s", VERSION);
	i_output (mesg, "ok");

	queue_init();
	char *next_song;
	next_song = get_next_song();
	printf("%s\n", next_song);

	return 0;

}

