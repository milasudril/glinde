#ifdef __WAND__
target[name[actioncompiler_cxxcompiler.h] type[include]]
dependency[actioncompiler_cxxcompiler.o]
#endif

#ifndef GLINDE_ACTIONCOMPILER_CXXCOMPILER_H
#define GLINDE_ACTIONCOMPILER_CXXCOMPILER_H

#include "actioncompiler_cxxoptions.h"

namespace Glinde
	{
	namespace ActionCompiler
		{
		class CxxCompiler
			{
			public:
				explicit CxxCompiler(const CxxOptions& options);

				const char* versionGet() const
					{return m_version.begin();}

				void compile(const char* source,const char* dest,const CxxOptions& options) const;

				void link(const Range<const String>& files,const char* dest
					,const CxxOptions& options) const;

				CxxOptions& options() noexcept
					{return m_options;}

				const CxxOptions& options() const noexcept
					{return m_options;}

				long int cxxversionDefault() const noexcept
					{return m_cxxversion_default;}

			private:
				String m_version;
				CxxOptions m_options;
				long int m_cxxversion_default;
			};
		}
	}

#endif
