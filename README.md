# Arizona
### Audio streaming client for Icecast internet radio's

Arizona is a free software streaming client for Icecast servers
written in C programming language using the libshout library.
She supports playing ordered songs using the "queue" table
in a MySQL database and common functions like playlist sorting
and playlist repeating.

#### Installation

The system I am going to use for install is Arch Linux,
but Arizona should compatible with all GNU/Linux distributions,
both 32 and 64 bit. We are going to test Arizona on different
Unix-like operating systems soon.

Dependencies:
- clang compiler
- pkg-config
- json-c
- make
- mariadb-clients / libmysqlclient
- libshout
` sudo pacman -S clang pkgconf json-c make mariadb-clients libshout `

Cloning from this repository:
We are recommending to use Git for that, but you can also download .zip
directly from GitHub website and unpack it in a desired location.
` sudo pacman -S git `
` git clone https://github.com/tlpr/arizona.git `

Change directory to src and compile
` cd arizona/src/ `
` make `

After successful compiling you should own a portable executable
located in "arizona/bin/" directory. In case you want to install
Arizona on your system you can just simply:
` sudo make install `

To revert installation:
` sudo make uninstall `

#### Command-line arguments

` arizona -c /path/to/config.json `
` arizona --use-config /path/to/config.json `

Use configuration file inside a non-standard directory.

` arizona -v `
` arizona --version `

Display version information.

#### Configuring

Configuration file located is either in the
same directory as Arizona executable (called cfg.json)
or in "/etc/arizona.json" in case you have installed it
in you system.

JSON entries labeled with numeric key (eg. "0" / "1" / "2" ...)
are used for comments and changing them do not affect
anything.

` "color": 1 `
color (default: 1) is responsible for telling the i\_output function
in debug.h header file to display colors in "[ OK ] / [ Warn ] / [ Err ]"
prefixes while outputting messages.

` "silence-warnings": 0 `
silence-warnings (default: 0) is used for removing unnecessary stuff
from the screen with prefix "[ Warn ]" used mostly for debugging the
program.

` "db-host": "127.0.0.1" `
db-host (default "127.0.0.1") is the IP address of the MySQL
database server. **In case you don't want to use it, please refer to
"use-mysql" entry below.**

` "db-name": " " `
db-name (default " ") is the name of the database on a MySQL server.

` "db-user": " " `
db-user (default "root") is the username used to log into MySQL server,
it is highly recommended to use user with limited permissions instead of
root.

` "db-pass": " " `
db-pass (default " ") is the password for "db-user"

` "db-port": 3306 `
db-port (default 3306) is the port of the MySQL server.

` "shout-host": "127.0.0.1" `
` "shout-port": 8002 `
shout-host (default "127.0.0.1") is the IP address of the Icecast server.
shout-port (default 8002) is the port of the Icecast server.

` "shout-frmt": "MP3" `
shout-frmt (default "MP3") is the format of the files that
will be streamed into the Icecast server. Currently available formats are
"MP3" and "OGG".

` "shout-mnt": " " `
shout-mnt (default " ") is the mountpoint for the Icecast server. (eg. /default.mp3)

` "shout-user": "source" `
` "shout-pass": " " `
shout-user (default "source") is the username for the Icecast server.
shout-pass (default " ") is the password to the user above.

` "song-play-mode": "random" `
song-play-mode (default "random") is the playlist sorting algorithm. It can be either
"random" or "alphabetical".

` "use-mysql": 1 `
use-mysql (default 1) is the switch of usage of the MySQL server. Switch it to 0
if you don't want Arizona to scan the database in search of song orders.

` "audio-directory": "music/" `
audio-directory (default "music/") is the path for directory containing song files to play.
Use full path in case Arizona is installed into the system.

` "repeat": 1 `
repeat (default 1) is repeating the entire playlist if it reaches the end. Switch it to 0
to turn off Arizona after it's done playing the music inside a folder. 

#### Licenses and credits

Arizona is free software licensed under the [GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)-only.
Las Pegasus Radio strives to promote and contribute to the free software and give developers and its users
freedom by making software usable to itself and, in meantime, making software available for all to edit and use.

Many thanks to creators of the libraries listed below,
without them Arizona would possibly not exist:

- [libshout](https://gitlab.xiph.org/xiph/icecast-libshout/) (GNU Library Public License, by Xiph.org Foundation)
- [libmysqlclient](https://dev.mysql.com/downloads/connector/c/) (GNU GPL, by Oracle Corp.)
- [json-c](https://github.com/json-c/json-c)

