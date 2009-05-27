# This file is generated manually by NetXRay@byhh.
# It isn't very elagent. Don't mock me :)

CC=gcc
Flags=-O2

all:	mystar

mystar: mystar.o md5.o myerr.o blog.o sendpacket.o conn_monitor.o
	$(CC) $(Flags) -o $@  $^ -static -lnet -lpcap -lpthread

conn_monitor.o: conn_monitor.c conn_monitor.h
	$(CC) $(Flags) -o $@ -c $<

md5.o:   md5.c md5.h
	$(CC) $(Flags) -o $@ -c $<

myerr.o: myerr.c myerr.h
	$(CC) $(Flags) -o $@ -c $<

blog.o:  blog.c blog.h  myerr.h
	$(CC) $(Flags) -o $@ -c $<

sendpacket.o: sendpacket.c sendpacket.h md5.h global.h blog.h
	$(CC) $(Flags) -o $@ -c $<

mystar.o: mystar.c mystar.h sendpacket.h myerr.h blog.h global.h
	$(CC) $(Flags) -o $@ -c $<

clean:
	rm -f *.o mystar
rebuild:
	make clean all

