#ifdef __WAND__
target[name[gameloop.h] type[include]]
dependency[gameloop.o]
#endif

/**\file gameloop.h \brief Declares the gameLoopRun function
 *
 */

#ifndef GLINIE_GAMELOOP_H
#define GLINIE_GAMELOOP_H

namespace Glinie
	{
	class Window;
	class Timer;
	class World;
	class UIManager;


	/**\brief The main game loop
	 *
	 * This function runs the main game loop. Each iteration proceeds like this:
	 *
	 *  1. Check if the application should close. If so, exit the loop.
	 *  2. Check for input events by calling eventsPoll on the UIManager. This
	 *     step may update the World, or any of its WorldObjects, through
	 *     UserEventHandlers.
	 *  3. Update the World
	 *  4. Update the Window
	 *  5. Wait on the Timer for next frame
	 *
	 * \param ui The UIManager resposible for delegating input events
	 * \param window The Window that shows the world
	 * \param world_clock The Timer object used for controlling the loop speed
	 * \param world The World evolving through this loop
	*/
	void gameLoopRun(UIManager& ui,Window& window
		,const Timer& world_clock,World& world);
	}

#endif
