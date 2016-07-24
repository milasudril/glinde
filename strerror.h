//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"strerror.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"strerror.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_STRERROR_H
#define GLINDE_STRERROR_H

#include <cstddef>

namespace Glinde
	{
	int strerror(int errcode,char* buffer,size_t length);
	}

#endif
