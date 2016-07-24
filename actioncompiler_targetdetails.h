//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_targetdetails.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ACTIONCOMPILER_TARGETDETAILS_H
#define GLINDE_ACTIONCOMPILER_TARGETDETAILS_H

#include "range.h"
#include "actioncompiler_target.h"

namespace Glinde
	{
	namespace ActionCompiler
		{
		class TargetDetails
			{
			public:
				virtual void compile(Target& target
					,const Range<const Target* const>& targets_related)=0;
				virtual bool upToDate(const Target& target
					,const Range<const Target* const>& targets_related) const noexcept=0;
				virtual ~TargetDetails()=default;
			};
		}
	}

#endif
