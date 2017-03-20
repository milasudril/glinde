//@	{"targets":[{"name":"cmdprocmain.hpp","type":"include"}]}

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
				,const ArrayDynamic<uint32_t>& cmdbuff)
				{
				fprintf(stderr,"Invoke command\n");
				coninput.status(Status::READY);
				}
		private:
			EngineBase* r_engine;
		};
	}

#endif