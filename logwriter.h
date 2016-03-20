#ifdef __WAND__
target[name[logwriter.h] type[include]]
dependency[logwriter.o]
#endif

#ifndef GLINDA_LOGWRITER_H
#define GLINDA_LOGWRITER_H

namespace Glinda
	{
#ifdef NDEBUG
	enum class LogMessageType:unsigned int{INFORMATION,WARNING,ERROR};
#else
	enum class LogMessageType:unsigned int{INFORMATION,WARNING,ERROR,DEBUG};
#endif
	void logWrite(LogMessageType type,const char* format,...);

	void deathtrapHandlerActivate();
	};

#endif
