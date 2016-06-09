#ifdef __WAND__
target[name[actionprogram.h] type[include]]
dependency[actionprogram.o]
#endif

#ifndef GLINDE_ACTIONPROGRAM_H
#define GLINDE_ACTIONPROGRAM_H

#include "plugin.h"
#include "string.h"

namespace Glinde
	{
	class Filesystem;
	class SdkResource;

	class ActionProgram
		{
		public:
			ActionProgram(Filesystem&& source,const char* name
				,const Range<const SdkResource>& sdk):
				ActionProgram(source,name,sdk){}

			ActionProgram(Filesystem& source,const char* name
				,const Range<const SdkResource>& sdk);

			template<class Function>
			Function entryPointGet(const char* name) const
				{return m_plug.entryPointGet<Function>(name);}

		private:
			String m_dirtemp;
			Plugin m_plug;
		};
	}

#endif
