//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_targetloader.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ACTIONCOMPILER_TARGETLOADER_H
#define GLIDNE_ACTIONCOMPILER_TARGETLOADER_H

namespace Glinde
	{
	namespace ActionCompiler
		{
		class Spider;
		class TargetLoader
			{
			public:
				virtual void targetsLoad(const char* name_src,const char* in_dir
					,Spider& spider) const=0;
			};
		}
	}

#endif
