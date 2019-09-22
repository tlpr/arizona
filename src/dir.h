/*
 *
 * The Las Pegasus Radio
 * https://github.com/tlpr
 * The is free software licensed under the
 * GNU General Public License 3.0-only
 *
 */

#include <dirent.h>

size_t read_directory (char *dir_path, char ***directory_listing)
{

	DIR *directory = NULL;
	struct dirent *ep = NULL;
	char dmesg[100];
	
	size_t count  = 0;

	directory = opendir(dir_path);
	if ( directory == NULL )
	{
		sprintf(dmesg, "Error while reading directory \"%s\"\n", dir_path);
		i_output(dmesg, "error");
		return 0;
	}

	*directory_listing = NULL;
	ep = readdir(directory);
	while ( NULL != ep )
	{
		if ( ep->d_type == DT_REG )
			count++;

		ep = readdir(directory);
	}

	rewinddir(directory);
	*directory_listing = calloc(count, sizeof(char *));

	count = 0;
	ep = readdir(directory);
	while (NULL != ep)
	{

		if ( ep->d_type == DT_REG )
			(*directory_listing)[count++] = strdup(ep->d_name);

		ep = readdir(directory);

	}

	closedir(directory);
	sprintf(dmesg, "Found %zu files in \"%s\"", count, dir_path);
	i_output(dmesg, "ok");

	return count;

}

