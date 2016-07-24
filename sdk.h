//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"sdk.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"sdk.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_SDK_H
#define GLINDE_SDK_H

#include "range.h"

namespace Glinde
	{
	class SdkResource;
	Range<const SdkResource> sdk() noexcept;
	}

#endif
