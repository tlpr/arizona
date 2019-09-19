#!/bin/bash
clang -Wall -g main.c -L/usr/lib `pkg-config --cflags json-c` -o ../bin/arizona.o `pkg-config --libs json-c` -unistd
cp "cfg.json" "../bin/cfg.json"
cd ../bin/
./arizona.o
cd ../src/

