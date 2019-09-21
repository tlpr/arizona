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
	sprintf(
		dmesg, "Initializing libshout using http://%s:xxxx@%s:%s%s",
		icecast_user, icecast_host, icecast_pstr, icecast_mnt
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


void run_stream ()
{

	char dmesg[100];

	int sopen_status = shout_open(shout);
	if ( sopen_status != SHOUTERR_SUCCESS )
	{
		i_output("An error occured while connecting to Icecast. (maybe wrong data?)", "error");
		exit(0);
	}

	unsigned char buffer[4096];
	size_t read, total;
	int ret;

	char *play_mode = get_value_from_json(config, "song-play-mode");
	sprintf(dmesg, "Selected play mode: %s", play_mode);
	i_output(dmesg, "ok");

	char *audio_dir = get_value_from_json(config, "audio-directory");
	char *repeat_str = get_value_from_json(config, "repeat");

	int repeat = atoi(repeat_str);
	free(repeat_str);

	char **songs;
	size_t song_count;

	song_count = read_directory(audio_dir, &songs);
	sprintf(dmesg, "Found %zu files in \"%s\"", song_count, audio_dir);
	i_output(dmesg, "ok");

	for ( int i = 0; i <= song_count; i++ )
	{

		if ( songs[i] == NULL )
		{
			if (repeat)
			{
				i_output("Encountered end of playlist. Repeating...", "ok");
				i = -1; continue;
			}

			else
			{
				i_output("Encountered end of playlist and repeat is disabled.", "ok");
				break;
			}
		}

		sprintf(dmesg, "Playing %s%s", audio_dir, songs[i]);
		i_output(dmesg, "ok");

		char *full_path = malloc( 6 + strlen(audio_dir) + strlen(songs[i]) + 1 );
		strcpy(full_path, audio_dir);
		strcat(full_path, songs[i]);

		FILE *audio = fopen( full_path, "r" );
		free(full_path);
		
		while (1)
		{
		
			read = fread( buffer, 1, sizeof(buffer), audio );
			total = total + read;

			if ( read > 0 )
			{
				
				ret = shout_send (shout, buffer, read);
				if (ret != SHOUTERR_SUCCESS)
					i_output("An error occured while sending buffer.", "error");

			}

			else
				break;

			shout_sync(shout);

		}

		fclose (audio);

		if ( i == song_count )
		{
			i_output("Reached the end of playlist. Repeating...", "ok");
			i = 0;
		}
	
	}

	i_output("Closing stream.", "ok");
	shout_close (shout);

}

