//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[
//@	                {
//@	                    "ref":"dl",
//@	                    "rel":"external"
//@	                }
//@	            ],
//@	            "name":"plugin.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "plugin.h"
#include "errormessage.h"
#include "variant.h"
#include <dlfcn.h>

using namespace Glinde;

Plugin::Plugin(const char* filename):m_name(filename)
	{
	m_handle=dlopen(filename,RTLD_LOCAL|RTLD_NOW|RTLD_DEEPBIND);
	if(m_handle==NULL)
		{
		auto err=dlerror();
		throw ErrorMessage("It was not possible to load the plugin #0;.\n  #1;"
			,{filename,err});
		}
	}

Plugin::~Plugin()
	{
	if(m_handle!=NULL)
		{dlclose(m_handle);}
	}

void Plugin::entryPointGet(const char* name,intptr_t* p_loc) const
	{
	auto sym=dlsym(m_handle,name);
	if(sym==NULL)
		{
		throw ErrorMessage("It was not possible to find the entry point #0; in #1;.\n  #2;"
			,{name,m_name.begin(),dlerror()});
		}
	*p_loc=reinterpret_cast<intptr_t>(sym);
	}
