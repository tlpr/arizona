/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * The is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

#include <json.h>
#include <stdio.h>
#include <unistd.h>


struct json_object *read_configuration ()
{

	char *config_file_path = "cfg.json";

	// Check if the file exists.
	if ( access( config_file_path, F_OK ) == -1 )
	{
		printf("Couldn't find the configuration file. Exiting...\n");
		exit(0);
	}

	// Read the file and parse JSON.
	FILE *config_file_obj = fopen(config_file_path, "r");
	if ( !config_file_obj )
	{
		printf("Could not read the configuration file. Exiting...\n");
		exit(0);
	}
	
	char *buffer = 0;
	long length;

	fseek (config_file_obj, 0, SEEK_END);
	length = ftell (config_file_obj);

	fseek (config_file_obj, 0, SEEK_SET);
	buffer = malloc (length);

	if (buffer)
		fread (buffer, 1, length, config_file_obj);

	fclose (config_file_obj);

	struct json_object *config_file_json;
	config_file_json = json_tokener_parse(buffer);

	return config_file_json;

}


char *get_value_from_json ( struct json_object *configuration, char* key )
{

	struct json_object *tmp;
	json_object_object_get_ex(configuration, key, &tmp);
	return (char*)json_object_to_json_string(tmp);

}

