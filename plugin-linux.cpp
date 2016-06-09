#ifdef __WAND__
target[name[plugin.o] type[object] platform[;GNU/Linux] dependency[dl;external] ]
#endif

#include "plugin.h"
#include "errormessage.h"
#include <dlfcn.h>

using namespace Glinde;

Plugin::Plugin(const char* filename):m_name(filename)
	{
	m_handle=dlopen(filename,RTLD_LOCAL|RTLD_NOW|RTLD_DEEPBIND);
	if(m_handle==NULL)
		{
		auto err=dlerror();
		for(int k=0;k<2;++k)
			{
			while(*err!=':' && *err!='\0')
				{++err;}
			if(*err=='\0')
				{break;}
			++err;
			}
		throw ErrorMessage("It was not possible to load the plugin %s.%s"
			,filename,err);
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
		throw ErrorMessage("It was not possible to find the entry point %s in %s. %s"
			,name,m_name.begin(),dlerror());
		}
	*p_loc=reinterpret_cast<intptr_t>(sym);
	}
