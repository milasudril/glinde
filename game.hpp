//@	{
//@	 "targets":[{"name":"game.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"game.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_GAME_HPP
#define GLINDE_GAME_HPP

#include "harvester/directory.hpp"

namespace Glinde
	{
	class MessageQueue;

	class Game
		{
		public:
			explicit Game(MessageQueue& msgqueue,const char* filename);
			~Game();

		private:
			MessageQueue* r_msgqueue;
			Harvester::Directory m_dirtemp;
		};
	}

#endif
