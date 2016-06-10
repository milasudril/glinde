#ifdef __WAND__
target[name[enginedefault.h] type[include]]
dependency[enginedefault.o]
#endif

#ifndef GLINDE_ENGINEDEFAULT_H
#define GLINDE_ENGINEDEFAULT_H

#include "engine.h"
#include "console.h"
#include "logwriterconsole.h"
#include "messagequeue.h"
#include "string.h"

namespace Glinde
	{
	class Window;
	class Timer;
	class Clock;
	class WorldDefault;
	class Thread;

	class EngineDefault:public Engine
		{
		public:
			EngineDefault();
			~EngineDefault();

			void run();

			EngineDefault& windowSet(Window* window) noexcept
				{
				r_window=window;
				return *this;
				}

			EngineDefault& timerSet(const Timer* timer) noexcept;

			EngineDefault& framecounterSet(uint64_t fc) noexcept
				{
				m_frame_current=fc;
				return *this;
				}

			void stop();
			void consoleWrite(const char* string);
			void consoleWrite(char ch);
			void consoleColorMaskSet(uint8_t color_mask);

			const Console& consoleGet() const noexcept
				{return m_con;}

			EngineDefault& worldLoadAsync(const char* file);

		private:
			class WorldLoader;

			Window* r_window;
			const Timer* r_timer;
			Clock* r_clock;

			WorldDefault* m_world;
			Thread* m_world_loader_task;
			WorldLoader* m_world_loader;

			Console m_con;
			LogWriterConsole m_log;
			MessageQueue m_messages;
			uint64_t m_frame_current;
			uint32_t m_message_count;
			bool m_stop;


			void worldLoadedPost(WorldDefault* world);

			void messageProcess(const Message& msg);
		};
	}

#endif
