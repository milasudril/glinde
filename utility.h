//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"utility.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"utility.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_UTILITY_H
#define GLINDE_UTILITY_H

#include "string.h"

namespace Glinde
	{
	namespace Utility
		{
		void mkdir(const char* dirname);
		String mkdtemp(const char* prefix);
		void rmdir(const char* dirname);
		bool exists(const char* filename) noexcept;
		bool newer(const char* filename_a,const char* filename_b) noexcept;
		}
	}

#endif
