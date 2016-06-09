#ifdef __WAND__
target[name[actioncompiler_spiderdefault.h] type[include]]
dependency[actioncompiler_spiderdefault.o]
#endif

#ifndef GLINDE_ACTIONCOMPILER_SPIDERDFAULT_H
#define GLINDE_ACTIONCOMPILER_SPIDERDFAULT_H

#include "actioncompiler_spider.h"
#include "arraydynamic.h"
#include "string.h"
#include <set>
#include <map>

namespace Glinde
	{
	class Filesystem;
	class String;
	class Stringkey;

	namespace ActionCompiler
		{
		class TargetDefault;
		class TargetLoader;
		class SpiderDefault:public Spider
			{
			public:
				SpiderDefault(std::map<Stringkey,TargetDefault>&
					,std::map<Stringkey,TargetLoader*>&&)=delete;

				SpiderDefault(std::map<Stringkey,TargetDefault>& targets
					,const char* target_prefix
					,const std::map<Stringkey,const TargetLoader*>& loaders
					,const char* source_prefix);

				~SpiderDefault();

				SpiderDefault& filenameAdd(const char* filename,bool directory_is);

				SpiderDefault& scan(Filesystem& fs);

				Target& targetAdd(const char* name,const char* name_src
					,std::unique_ptr<TargetDetails>&& details);

				const char* sourcePrefixGet() const noexcept
					{return m_src_prefix.begin();}

				const char* targetPrefixGet() const noexcept
					{return m_target_prefix.begin();}

				const TargetLoader* loaderGet(const char* filename) const noexcept;

			private:
				std::map<Stringkey,TargetDefault>& r_targets;
				String m_target_prefix;
				const std::map<Stringkey,const TargetLoader*>& r_loaders;
				String m_src_prefix;
				ArrayDynamic< std::pair<String,bool> > m_files;
				std::set<Stringkey> m_files_visited;
			};
		}
	}

#endif
