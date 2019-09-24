/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * This is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

shout_t *shout;
shout_metadata_t *stream_metadata;
char *icecast_frmt;

void stream_init ()
{

	// Initialize libshout
	shout_init();
	shout = shout_new();

	// Get the values from configuration file.
	char* icecast_host = get_value_from_json (config, "shout-host");
	char* icecast_pstr = get_value_from_json (config, "shout-port");
	char* icecast_user = get_value_from_json (config, "shout-user");
	char* icecast_pass = get_value_from_json (config, "shout-pass");
	      icecast_frmt = get_value_from_json (config, "shout-frmt");
	char* icecast_mnt  = get_value_from_json (config, "shout-mnt" );

	char* stream_name = get_value_from_json (config, "stream-name");
	char* stream_genre = get_value_from_json (config, "stream-genre");
	char* stream_url = get_value_from_json (config, "stream-url");
	char* stream_desc = get_value_from_json (config, "stream-desc");
	char* stream_irc = get_value_from_json (config, "stream-irc");

	// Initialize debug string variable.
	char dmesg[100];

	// Print out the warning.
	sprintf(
		dmesg, "Initializing libshout using http://%s:xxxx@%s:%s%s",
		icecast_user, icecast_host, icecast_pstr, icecast_mnt
	);

	i_output(dmesg, "warning");

	// Convert char* icecast_pstr to integer icecast_port and free up the space taken by icecast_pstr.
	int icecast_port = atoi(icecast_pstr);
	free(icecast_pstr);

	// Put the required data into libshout.
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

	// Set optional metadata

	stream_metadata = shout_metadata_new();

	if ( strcmp(stream_name, "") != 0 )  shout_set_name( shout, stream_name );
	if ( strcmp(stream_url, "") != 0 )   shout_set_url( shout, stream_url );
	if ( strcmp(stream_genre, "") != 0 ) shout_set_genre( shout, stream_genre );
	if ( strcmp(stream_desc, "") != 0 )  shout_set_description( shout, stream_desc);

	//
	// ^ shout_set_{meta} is deprecated according to the documentation, but
	// I couldn't get below code to send anything for some unknown reason as
	// it simply doesn't return any error and just doesn't set it on Icecast.
	// (it works for setting "song" though.)
	//

	//shout_metadata_add( stream_metadata, SHOUT_META_NAME, stream_name );
	//shout_metadata_add( stream_metadata, SHOUT_META_URL, stream_url );
	//shout_metadata_add( stream_metadata, SHOUT_META_GENRE, stream_genre );
	//shout_metadata_add( stream_metadata, SHOUT_META_DESCRIPTION, stream_desc );

	if ( strcmp(stream_irc, "") != 0 )
		shout_metadata_add( stream_metadata, SHOUT_META_IRC, stream_irc );

	shout_set_metadata( shout, stream_metadata );

}


