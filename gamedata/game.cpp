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
			explicit HarvesterExecutionControl(const char* src):filename(src),first(1)
				{}

			void raise(const char* message)
				{
				throw ErrorMessage("It was not possible to extract any game data from #0;. #1;.",{filename,message});
				}

			void progress(double x,const char* name)
				{
				if(first)
					{
					logWrite(Log::MessageType::INFORMATION,"Extracting game data from #0;",{filename});
					first=0;
					}
				}

		private:
			const char* filename;
			bool first;
		};
	}

Game::Game(MessageQueue& msgqueue,const char* filename):r_msgqueue(&msgqueue)
	,m_dirtemp(Harvester::extract(filename,"/tmp",HarvesterExecutionControl{filename}))
	{}

Game::~Game()
	{}
