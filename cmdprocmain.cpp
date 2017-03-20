//@	{
//@	 "targets":[{"name":"cmdprocmain.o","type":"object"}]
//@	}

#include "cmdprocmain.hpp"

using namespace Glinde;

static Range<const uint32_t> tokenGet(Range<const uint32_t> r,ArrayDynamic<uint32_t>& buffer)
	{
	auto ptr=r.begin();
	auto ptr_end=r.end();
	buffer.clear();
	auto state=0;
	while(ptr!=ptr_end)
		{
		auto ch_in=*ptr;
		switch(state)
			{
			case 0:
				switch(ch_in)
					{
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
					case '\\':
						state=2;
						break;
					case ' ':
						return Range<const uint32_t>(ptr,ptr_end);
					default:
						buffer.append(ch_in);
					}
				break;

			case 2:
				buffer.append(ch_in);
				state=1;
				break;

			default:
				break;
			}
		++ptr;
		}
	return Range<const uint32_t>(ptr,ptr_end);
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

static bool equals(Range<const uint32_t> a,const char32_t* str)
	{
	auto a_begin=a.begin();
	auto a_end=a.end();
	while(*str!='\0' && a_begin!=a_end)
		{
		if(*str!=*a_begin)
			{return 0;}
		++a_begin;
		++str;
		}
	return *str=='\0' && a_begin==a_end;
	}

void CmdProcMain::process(ConsoleInputHandler& coninput
	,Range<const uint32_t> cmdbuff)
	{
	ArrayDynamic<uint32_t> tok;
	ArrayDynamic<decltype(tok)> cmd;
	while(cmdbuff.begin()!=cmdbuff.end())
		{
		cmdbuff=tokenGet(cmdbuff,tok);
		if(tok.length()!=0)
			{cmd.append(tok);}
		}

	if(equals(cmd[0],U"exit"))
		{r_engine->stop();}
	else
		{coninput.status(Status::READY);}
	}
