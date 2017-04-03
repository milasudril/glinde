//@	{"targets":[{"name":"logwriterconsole.o","type":"object"}]}

#include "logwriterconsole.hpp"
#include "console.hpp"
#include <cstdint>
#include <cstdio>

using namespace Glinde;

void LogWriterConsole::write(Log::MessageType type,const char* message) noexcept
	{
	const char* infostring="";
	uint8_t color_mask=0x07;
	switch(type)
		{
		case Log::MessageType::INFORMATION:
			infostring="(i) ";
			color_mask=0x0a;
			break;

		case Log::MessageType::WARNING:
			infostring="(!) ";
			color_mask=0x0e;
			break;

		case Log::MessageType::ERROR:
			infostring="(x) ";
			color_mask=0x0c;
			break;

		case Log::MessageType::DEBUG:
			infostring="(DEBUG) ";
			color_mask=0x0d;
			break;

		case Log::MessageType::LINE_QUOTE:
			infostring="  ";
			color_mask=0x07;
			break;
		}
	r_con->colorMask(color_mask);
	r_con->writeUTF8(infostring);
	r_con->colorMask(0x07);
	r_con->writeUTF8(message);
	r_con->write('\n');
	}

void LogWriterConsole::progress(const char* message,double x) noexcept
	{
	int val=(r_con->colsCount() - 1)*x;
	while(val!=prog_state)
		{
		r_con->write(U'█');
		++prog_state;
		}
	}


void LogWriterConsole::progressInit() noexcept
	{
	r_con->write('\n').write(' ');
	auto n=r_con->colsCount() - 2;
	while(n)
		{
		r_con->write(U'▒');
		--n;
		}
	r_con->write('\r').write(' ');
	prog_state=0;
	}

void LogWriterConsole::progressEnd() noexcept
	{
	r_con->write('\n').write('\n');
	}
