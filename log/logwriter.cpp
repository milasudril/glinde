//@	{"targets":[{"name":"logwriter.o","type":"object"}]}

#include "logwriter.hpp"
#include "logdefault.hpp"
#include "../stringformat.hpp"

using namespace Glinde;

static LogDefault s_log;

Log& Glinde::logGlobalGet() noexcept
	{return s_log;}

unsigned int Glinde::logWriterAttach(Log::Writer& writer) noexcept
	{return s_log.writerAttach(writer);}

void Glinde::logWriterDetach(unsigned int index) noexcept
	{s_log.writerDetach(index);}

unsigned int Glinde::logQueueAttach(MessageQueue& q) noexcept
	{
	s_log.queueAttach(q);
	return 0;
	}

void Glinde::logQueueDetach(unsigned int index) noexcept
	{s_log.queueDetach(index);}

void Glinde::deathtrapHandlerActivate() noexcept
	{s_log.deathtrapHandlerActivate();}

void Glinde::logWrite(Log::MessageType type
	,const char* format,const std::initializer_list<Variant>& args)
	{s_log.write(type,format,args);}

void Glinde::logProgressInit()
	{s_log.progressInit();}

void Glinde::logProgressEnd()
	{s_log.progressEnd();}

void Glinde::logProgress(double x,const char* message)
	{s_log.progress(x,message);}
