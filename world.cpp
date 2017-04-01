//@	{
//@	 "targets":[{"name":"world.o","type":"object"}]
//@	}

#include "world.hpp"
#include "errormessage.hpp"
#include "variant.hpp"
#include "harvester/harvester.hpp"

using namespace Glinde;

namespace
	{
	class HarvesterExecutionControl
		{
		public:
			explicit HarvesterExecutionControl(const char* src):filename(src)
				{}

			void raise(const char* message)
				{
				throw ErrorMessage("It was not possible to extract game data from #0;. #1;.",{filename,message});
				}

			void progress(double x,const char* name)
				{
				
				}

		private:
			const char* filename;
		};
	}

World::World(MessageQueue& msgqueue,const char* filename):r_msgqueue(&msgqueue)
	,m_dirtemp(Harvester::extract(filename,"/tmp",HarvesterExecutionControl{filename}))
	{}

World::~World()
	{}