//@	{"targets":[{"name":"commandprocessor.hpp","type":"include"}]}

#ifndef GLINDE_COMMANDPROCESSOR_HPP
#define GLINDE_COMMANDPROCESSOR_HPP

#include "range.hpp"
#include <cstdint>

namespace Glinde
	{
	class ConsoleInputHandler;

	class CommandProcessor
		{
		public:
			virtual ~CommandProcessor()=default;
			virtual void process(ConsoleInputHandler& coninput
				,Range<const uint32_t> cmdbuff)=0;
		};
	}

#endif