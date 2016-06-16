#ifdef __WAND__
target[name[logwriter.h] type[include]]
dependency[logwriter.o]
#endif

#ifndef GLINDE_LOGWRITER_H
#define GLINDE_LOGWRITER_H

#include "log.h"
#include <cstdint>

namespace Glinde
	{
	void logWrite(Log::MessageType type,const char* format
		,const std::initializer_list<Variant>& args) noexcept;
	void logWriteDebug(const char* file,uint64_t line,const char* format
		,const std::initializer_list<Variant>& args) noexcept;
	uint32_t logWriterAttach(Log::Writer& writer) noexcept;
	void logWriterDetach(uint32_t index) noexcept;
	void deathtrapHandlerActivate() noexcept;
	Log& logGlobalGet() noexcept;
	}

#endif
