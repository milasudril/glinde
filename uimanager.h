#ifdef __WAND__
target[name[uimanager.h] type[include]]
dependency[uimanager.o]
#endif

/** \file uimanager.h \brief Defintion of the UIManager class
 */

#ifndef GLINDA_UIMANAGER_H
#define GLINDA_UIMANAGER_H

namespace Glinda
	{
	class Window;
	class Timer;

	/**\brief Handles the event loop and initialization of the windowing system.
	 *
	 * This class handles the event loop and the initialization of the
	 * windowing system.
	 *
	*/
	class UIManager
		{
		public:
			UIManager();
			~UIManager();

			void eventWindowSet(Window* window) noexcept
				{r_window=window;}

			void inputLoopRun(const Timer& world_clock) noexcept;

		private:
			Window* r_window;
		};
	}

#endif
