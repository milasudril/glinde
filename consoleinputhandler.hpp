//@	{"targets":[{"name":"consoleinputhandler.hpp","type":"include"}]}

#ifndef GLINDE_CONSOLEINPUTHANDLER_HPP
#define GLINDE_CONSOLEINPUTHANDLER_HPP

#include "inputhandler.hpp"
#include "console.hpp"

namespace Glinde
	{
	class ConsoleInputHandler:public InputHandler
		{
		public:
			ConsoleInputHandler(Console& con):r_con(&con)
				{}

			void codepoint(uint32_t cp) noexcept
				{r_con->write(cp);}

			void key(int scancode,GLFWmm::WindowBase::Action action
				,unsigned int modifiers)
				{
				switch(scancode)
					{
					case 36:
						if(action!=GLFWmm::WindowBase::Action::RELEASE)
							{
							r_con->write('\n');
							if(modifiers==0)
								{fprintf(stderr,"Invoke command\n");}
							}
						break;
					default:
						fprintf(stderr,"%d\n",scancode);
					}
				}

		private:
			Console* r_con;

		};
	}

#endif
