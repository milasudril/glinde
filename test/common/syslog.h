//@	{
//@	 "targets":[{"name":"syslog.h","type":"include"}]
//@	,"dependencies_extra":[{"ref":"syslog.o"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	}

#ifndef SYSLOG_H
#define SYSLOG_H

namespace Glinde
	{class Log;}

extern Glinde::Log* g_syslog;

#endif
