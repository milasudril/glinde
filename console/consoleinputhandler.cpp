/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "targets":[{"name":"consoleinputhandler.o","type":"object"}]
//@	}

#include "consoleinputhandler.hpp"
#include "../commandprocessor.hpp"
#include "../io/utf8.hpp"

using namespace Glinde;

static std::pair<intptr_t,int> line_length(const uint32_t* begin,const uint32_t* end)
	{
	auto ptr=end;
	while(ptr!=begin)
		{	
		--ptr;
		if(*ptr=='\n')
			{return {end-(ptr+1),0};}
		}
	return {end-ptr,1};
	}

void ConsoleInputHandler::key(int scancode,GLFWmm::WindowBase::Action action
	,unsigned int modifiers)
	{
	switch(m_status)
		{
		case Status::READY:
			switch(scancode)
				{
				case 22:
					if(m_input_buffer.length()!=0 && action!=GLFWmm::WindowBase::Action::RELEASE)
						{
						auto ch=*(m_input_buffer.end() - 1);
						m_input_buffer.truncate();
						if(ch=='\n')
							{
							auto l=line_length(m_input_buffer.begin(),m_input_buffer.end());
							r_con->eraseLinefeed(l.first + l.second);
							}
						else
							{r_con->erase();}
						}
					break;

				case 36:
					if(action!=GLFWmm::WindowBase::Action::RELEASE)
						{
						r_con->write('\n');
						if(modifiers==0)
							{
							m_status=Status::WAITING;
							r_cmdproc->process(*this,utf8_encode(m_input_buffer).begin());
							}
						else
							{m_input_buffer.append('\n');}
						}
					break;

				default:
					break;
				//	fprintf(stderr,"%d\n",scancode);
				}
			break;
		case Status::READY_AFTER_KEY:
			if(action!=GLFWmm::WindowBase::Action::RELEASE)
				{status(Status::READY);}
			break;
		default:
			break;
		}
	}
