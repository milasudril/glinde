#ifdef __WAND__
target[name[logwriter.o] type[object]]
#endif

#include "logwriter.h"
#include "logdefault.h"

using namespace Glinde;

static LogDefault s_log;

unsigned int Glinde::logWriterAttach(Log::Writer& writer) noexcept
	{return s_log.writerAttach(writer);}

void Glinde::logWriterDetach(unsigned int index) noexcept
	{s_log.writerDetach(index);}

void Glinde::deathtrapHandlerActivate() noexcept
	{s_log.deathtrapHandlerActivate();}

void Glinde::logWrite(Log::MessageType type,const char* format,...) noexcept
	{
	}
