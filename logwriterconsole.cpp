#ifdef __WAND__
target[name[logwriterconsole.o] type[object]]
#endif

#include "logwriterconsole.h"
#include "engine.h"
#include <cstdint>
#include <cstdio>

using namespace Glinde;

void LogWriterConsole::write(LogMessageType type,const char* message)
	{
	const char* infostring="";
	uint8_t color_mask=0x07;
	switch(type)
		{
		case LogMessageType::INFORMATION:
			infostring="(i) ";
			color_mask=0x0a;
			break;

		case LogMessageType::WARNING:
			infostring="(!) ";
			color_mask=0x0e;
			break;

		case LogMessageType::ERROR:
			infostring="(x) ";
			color_mask=0x0c;
			break;

		case LogMessageType::DEBUG:
			infostring="(DEBUG) ";
			color_mask=0x0d;
			break;

		case LogMessageType::LINE_QUOTE:
			infostring="  ";
			color_mask=0x07;
			break;
		}
	r_engine.consoleColorMaskSet(color_mask);
	r_engine.consoleWrite(infostring);
	r_engine.consoleColorMaskSet(0x07);
	r_engine.consoleWrite(message);
	r_engine.consoleWrite('\n');
	}
