#ifdef __WAND__
target[name[actioncompiler_targetcxxloader.h] type[include]]
dependency[actioncompiler_targetcxxloader.o]
#endif

#ifndef ACTIONCOMPILER_TARGETCXXLOADER_H
#define ACTIONCOMPILER_TARGETCXXLOADER_H

#include "actioncompiler_targetloader.h"
#include "string.h"

namespace Glinde
	{
	namespace ActionCompiler
		{
		class CxxCompiler;
		class TargetCxxLoader:public TargetLoader
			{
			public:
				explicit TargetCxxLoader(CxxCompiler& compiler):r_compiler(compiler)
					{}

				void targetsLoad(const char* name_src,const char* in_dir
					,Spider& spider) const;

				TargetCxxLoader& includeDirAdd(const char* dir_name);

			private:
				CxxCompiler& r_compiler;
				ArrayDynamic<String> m_include_dir;
			};
		}
	}

#endif
