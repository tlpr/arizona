
CC = clang
all: compile copy_cfg done

CFLAGS = -g -Wall -L/usr/lib `pkg-config --cflags --libs json-c` `mysql_config --cflags --libs` -unistd -lshout

SRC := main.c
BIN := ../bin/arizona.o

DESTDIR?=/usr/local/bin
CONFDIR?=/etc

.PHONY: install
install:
	cp ../bin/arizona.o $(DESTDIR)/arizona
	cp ../bin/cfg.json $(CONFDIR)/arizona.json

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)/arizona
	rm -f $(CONFDIR)/arizona.json

compile:
	@echo -e "\e[1mCompiling...\e[0m"
	${CC} ${SRC} ${CFLAGS} -o ${BIN}

copy_cfg:
	@echo -e "\e[1mCopying configuration...\e[0m"
	cp "cfg.json" "../bin/cfg.json"

done:
	@echo -e "\e[1;32mArizona can be ran under ../bin/arizona.o\nor installed using \"sudo make install\"\e[0m"

