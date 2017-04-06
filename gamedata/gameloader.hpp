//@	{
//@	 "targets":[{"name":"gameloader.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"gameloader.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_GAMELOADER_HPP
#define GLINDE_GAMELOADER_HPP

#include "game.hpp"
#include "../enginebase.hpp"
#include "../string.hpp"

namespace Glinde
	{
	class GameLoader
		{
		public:
			GameLoader(const GameLoader&)=delete;
			GameLoader& operator=(const GameLoader&)=delete;

			GameLoader(GameLoader&& g) noexcept;

			GameLoader& operator=(GameLoader&& g)=delete; //For now...

			explicit GameLoader(const char* src,EngineBase& eng,MessageQueue& msgqueue):
				m_src(src),r_msgqueue(&msgqueue),r_eng(&eng)
				{}

			void operator()();

			const ErrorMessage& error() const noexcept
				{return m_error;}

		private:
			String m_src;
			MessageQueue* r_msgqueue;
			EngineBase* r_eng;
			ErrorMessage m_error;
		};
	}

#endif
