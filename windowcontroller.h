#ifdef __WAND__
target[name[windowcontroller.h] type[include]]
dependency[windowcontroller.o]
#endif

#ifndef GLINDA_WINDOWCONTROLLER_H
#define GLINDA_WINDOWCONTROLLER_H

namespace Glinda
	{
	class Window;
	class WindowController
		{
		public:
			WindowController();
			~WindowController();

			void eventWindowSet(Window* window) noexcept
				{r_window=window;}

			void inputLoopRun() noexcept;

		private:
			Window* r_window;
		};
	}

#endif
