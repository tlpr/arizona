
CC = clang
all: compile copy_cfg done

CFLAGS = -g -Wall -L/usr/lib `pkg-config --cflags --libs json-c` `mysql_config --cflags --libs` -unistd -lshout

SRC := main.c
BIN := ../bin/arizona.o

.PHONY: install
install arizona:
	mv "../bin/arizona.o" "/usr/bin/arizona"
	mv "../bin/cfg.json" "/etc/arizona.json"

.PHONY: uninstall
uninstall:
	rm -f "/usr/bin/arizona"
	rm -f "/etc/arizona.json"

compile:
	@echo -e "\e[1mCompiling...\e[0m"
	${CC} ${SRC} ${CFLAGS} -o ${BIN}

copy_cfg:
	@echo -e "\e[1mCopying configuration...\e[0m"
	cp "cfg.json" "../bin/cfg.json"

done:
	@echo -e "\e[1;32mArizona can be ran under ../bin/arizona.o\nor installed using \"sudo make install\"\e[0m"

