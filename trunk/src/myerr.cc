/*****************************************************
 *  myerr.cc
 *
 *  Copyright (C) 2009  csip(amoblin@gmail.com)
 *
 *  ChangeLog:
 *
 *  Description:
 *	Warning: this file should be in UTF-8.
 *  
 ******************************************************/

/* This file comes from APUE, slightly edited by NetXRay@byhh */

#include "Mystar.h"
/* Nonfatal error related to a system call.
 * Print a message and return. */
void Mystar::err_ret(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	return;
}
/* Fatal error related to a system call.
 * Print a message and terminate. */
void Mystar::err_sys(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}
/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */
void Mystar::err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	exit(1);		/* shouldn't get here */
}
/* Nonfatal error unrelated to a system call.
 * Print a message and return. */
void Mystar::err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	return;
}
/* Fatal error unrelated to a system call.
 * Print a message and terminate. */
void Mystar::err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	exit(1);
}
/* Print a message and return to caller.
 * Caller specifies "errnoflag". */
void Mystar::err_doit(int errnoflag, const char *fmt, va_list ap)
{
	int		errno_save;
	char	buf[4096];

	errno_save = errno;		/* value caller might want printed */
	vsprintf(buf, fmt, ap);
	if (errnoflag)
		sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
	return;
}