void run_stream ()
{

	// Initialize debug string variable.
	char dmesg[100];

	// Open Icecast connection.
	int sopen_status = shout_open(shout);
	if ( sopen_status != SHOUTERR_SUCCESS )
	{
		i_output("An error occured while connecting to Icecast. (maybe wrong data?)", "error");
		exit(0);
	}

	// Initialize required variables.
	unsigned char buffer[4096];
	size_t read, total;
	int ret;

	// Get the required information from the configuration file
	char *play_mode = get_value_from_json(config, "song-play-mode");
	char *audio_dir = get_value_from_json(config, "audio-directory");
	char *repeat_str = get_value_from_json(config, "repeat");
	char *str_mysql = get_value_from_json(config, "use-mysql");

	// Convert char*'s to integers and free up the space.
	int repeat = atoi(repeat_str);
	free(repeat_str);

	int use_mysql = atoi(str_mysql);
	free(str_mysql);

	// Read the contents of specified in the configuration directory.
	char **songs;
	size_t song_count = read_directory(audio_dir, &songs);

	if ( !song_count ) {
		sprintf(dmesg, "Specified directory is empty. Halting...");
		i_output(dmesg, "error");
		exit(0);
	}

	char *tmp;

	if ( strcmp(play_mode, "alphabetical") == 0 )
	{

		for (int i = 0; songs[i]; i++)
		{
			for (int j = 0; songs[j]; j++)
			{
				if ( strcmp(songs[i], songs[j]) < 0 )
				{
				
					tmp = songs[i];
					songs[i] = songs[j];
					songs[j] = tmp;

				}
			}
		}

	}

	else if ( strcmp(play_mode, "random") == 0 )
	{
	
		for ( int i = 0; i < song_count; i++ )
		{
		
			srand (time(NULL));
			int j = rand() % song_count;
			tmp = songs[j];
			songs[j] = songs[i];
			songs[i] = tmp;
		
		}
		
	}

	else if ( strcmp(play_mode, "none") == 0 ) { } // do nothing if "none"

	else
	{
	
		sprintf(dmesg, "Unknown play mode \"%s\"", play_mode);
		i_output(dmesg, "error");
		exit(0);

	}

	sprintf(dmesg, "Using play mode \"%s\"", play_mode);
	i_output(dmesg, "ok");

	int access_error_count = 0;
	int extension_error_count = 0;
	int last_extension_error = 0;

	int last_rescan = 0;
	char * requested_song = "";
	char * extension;
	int time_now;

	// Loop through files in the directory.
	for ( int i = 0; i <= song_count; i++ )
	{

		sprintf(dmesg, "Starting loop for ID %d ( %s )", i, songs[i]);
		i_output(dmesg, "warning");

		// End of array
		if ( songs[i] == NULL )
		{

			if (repeat)
			{

				i_output("Encountered end of playlist. Repeating...", "ok");
				// If play mode is random reshuffle the array.
				if ( strcmp(play_mode, "random") )
				{
					int j = rand() % song_count;
					char* tmp = songs[j];
					songs[j] = songs[i];
					songs[i] = tmp;
				}
				access_error_count = 0;
				i = -1; continue;

			}

			else
			{
				i_output("Encountered end of playlist and repeat is disabled.", "ok");
				break;
			}
		}

		// Check if the file has MP3 or OGG extension.
		extension = strrchr(songs[i], '.');

		sprintf(dmesg, "|%s|", extension);
		i_output(dmesg, "warning");

		if ( extension != NULL )
		{
			if ((strcmp(extension, ".mp3") != 0) && (strcmp(extension, ".ogg") != 0))
			{

				sprintf(dmesg, "File %s is not MP3 or OGG.", songs[i]);
				i_output(dmesg, "warning");
				time_now = time(NULL);

				// To prevent infinite loop-like situations, check if there was
				// more than 5 errors in last 15 seconds.
				if ( (last_extension_error + 15) > time_now )
				{
					if (extension_error_count > 5)
					{
						i_output("Too much files in specified directory has wrong extension. Halting.", "error");
						break;
					}
					extension_error_count++;
				}
				
				else
					extension_error_count = 0;
				

				last_extension_error = time_now;
				continue;

			}
		}
		
		char *full_path;
		char *song_name;

		if ( use_mysql )
		{
			i_output("Checking next song...", "warning");
			requested_song = get_next_song();
		}


		if ( strcmp(requested_song, "") != 0 )
		{
		
			i_output("Found new request in MySQL queue.", "warning");

			full_path = malloc( strlen(audio_dir) + strlen(requested_song) + 4 );
			strcpy(full_path, audio_dir);
			strcat(full_path, requested_song);
			strcat(full_path, (strcmp(icecast_frmt, "MP3") == 0) ? ".mp3" : ".ogg" );
			song_name = requested_song;			


			strcpy(requested_song, "");
			i--;

		}
		
		else
		{
			i_output("No new requests in MySQL.", "warning");
			full_path = malloc( strlen(audio_dir) + strlen(songs[i]) + 1 );
			strcpy(full_path, audio_dir);
			strcat(full_path, songs[i]);
			song_name = songs[i];
		}


		if ( access( full_path, R_OK ) == -1 )
		{
			access_error_count++;
			sprintf(dmesg, "File \"%s\" is unreadable. Skipping...", full_path);
			i_output(dmesg, "error");
			free(full_path);

			if ( ((access_error_count / song_count) * 100) > 10 )
			{

				time_now = (int) time(NULL);
				if ( last_rescan > (time_now - 60) )
				{
					i_output("File access errors do not stop after rescan, halting.", "error");
					break;
				}

				i_output("More than 10\% of files in directory cannot be accessed. Rescanning directory...", "error");
				song_count = read_directory(audio_dir, &songs);
				access_error_count = 0;
				last_rescan = time_now;

			}	

			if ( songs[ i + 1 ] == NULL )
			{
				access_error_count = 0;
				i = -1;
			}

			continue;
		}

		else
		{
			sprintf(dmesg, "Streaming \"%s\"...", full_path);
			i_output(dmesg, "ok");
		}

		// Read the file.
		FILE *audio = fopen( full_path, "r" );
		free(full_path);

		shout_metadata_add( stream_metadata, "song", song_name );
		shout_set_metadata( shout, stream_metadata );
		
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

