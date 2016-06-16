#ifdef __WAND__
target[name[logwriter.o] type[object]]
#endif

#include "logwriter.h"
#include "logdefault.h"
#include "stringformat.h"

using namespace Glinde;

static LogDefault s_log;

Log& Glinde::logGlobalGet() noexcept
	{
	return s_log;
	}

unsigned int Glinde::logWriterAttach(Log::Writer& writer) noexcept
	{return s_log.writerAttach(writer);}

void Glinde::logWriterDetach(unsigned int index) noexcept
	{s_log.writerDetach(index);}

void Glinde::deathtrapHandlerActivate() noexcept
	{s_log.deathtrapHandlerActivate();}

void Glinde::logWrite(Log::MessageType type
	,const char* format,const std::initializer_list<Variant>& args) noexcept
	{
	s_log.write(type,format,args);
	}

void Glinde::logWriteDebug(const char* file,uint64_t line
	,const char* format_string
	,const std::initializer_list<Variant>& args) noexcept
	{
	char buffer_a[4096];
	format(Range<char>(buffer_a,sizeof(buffer_a)/sizeof(buffer_a[0]))
		,format_string,args);

	char buffer_b[4096];
	format(Range<char>(buffer_b,sizeof(buffer_b)/sizeof(buffer_b[0]))
		,"#0;:#1;: #2;",{file,line,buffer_a});
	s_log.write(Log::MessageType::DEBUG,"#0;",{buffer_b});
	}

