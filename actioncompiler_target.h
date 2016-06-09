#ifdef __WAND__
target[name[actioncompiler_target.h] type[include]]
#endif

#ifndef GLINDE_ACTIONCOMPILER_TARGET_H
#define GLINDE_ACTIONCOMPILER_TARGET_H

#include "range.h"

namespace Glinde
	{
	namespace ActionCompiler
		{
		class Dependency;

		class TargetDetails;

		class Target
			{
			public:
				virtual bool completed() const noexcept=0;

				virtual bool upToDate(const Range<const Target* const>& targets_rel) const noexcept=0;

				virtual void compile(const Range<const Target* const>& targets_rel)=0;

				virtual Range<Dependency> dependencies() noexcept=0;

				virtual Range<const Dependency> dependencies() const noexcept=0;

				virtual Target& dependencyAdd(Dependency&& dep)=0;

				virtual size_t rankGet() const noexcept=0;

				virtual Target& rankSet(size_t rank) noexcept=0;

				virtual bool leafIs() const noexcept=0;

				virtual bool visited() const noexcept=0;

				virtual size_t& childrenCount() noexcept=0;

				virtual const char* nameGet() const noexcept=0;

				virtual const char* nameFullGet() const noexcept=0;

				virtual const char* nameSourceGet() const noexcept=0;

				virtual const char* nameSourceFullGet() const noexcept=0;

				virtual const TargetDetails& detailsGet() const noexcept=0;
			};
		}
	}

#endif

