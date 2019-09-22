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


struct json_object *read_configuration (char *config_file_path)
{

	// Find the configuration file.
	if ( strcmp(config_file_path, "") == 0 )
	{

		if ( access("./cfg.json", R_OK) != -1 )
			config_file_path = "./cfg.json";
	
		else if ( access("/etc/arizona.json", R_OK) != -1 )
			config_file_path = "/etc/arizona.json";

		else
		{
			printf("[Err]  Error accessing configuration file. Halting.\n");
			exit(0);
		}

	}

	printf("Starting with \"%s\"...\n", config_file_path);

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
	return (char*)json_object_get_string(tmp);

}

