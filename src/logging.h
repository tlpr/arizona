/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * This is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

int log_size_warning_sent = 0;

void append_log (char * message)
{

	int log_file_enabled = atoi( get_value_from_json(config, "enable-logging") );
	if (!log_file_enabled) return;
	
	char * log_file_path = get_value_from_json(config, "log-file");
	int log_file_size = atoi( get_value_from_json(config, "log-file-size-limit") );
	
	if ( (access(log_file_path, W_OK) == -1) || (access(log_file_path, R_OK) == -1) )
	{
		FILE * created_file = fopen(log_file_path, "ab+");
		if ( created_file == NULL )
		{
			printf("%s \"%s\" %s\n", "Log file", log_file_path, "is inaccessible, please check configuration and permissions. Skipping...");
			return;
		}
		
		fclose(created_file);
	}
	
	if ( (log_file_size < 1000) && (!log_size_warning_sent) )
	{
		printf("%s\n", "WARNING: Log file size limit is extremely low, please consider making it higher.");
		log_size_warning_sent = 1;
	}
	
	FILE * log_file = fopen(log_file_path, "a");
	fseek(log_file, 0, SEEK_END);
	signed int current_size = ftell(log_file);
	fclose(log_file);
	
	if ( current_size > log_file_size )
		remove(log_file_path);
	
	log_file = fopen(log_file_path, "a");
	
	time_t time_now = time(NULL);
	struct tm ltime = *localtime(&time_now);
	fprintf(
		log_file, "[%d.%d.%d %d:%d:%d] %s\n",
		ltime.tm_mday, ltime.tm_mon + 1, ltime.tm_year + 1900,
		ltime.tm_hour, ltime.tm_min, ltime.tm_sec, message
	);
	
	fclose(log_file);
	
}
