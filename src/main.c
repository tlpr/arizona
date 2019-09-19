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


int main ()
{

	config = read_configuration();
	i_output ("Arizona", "ok");
	return 0;

}

