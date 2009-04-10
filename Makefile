# Target, compiler and objects.
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
CFLAGS = `pkg-config --cflags gtkmm-2.4 libglademm-2.4`
# Use for link.
CLIBS  = `pkg-config --libs gtkmm-2.4 libglademm-2.4`
#FLAGS = -DDEBUG -Wall -pedantic -ansi
FLAGS = -DDEBUG 
#xml flag
XMLFLAGS = `pkg-config libxml++-2.6 --cflags --libs`

all:	$(EXEC)

SRCS := MD5.cc checkAndSetConfig.cc User.cc blog.cc sendpacket.cc myerr.cc CodeConverter.cc gbk2utf8.cc Mystar.cc tinyxml.cpp tinystr.cpp tinyxmlerror.cc tinyxmlparser.cpp

OBJS := $(addsuffix .o, $(basename $(SRCS)))

SRCS :=$(addprefix $(SRC)/, $(SRCS))

OBJS := $(addprefix $(OBJ)/, $(OBJS))

${EXEC}: $(OBJS) $(OBJ)/gMystar.o $(OBJ)/main.o
	${CC} ${CLIBS} -lnet -lpcap $^ -g -o $@

$(OBJ)/gMystar.o : $(SRC)/gMystar.cc 
	${CC} $(FLAGS) $(CFLAGS) -c $< -o $@

$(OBJ)/main.o : $(SRC)/main.cc 
	${CC} $(FLAGS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.cc
	${CC} $(FLAGS) -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.cpp
	${CC} $(FLAGS) -c $< -o $@

clean:
	rm $(OBJ)/*.o
run:
	(cd $(BIN);sudo ./gMystar)
runc:
	(cd $(BIN);sudo ./gMystar --nogui)
runt:
	(cd $(BIN);sudo ./gMystar --test)
install:
	#mkdir /etc/gMystar
	cp -v $(BIN)/gMystar /usr/sbin/gMystar
	cp -v $(DATA)/gMystar.glade /etc/gMystar/ui.glade
	cp -v $(DATA)/disconnect.png /etc/gMystar/disconnect.png
	cp -v $(DATA)/connect.png /etc/gMystar/connect.png
debug:
	cd $(BIN);sudo gdb ./gMystar
release:
	tar zcvf ~/Dropbox/Public/gmystar.tar.gz $(gMystar_HOME)/* --exclude $(gMystar_HOME)/doc --exclude $(gMystar_HOME)/dia
