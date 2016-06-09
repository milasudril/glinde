#ifdef __WAND__
target[name[actioncompiler_spider.h] type[include]]
#endif

#ifndef GLINDE_ACTIONCOMPILER_SPIDER_H
#define GLINDE_ACTIONCOMPILER_SPIDER_H

#include <memory>
#include <utility>

namespace Glinde
	{
	class Filesystem;

	namespace ActionCompiler
		{
		class Target;
		class TargetDetails;
		class TargetLoader;

		class Spider
			{
			public:
				virtual Spider& scan(Filesystem& fs)=0;
				virtual Spider& filenameAdd(const char* filename,bool directory_is)=0;
				virtual Target& targetAdd(const char* name,const char* name_src
					,std::unique_ptr<TargetDetails>&& details)=0;
				virtual const char* sourcePrefixGet() const noexcept=0;
				virtual const char* targetPrefixGet() const noexcept=0;
				virtual const TargetLoader* loaderGet(const char* filename) const noexcept=0;

			};
		}
	}
#endif
