#ifdef __WAND__
target[name[gameloop.h] type[include]]
dependency[gameloop.o]
#endif

#ifndef GLINDA_GAMELOOP_H
#define GLINDA_GAMELOOP_H

namespace Glinda
	{
	class Window;
	class Timer;
	class World;
	class UIManager;

	void gameLoopRun(UIManager& ui,Window& window
		,const Timer& world_clock,World& world);
	}

#endif
