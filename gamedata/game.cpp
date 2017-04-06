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
//@	 "targets":[{"name":"game.o","type":"object"}]
//@	}

#include "game.hpp"
#include "../errormessage.hpp"
#include "../variant.hpp"
#include "../harvester/harvester.hpp"
#include "../log/logwriter.hpp"

using namespace Glinde;

namespace
	{
	class HarvesterExecutionControl
		{
		public:
			explicit HarvesterExecutionControl(const char* src):filename(src),first(1),x_0(0)
				{}

			void raise(const char* message)
				{
				throw ErrorMessage("It was not possible to extract any game data from #0;. #1;.",{filename,message});
				}

			Harvester::ProgressStatus progress(double x,const char* name)
				{
				if(first)
					{
					logWrite(Log::MessageType::INFORMATION,"Extracting game data from #0;",{filename});
					logProgressInit();
					first=0;
					}
				assert(x>=x_0);
				x_0=x;
				logProgress(x,name);
				return Harvester::ProgressStatus::EXTRACT;
				}

		private:
			const char* filename;
			bool first;
			double x_0;
		};
	}

Game::Game(MessageQueue& msgqueue,const char* filename):r_msgqueue(&msgqueue)
	,m_dirtemp(Harvester::extract(filename,"/tmp",HarvesterExecutionControl{filename}))
	{
	logProgressEnd();
	logWrite(Log::MessageType::INFORMATION,"Game data extracted from #0;",{filename});
	}

Game::~Game()
	{}
