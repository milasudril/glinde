//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"actioncompiler_targetdirectory.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_targetdirectory.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
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
