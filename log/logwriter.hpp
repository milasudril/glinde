//@	{
//@	 "dependencies_extra":[{"ref":"logwriter.o","rel":"implementation"}]
//@ ,"targets":[{"name":"logwriter.hpp","type":"include"}]
//@	}

#ifndef GLINDE_LOGWRITER_HPP
#define GLINDE_LOGWRITER_HPP

#include "log.hpp"
#include <cstdint>

namespace Glinde
	{
	class MessageQueue;

	void logWrite(Log::MessageType type,const char* format
		,const std::initializer_list<Variant>& args);

	void logProgress(double x,const char* message);

	unsigned int logWriterAttach(Log::Writer& writer) noexcept;

	void logWriterDetach(unsigned int index) noexcept;

	unsigned int logQueueAttach(MessageQueue& queue) noexcept;

	void logQueueDetach(unsigned int index) noexcept;

	void deathtrapHandlerActivate() noexcept;

	Log& logGlobalGet() noexcept;
	}

#endif
