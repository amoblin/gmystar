###############################
# Makefile
#
# Copyright (C) 2009  csip(amoblin@gmail.com)
#
# ChangeLog:
#
# Description:
# Warning: this file should be in UTF-8.
#
# Target, compiler and objects.
###############################
gMystar_HOME   = .
OBJ    = $(gMystar_HOME)/obj
DOC    = $(gMystar_HOME)/doc
DIA    = $(gMystar_HOME)/dia
SRC    = $(gMystar_HOME)/src
BIN    = $(gMystar_HOME)/bin
DATA   = $(gMystar_HOME)/data
CC     = g++
EXEC   = $(BIN)/gMystar

# Use for compile.
CFLAGS = `pkg-config --cflags dbus-glib-1 libglademm-2.4 gtkmm-2.4 libnotifymm-1.0 libnm_glib`
# Use for link.
CLIBS  = `pkg-config --libs dbus-glib-1 gtkmm-2.4 libglademm-2.4 libnotifymm-1.0 libnm_glib`
#FLAGS = -DDEBUG -Wall -pedantic -ansi
FLAGS = -DDEBUG 
#xml flag
XMLFLAGS = `pkg-config libxml++-2.6 --cflags --libs`

all:	$(EXEC)

SRCS := MD5.cc checkAndSetConfig.cc User.cc blog.cc sendpacket.cc myerr.cc gbk2utf8.cc Mystar.cc tinyxml/tinyxml.cpp tinyxml/tinystr.cpp tinyxml/tinyxmlerror.cc tinyxml/tinyxmlparser.cpp

OBJS := $(addsuffix .o, $(basename $(SRCS)))

SRCS :=$(addprefix $(SRC)/, $(SRCS))

OBJS := $(addprefix $(OBJ)/, $(OBJS))

$(EXEC): $(OBJS) $(OBJ)/gMystar.o $(OBJ)/main.o
	$(CC) $(CLIBS) -lnet -lpcap $^ -g -o $@

$(OBJ)/gMystar.o : $(SRC)/gMystar.cc 
	$(CC) --static $(FLAGS) $(CFLAGS) -lnotify -c $< -o $@

$(OBJ)/main.o : $(SRC)/main.cc 
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@

$(OBJ)/Mystar.o : $(SRC)/Mystar.cc 
	$(CC) $(FLAGS) `pkg-config --cflags dbus-glib-1 libnm_glib` -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.cc
	$(CC) $(FLAGS) `pkg-config --cflags dbus-glib-1 libnm_glib` -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)/*.o
run:
	(xterm -e "cd $(BIN);sudo ./gMystar")
runc:
	(xterm -e "cd $(BIN);sudo ./gMystar --nogui")
runt:
	(cd $(BIN);sudo ./gMystar --test)
install:
	#mkdir /usr/share/gMystar
	cp -vr $(BIN) /usr/share/gMystar
	cp -vr $(DATA) /usr/share/gMystar
	#ln -s /usr/share/gMystar/bin/gMysar /usr/sbin/gMystar
	cp -v /usr/share/gMystar/bin/gMystar /usr/sbin/gMystar
debug:
	cd $(BIN);sudo gdb ./gMystar
gMystar_HOME_r   = $(gMystar_HOME)/../gmystar
release:
	tar jcvf ~/Dropbox/Public/gmystar.tar.bz2 $(gMystar_HOME_r)/* --exclude $(gMystar_HOME_r)/doc --exclude $(gMystar_HOME_r)/dia --exclude $(gMystar_HOME_r)/ref
	$(gMystar_HOME)/makeall
	$(gMystar_HOME)/back_windows
