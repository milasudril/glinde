#ifdef __WAND__
target[name[actioncompiler_targetdirectory.h] type[include]]
dependency[actioncompiler_targetdirectory.o]
#endif

#ifndef GLINDE_ACTIONCOMPILER_TARGETDIRECTORY_H
#define GLINDE_ACTIONCOMPILER_TARGETDIRECTORT_H

#include "actioncompiler_targetdetails.h"
#include <memory>

namespace Glinde
	{
	namespace ActionCompiler
		{
		class Spider;
		class Target;

		class TargetDirectory:public TargetDetails
			{
			public:
				TargetDirectory();
				void compile(Target& target
					,const Range<const Target* const>& targets_related);

				bool upToDate(const Target& target
					,const Range<const Target* const>& targets_related) const noexcept;

				static std::unique_ptr<TargetDirectory> create();
			};
		}
	}

#endif
