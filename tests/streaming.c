#include <stdio.h>

#include "shout/shout.h"


int main()
{

	unsigned char buffer[4096];
	size_t read, total;
	int ret;

	shout_t *shout;
	
	shout_init();
	shout = shout_new();

	shout_set_host(shout, "127.0.0.1");
	shout_set_protocol(shout, SHOUT_PROTOCOL_HTTP);
	shout_set_port(shout, 8001);
	
	shout_set_mount(shout, "/main.mp3");
	shout_set_user(shout, "mainn");
	shout_set_password(shout, "");

	shout_set_format(shout, SHOUT_FORMAT_MP3);


	if ( shout_open(shout) == SHOUTERR_SUCCESS )
	{
	
		total = 0;
		FILE *audio_file = fopen("/home/finn/Music/audio.mp3", "r");

		while (1)
		{

			read = fread(buffer, 1, sizeof(buffer), audio_file);
			total = total + read;

			if (read > 0)
			{

				ret = shout_send(shout, buffer, read);
				if (ret != SHOUTERR_SUCCESS)
				{
					printf("Error: %s\n", shout_get_error(shout));
					break;
				}

			}

			else
				break;

			shout_sync(shout);

		}

	}

	shout_close(shout);
	return 0;

}


