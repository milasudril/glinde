//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"strerror.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "strerror.h"

#ifdef _GNU_SOURCE

extern "C"
    {
    extern int __xpg_strerror_r(int errcode,char* buffer,size_t length);
    #define strerror_r __xpg_strerror_r
    }

#else

#include <cstring>

#endif

using namespace Glinde;

int Glinde::strerror(int errcode,char* buffer,size_t length)
	{
	auto ret=strerror_r(errcode,buffer,length);
	buffer[length-1]=0;
	return ret;
	}
