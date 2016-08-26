//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"enginedefault.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"enginedefault.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ENGINEDEFAULT_H
#define GLINDE_ENGINEDEFAULT_H

#include "engine.h"
#include "console.h"
#include "logwriterconsole.h"
#include "handle.h"
#include "string.h"
#include "message.h"

namespace Glinde
	{
	class MessageQueue;
	class Window;
	class Timer;
	class Clock;
	class WorldDefault;

	template<class Runner>
	class Thread;

	class EngineDefault:public Engine
		{
		public:
			EngineDefault(MessageQueue& messages);
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

			MessageQueue& r_messages;
			class WorldOwner
				{
				public:
					virtual void operator()(uint64_t time,WorldDefault* world);

					WorldDefault* world() noexcept
						{return m_world;}

					WorldOwner();
					~WorldOwner();

				private:
					WorldDefault* m_world;
				};
			Message::Processor<WorldDefault*,WorldOwner> m_world_status;

			Window* r_window;
			const Timer* r_timer;
			Clock* r_clock;

			Thread<WorldLoader>* m_world_loader;

			Console m_con;
			LogWriterConsole m_log;
			uint32_t m_con_index;

			uint64_t m_frame_current;
			uint32_t m_message_count;
			bool m_stop;


			void worldLoadedPost(WorldDefault* world);
			void cleanup();
		};
	}

#endif
