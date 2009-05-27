SOURCES =md5.cpp \
kstar.cpp \
trayicon.cpp \
myinifile.cpp \
main.cpp \
mystar.cpp

TARGET =./kstar
TEMPLATE = app
LIBS += -lpcap  \
-lnet 

FORMS += kstarbase.ui


HEADERS += kstar.h \
 trayicon.h\
 md5.h\
 mystar.h\
 myinifile.h 


