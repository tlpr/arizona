#!/bin/bash
clang -Wall -g main.c -L/usr/lib `pkg-config --cflags json-c` `mysql_config --cflags --libs` -o ../bin/arizona.o `pkg-config --libs json-c` -unistd -lshout
cp "cfg.json" "../bin/cfg.json"
cd ../bin/
./arizona.o
cd ../src/

