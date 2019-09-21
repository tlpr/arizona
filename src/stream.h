/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * This is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

shout_t *shout;


void stream_init ()
{

	shout_init();
	shout = shout_new();

	char* icecast_host = get_value_from_json (config, "shout-host");
	char* icecast_pstr = get_value_from_json (config, "shout-port");
	char* icecast_user = get_value_from_json (config, "shout-user");
	char* icecast_pass = get_value_from_json (config, "shout-pass");
	char* icecast_frmt = get_value_from_json (config, "shout-frmt");
	char* icecast_mnt  = get_value_from_json (config, "shout-mnt" );
	
	char dmesg[100];

	// Print out the warning
	char ic_pass_starred[150];
	int i = 0;
	while (icecast_pass[i] != '\0')
	{
		ic_pass_starred[i] = '*';
		i++;
	}

	sprintf(
		dmesg, "Initializing libshout using http://%s:%s@%s:%s%s",
		icecast_user, ic_pass_starred, icecast_host, icecast_pstr, icecast_mnt
	);

	i_output(dmesg, "warning");

	int icecast_port = atoi(icecast_pstr);
	free(icecast_pstr);

	shout_set_host(shout, icecast_host);
	shout_set_port(shout, icecast_port);

	shout_set_protocol(shout, SHOUT_PROTOCOL_HTTP);

	shout_set_mount(shout, icecast_mnt);
	shout_set_user(shout, icecast_user);
	shout_set_password(shout, icecast_pass);

	if ( strcmp(icecast_frmt, "MP3") == 0 )
	{
		i_output("Setting format MP3", "ok");
		shout_set_format(shout, SHOUT_FORMAT_MP3);
	}

	else if ( strcmp(icecast_frmt, "OGG") == 0 )
	{
		i_output("Setting format OGG", "ok");
		shout_set_format(shout, SHOUT_FORMAT_OGG);
	}

	else
	{
		i_output("Unknown format.", "error");
		exit(0);
	}

}

