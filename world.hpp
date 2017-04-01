//@	{
//@	 "targets":[{"name":"world.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"world.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_WORLD_HPP
#define GLINDE_WORLD_HPP

#include "harvester/directory.hpp"

namespace Glinde
	{
	class MessageQueue;

	class World
		{
		public:
			explicit World(MessageQueue& msgqueue,const char* filename);
			~World();

		private:
			MessageQueue* r_msgqueue;
			Harvester::Directory m_dirtemp;
		};
	}

#endif
