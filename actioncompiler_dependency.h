//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"actioncompiler_dependency.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_dependency.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ACTIONCOMPILER_DEPENDENCY_H
#define GLINDE_ACTIONCOMPILER_DEPENDENCY_H

#include "stringkey.h"
#include "string.h"
#include "actioncompiler_target.h"

namespace Glinde
	{
	namespace ActionCompiler
		{
		class Dependency
			{
			public:
				explicit Dependency(Stringkey target_id);

				explicit Dependency(const char* name);

				bool externalIs() const noexcept
					{return m_name.length()!=0;}

				const Stringkey& idGet() const noexcept
					{return m_target_id;}

				const char* nameGet() const noexcept
					{
					return r_target==nullptr?
						m_name.begin() : r_target->nameGet();
					}

				Dependency& targetSet(Target* target) noexcept
					{
					r_target=target;
					return *this;
					}

				const Target* target() const noexcept
					{return r_target;}

				Target* target() noexcept
					{return r_target;}

			private:
				String m_name;
				Stringkey m_target_id;
				Target* r_target;
			};
		}
	}

#endif
