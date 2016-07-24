//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"actioncompiler_main.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_main.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ACTIONCOMPILER_MAIN_H
#define GLINDE_ACTIONCOMPILER_MAIN_H

namespace Glinde
	{
	class Filesystem;
	template<class T>
	class Range;

	namespace ActionCompiler
		{
		class CxxOptions;

		void programCompile(Filesystem& source,const char* source_prefix
			,const char* target_prefix,const CxxOptions& options);
		}
	}

#endif
