#ifdef __WAND__
target[name[window.h] type[include]]
dependency[window.o]
#endif

#ifndef GLINIE_WINDOW_H
#define GLINIE_WINDOW_H

#include <cstdint>

namespace Glinie
	{
	class Window
		{
		public:
			class EventHandler
				{
				public:
					virtual void onKeyDown(Window& source,uint8_t scancode)=0;
					virtual void onKeyUp(Window& source,uint8_t scancode)=0;
					virtual void onTextInput(Window& source,uint32_t codepoint)=0;

					virtual void onMouseMove(Window& source,double x,double y)=0;
					virtual void onMouseDown(Window& source,int button)=0;
					virtual void onMouseUp(Window& source,int button)=0;

					virtual void onResize(Window& source,int width,int height)=0;
				};

			Window(const char* title,unsigned int width, unsigned int height
				,EventHandler& handler);
			~Window();

			void buffersSwap() noexcept;
			bool shouldClose() noexcept;
			void stickyKeysSet() noexcept;

			EventHandler& eventHandlerGet() noexcept
				{return *r_handler;}

			void eventHandlerSet(EventHandler& handler_new) noexcept
				{r_handler=&handler_new;}

			virtual void redraw() noexcept
				{}

			virtual void resized(int width,int height) noexcept
				{}

			void sizeGet(int& width,int& height) noexcept;

			void cursorPositionSet(double x,double y) noexcept;
			void cursorHide() noexcept;
			void cursorShow() noexcept;
			void cursorHideAndGrab() noexcept;

		private:
			EventHandler* r_handler;
			void* m_handle;
		};
	}

#endif
