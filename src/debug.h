/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * The is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

int color;

void i_output ( char *message, char *status )
{

	char *use_color = get_value_from_json(config, "color");
	color = ( use_color - "0" );

	// Bash colors
	// 0 - Reset, 1 - Bold,
	// 2 - Red,   3 - Green,
	// 4 - Yellow
	char *colors[] = {
		"\033[0m",  "\033[1m",
		"\033[31m", "\033[32m", "\033[33m"
	};

	char *prefixes[] = {"", "", ""};


	// sprintf() causes segmentation fault, temporary workaround is disabling colors.
	color = 0;
	// --- --- --- ---

	if ( color )
	{

	//	-- TO BE FIXED (:30) --
	//	sprintf( prefixes[0], "[Warn] " );
	//	sprintf( prefixes[1], "[Err] " );
	//	sprintf( prefixes[2], "[OK] " );

	}

	else
	{
		prefixes[0] = "[Warning] ";
		prefixes[1] = "[Error] ";
		prefixes[2] = "[OK] ";

	}

	// Create message.
	char message_ready[] = {};
	if ( strcmp(status, "warning") == 0 )
		sprintf(message_ready, "%s%s\n", prefixes[0], message);

	else if ( strcmp(status, "error") == 0 )
		sprintf(message_ready, "%s%s\n", prefixes[1], message);

	else
		sprintf(message_ready, "%s%s\n", prefixes[2], message);


	printf("%s", message_ready);

}

