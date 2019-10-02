/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * The is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

int color, warnings_disabled;

void i_output ( char *message, const char *status )
{

	append_log(message);
	
	char *use_color = get_value_from_json(config, "color");
	color = atoi(use_color);

	char *silence_warnings_str = get_value_from_json(config, "silence-warnings");
	warnings_disabled = atoi(silence_warnings_str);

	// Bash colors
	// 0 - Reset, 1 - Bold,
	// 2 - Red,   3 - Green,
	// 4 - Yellow
	char *colors[] = {
		"\033[0m",  "\033[1m",
		"\033[31m", "\033[32m", "\033[33m"
	};

	char *prefixes[] = {"", "", ""};

	if ( color )
	{

		char tmp_warn[30];
		sprintf( tmp_warn, "%s%s[ DBG ]%s  ", colors[4], colors[1], colors[0] );
		prefixes[0] = tmp_warn;
		
		char tmp_err[30];
		sprintf( tmp_err, "%s%s[ ERR ]%s  ", colors[2], colors[1], colors[0] );
		prefixes[1] = tmp_err;

		char tmp_ok[30];
		sprintf( tmp_ok, "%s%s[ OK ]%s   ", colors[3], colors[1], colors[0] );
		prefixes[2] = tmp_ok;

	}

	else
	{
		prefixes[0] = "[ DBG ]  ";
		prefixes[1] = "[ ERR ]  ";
		prefixes[2] = "[ OK ]   ";

	}

	int is_warning = strcmp(status, "debug");
	if (is_warning != 0) is_warning = strcmp(status, "warning");

	int is_error = strcmp(status, "error");

	// Create message.
	char message_ready[] = {};
	if ( is_warning == 0 )
		//printf("%s\n", prefixes[0]);
		sprintf(message_ready, "%s%s\n", prefixes[0], message);

	else if ( is_error == 0 )
		//printf("%s\n", prefixes[1]);
		sprintf(message_ready, "%s%s\n", prefixes[1], message);

	else
		sprintf(message_ready, "%s%s\n", prefixes[2], message);


	if ( !warnings_disabled || is_warning )
		printf("%s", message_ready);

}

void display_splash ( int use_color )
{

	if ( !use_color )

		printf(
			"\n%s\n%s%s\n%s\n%s\n%s\n\n",
			"              #                           ",
			"  ###  # ##        ####   ##   # #    ### ", VERSION,
			" #  #  ##     #      #   #  #  ## #  #  # ",
			" # ##  #      #     #    #  #  #  #  # ## ",
			"  # #  #      #    ####   ##   #  #   # # "
		);

	else

		printf(
			"\n%s\n%s%s\n%s\n%s\n%s\n\n",
			"           \033[32m   #                           ",
			"\033[31m  ###\033[33m  # ##    \033[36m    ####\033[34m   ## \033[35m  # # \033[31m   ### \033[0m", VERSION,
			"\033[31m #  #\033[33m  ##  \033[32m   #\033[36m      # \033[34m  #  #\033[35m  ## #\033[31m  #  # ",
			"\033[31m # ##\033[33m  #   \033[32m   #\033[36m     #  \033[34m  #  #\033[35m  #  #\033[31m  # ## ",
			"\033[31m  # #\033[33m  #   \033[32m   #\033[36m    ####\033[34m   ## \033[35m  #  #\033[31m   # # \033[0m"
		);

}
