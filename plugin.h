//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"plugin.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"plugin.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}

#ifndef GLINDE_PLUGIN_H
#define GLINDE_PLUGIN_H

#include "string.h"
#include <cstdint>

namespace Glinde
	{
	class Plugin
		{
		public:
			Plugin(const Plugin&)=delete;
			Plugin& operator=(const Plugin&)=delete;
			Plugin(Plugin&& obj):m_name(std::move(obj.m_name))
				,m_handle(obj.m_handle)
				{obj.m_handle=NULL;}

			Plugin& operator=(Plugin&& obj)
				{
				std::swap(obj.m_name,m_name);
				std::swap(obj.m_handle,m_handle);
				return *this;
				}

			explicit Plugin(const char* filename);
			~Plugin();

			template<class Entry>
			Entry entryPointGet(const char* name) const
				{
				Entry ret;
				entryPointGet(name,reinterpret_cast<intptr_t*>(&ret));
				return ret;
				}

		private:
			String m_name;
			void* m_handle;
			void entryPointGet(const char* name,intptr_t* result) const;
		};
	}

#endif

