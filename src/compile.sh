#!/bin/bash
clang -Wall -g main.c -L/usr/lib `pkg-config --cflags json-c` -o arizona.o `pkg-config --libs json-c` -unistd

