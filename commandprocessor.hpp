//@	{"targets":[{"name":"commandprocessor.hpp","type":"include"}]}

#ifndef GLINDE_COMMANDPROCESSOR_HPP
#define GLINDE_COMMANDPROCESSOR_HPP

#include "arraydynamic.hpp"

namespace Glinde
	{
	class ConsoleInputHandler;

	class CommandProcessor
		{
		public:
			virtual ~CommandProcessor()=default;
			virtual void process(ConsoleInputHandler& coninput
				,const ArrayDynamic<uint32_t>& cmdbuff)=0;
		};
	}

#endif