//@	{
//@	 "targets":[{"name":"cmdprocmain.o","type":"object"}]
//@	}

#include "display.hpp"
#include "cmdprocmain.hpp"
#include "errormessage.hpp"
#include "variant.hpp"
#include "string.hpp"

using namespace Glinde;

static const char* tokenGet(const char* ptr,String& buffer)
	{
	buffer.clear();
	auto state=0;
	while(1)
		{
		auto ch_in=*ptr;
		switch(state)
			{
			case 0:
				switch(ch_in)
					{
					case '\0':
						return nullptr;
					case '\\':
						state=2;
						break;
					default:
						if(ch_in>' ')
							{
							buffer.append(ch_in);
							state=1;
							}
					}
				break;

			case 1:
				switch(ch_in)
					{
					case '\0':
						return nullptr;
					case '\\':
						state=2;
						break;
					case ' ':
						return ptr;
					default:
						buffer.append(ch_in);
					}
				break;

			case 2:
				switch(ch_in)
					{
					case '\0':
						return nullptr;
					default:
						buffer.append(ch_in);
						state=1;
					}
				break;

			default:
				break;
			}
		++ptr;
		}
	return ptr;
	}

#if 0
static void print(Range<const uint32_t> r)
	{
	auto ptr=r.begin();
	auto ptr_end=r.end();
	putchar('[');
	while(ptr!=ptr_end)
		{
		putchar(*ptr);
		++ptr;
		}
	putchar(']');
	putchar('\n');
	}
#endif

void CmdProcMain::process(ConsoleInputHandler& coninput,const char* cmdbuff)
	{
	String tok;
	ArrayDynamic<decltype(tok)> cmd;
	do
		{
		cmdbuff=tokenGet(cmdbuff,tok);
		if(tok.length()!=0)
			{cmd.append(tok);}
		}
	while(cmdbuff!=nullptr);

	if(cmd.length()==0)
		{coninput.status(Status::READY);}
	else
		{
		if(cmd[0]=="exit")
			{r_engine->stop();}
		else
		if(cmd[0]=="consoletest")
			{r_engine->consoletest();}
		else
		if(cmd[0]=="glinfo")
			{
			auto info=r_display->glinfo();
			auto& con=coninput.consoleBuffer();
			con.writeUTF8(info.renderer)
				.writeUTF8(", ")
				.writeUTF8(info.version)
				.writeUTF8("\n");
			coninput.status(Status::READY);
			}
		else
		if(cmd[0]=="load")
			{
			if(cmd.length()==1)
				{throw ErrorMessage("What game do you want to load?",{});}
			if(cmd.length()>2)
				{throw ErrorMessage("load: Too many arguments. If the argument contains any whitespace, use \\ before any whitespace.",{});}
			r_engine->gameLoad(cmd[1].begin());
			}
		else
			{throw ErrorMessage("Bad command",{});}
		}
	}
