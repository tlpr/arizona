
CC = clang
all: compile copy_cfg done

CFLAGS = -g -Wall -L/usr/lib `pkg-config --cflags --libs json-c` `mysql_config --cflags --libs` -unistd -lshout

SRC := main.c
BIN := ../bin/arizona.o

compile:
	@echo -e "\e[1mCompiling...\e[0m"
	${CC} ${SRC} ${CFLAGS} -o ${BIN}

copy_cfg:
	@echo -e "\e[1mCopying configuration...\e[0m"
	cp "cfg.json" "../bin/cfg.json"

done:
	@echo -e "\e[1;32mArizona can be ran under ../bin/arizona.o\e[0m"

