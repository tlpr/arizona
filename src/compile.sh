#!/bin/bash
clang -Wall -g main.c -L/usr/lib `pkg-config --cflags json-c` `mysql_config --cflags --libs` -o ../bin/arizona.o `pkg-config --libs json-c` -unistd -lshout -target x86_64-pc-linux-gnu
#clang -Wall -g main.c -L/usr/lib `pkg-config --cflags json-c` `mysql_config --cflags --libs` -o ../bin/arizona.o `pkg-config --libs json-c` -unistd -lshout -target i386-pc-linux-gnu
cp "cfg.json" "../bin/cfg.json"
cd ../bin/ && echo; ./arizona.o && echo; cd ../src/

