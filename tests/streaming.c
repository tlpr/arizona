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
	shout_set_port(shout, 8002);
	
	shout_set_mount(shout, "/main.mp3");
	shout_set_user(shout, "mainn");
	shout_set_password(shout, "");

	shout_set_format(shout, SHOUT_FORMAT_MP3);


	if ( shout_open(shout) == SHOUTERR_SUCCESS )
	{
	
		total = 0;
		
		char *audio_files[] = {
			"/home/finn/Music/audio.mp3",
			"/home/finn/Music/citylove.mp3"
		};

		int playlist_size = sizeof(audio_files);


		for (int i = 0; i <= playlist_size; i++)
		{
			printf(audio_files[i]);
		
			// load file
			FILE *audio = fopen( audio_files[i], "r" );

			while (1)
			{
			
				read = fread (buffer, 1, sizeof(buffer), audio);
				total = total + read;

				if ( read > 0 )
				{
				
					ret = shout_send (shout, buffer, read);
					if ( ret != SHOUTERR_SUCCESS )
						printf("Error: %s\n", shout_get_error(shout));

					
				
				}

				else
					break;

				shout_sync(shout);

			}


		}

		shout_close(shout);
		
	}

	return 0;

}


