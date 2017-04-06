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
