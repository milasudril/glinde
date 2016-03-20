#ifdef __WAND__
target[name[windowsystem.h] type[include]]
dependency[windowsystem.o]
#endif

#ifndef GLINDA_WINDOWSYSTEM_H
#define GLINDA_WINDOWSYSTEM_H

namespace Glinda
	{
	class Window;
	class WindowSystem
		{
		public:
			WindowSystem();
			~WindowSystem();

			void eventWindowSet(Window* window) noexcept
				{r_window=window;}

			void inputLoopRun() noexcept;

		private:
			Window* r_window;
		};
	}

#endif
