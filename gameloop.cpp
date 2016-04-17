#ifdef __WAND__
target[name[gameloop.o] type[object]]
#endif

#include "gameloop.h"
#include "world.h"
#include "window.h"
#include "timer.h"
#include "uimanager.h"

#include <ctime>

using namespace Glinda;

void Glinda::gameLoopRun(UIManager& ui,Window& window,const Timer& world_clock
	,World& world)
	{
	uint64_t frame_count=0;
	auto dt=world_clock.delayGet();
	auto now=time(NULL);
	while(!window.shouldClose())
		{
		ui.eventsPoll();
		world.update(frame_count,dt,now);
		window.redraw();
		window.buffersSwap();
		world_clock.wait();
		++frame_count;
		}
	}
