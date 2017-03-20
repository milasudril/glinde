//@	{
//@	 "targets":[{"name":"cmdprocmain.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"cmdprocmain.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CMDPROCMAIN_HPP
#define GLINDE_CMDPROCMAIN_HPP

#include "commandprocessor.hpp"
#include "enginebase.hpp"
#include "console/consoleinputhandler.hpp"

namespace Glinde
	{
	class CmdProcMain:public CommandProcessor
		{
		public:
			explicit CmdProcMain(EngineBase& eng):r_engine(&eng)
				{}

			void process(ConsoleInputHandler& coninput
				,Range<const uint32_t> cmdbuff);

		private:
			EngineBase* r_engine;
		};
	}

#endif