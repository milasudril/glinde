#ifdef __WAND__
target[name[uimanager.h] type[include]]
dependency[uimanager.o]
#endif

/** \file uimanager.h \brief Defintion of the UIManager class
 */

#ifndef GLINIE_UIMANAGER_H
#define GLINIE_UIMANAGER_H

namespace Glinie
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

			void eventsPoll();
		};
	}

#endif
