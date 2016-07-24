//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_dependency.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "actioncompiler_dependency.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

Dependency::Dependency(Stringkey target_id):
	m_target_id(target_id),r_target(nullptr)
	{}

Dependency::Dependency(const char* name):
	m_name(name),m_target_id(""),r_target(nullptr)
	{}
