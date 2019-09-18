#!/bin/bash
gcc -Wall -g -o streaming.o streaming.c -lshout
gcc -Wall -g -o queue.o queue.c `mysql_config --cflags --libs`

