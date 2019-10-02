/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * This is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

// Variable used to compare it to new bumper
// to prevent repeating it for the second time.
char * last_bumper = NULL;

// Returns path to bumper or NULL, depending if it's either time to play
// scheduled bumper or random one.
char * get_bumper (void)
{

	char dmesg[150];
	
	// config declaration: main.c#~78
	char * bumpers_enabled_char = get_value_from_json(config, "play-bumpers");
	int bumpers_enabled = atoi(bumpers_enabled_char);
	
	if ( !bumpers_enabled )
		return NULL;
	
	char * bumpers_path = get_value_from_json(config, "bumpers-path");
	
	sprintf(dmesg, "Bumpers enabled at \"%s\". Scanning...", bumpers_path);
	i_output(dmesg, "ok");
	
	// Check if there are any scheduled 
	char **directory_listing;
	size_t directory_size = read_directory(bumpers_path, &directory_listing, 1);
	
	if ( directory_size == 0 )
		return NULL;
	
	// Obtain current time and convert it to localtime
	time_t time_now = time(NULL);
	ctime(&time_now);
	struct tm * local_now = localtime(&time_now);
	
	int hours_since_midnight = local_now->tm_hour;
	int minutes_since_midnight = local_now->tm_min;
	
	char bumper_directory_name[6];
	sprintf(bumper_directory_name, "%d_%d", hours_since_midnight, minutes_since_midnight);
	
	char bumper_to_be_played[ strlen(bumpers_path) + 6 + 96 ];
	int random_directory_exist = 0;
	i_output("Starting loop", "warning");
	
	// Loop through files
	for ( int i = 0; i < directory_size; i++ )
	{
		
		if ( strcmp(directory_listing[i], "random") == 0 )
		{
			random_directory_exist = 1;
			continue;
		}
		
		if ( strcmp(bumper_directory_name, directory_listing[i]) == 0 )
		{
		
			sprintf( bumper_to_be_played, "%s%s/bumper.mp3", bumpers_path, bumper_directory_name );
			break;
		
		}
	
	}

	if ( strlen(bumper_to_be_played) > 1 )
	{
		
		if ( access(bumper_to_be_played, R_OK) == -1 )
			return NULL;

		char * bumper_to_be_played_p = bumper_to_be_played;
		if ( last_bumper == bumper_to_be_played_p )
			return NULL;
		else
			last_bumper = bumper_to_be_played_p;
		
		return bumper_to_be_played_p;
		
	}

	// If not found, get a random bumper with a chance of "bumper-chance" (in cfg json).
	// If directory "random" doesn't exist, return NULL.
	if ( !random_directory_exist )
		return NULL;
	
	char * bumper_chance_char = get_value_from_json(config, "bumper-chance");
	int bumper_chance = atoi(bumper_chance_char);
	free(bumper_chance_char);
	
	int random_number = rand() % 101;
	if ( random_number > bumper_chance )
		return NULL;
	
	char random_directory_path[ strlen(bumpers_path) + 6 ];
	sprintf(random_directory_path, "%srandom", bumpers_path);
	
	free(directory_listing);
	directory_size = read_directory(random_directory_path, &directory_listing, 0);
	
	if ( directory_size == 0 )
		return NULL;
	else if ( directory_size == 1 )
		return directory_listing[0];
	
	random_number = (rand() % (unsigned int)directory_size);
	char * randomized_bumper = directory_listing[ random_number ];
	
	char random_full_path[ strlen(random_directory_path) + 96 ];
	sprintf(random_full_path, "%s/%s", random_directory_path, randomized_bumper);
	
	char * random_full_path_p = malloc( strlen(random_directory_path) + 96 );
	strcpy(random_full_path_p, random_full_path);
	
	if ( last_bumper == random_full_path_p )
		return NULL;
	else
		last_bumper = random_full_path_p;
	
	return random_full_path_p;
	
}
