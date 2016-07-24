//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"logwriter.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"logwriter.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_LOGWRITER_H
#define GLINDE_LOGWRITER_H

#include "log.h"
#include <cstdint>

namespace Glinde
	{
	class MessageQueue;

	void logWrite(Log::MessageType type,const char* format
		,const std::initializer_list<Variant>& args) noexcept;

	unsigned int logWriterAttach(Log::Writer& writer) noexcept;

	void logWriterDetach(unsigned int index) noexcept;

	unsigned int logQueueAttach(MessageQueue& queue) noexcept;

	void logQueueDetach(unsigned int index) noexcept;

	void deathtrapHandlerActivate() noexcept;

	Log& logGlobalGet() noexcept;


	}

#endif
