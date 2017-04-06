/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
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
