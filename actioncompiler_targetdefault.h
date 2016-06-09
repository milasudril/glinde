#ifdef __WAND__
target[name[actioncompiler_targetdefault.h] type[include]]
dependency[actioncompiler_targetdefault.o]
#endif

#ifndef GLINDE_ACTIONCOMPILER_TARGETDEFAULT_H
#define GLINDE_ACTIONCOMPILER_TARGETDEFAULT_H

#include "actioncompiler_target.h"
#include "actioncompiler_dependency.h"
#include "actioncompiler_targetdetails.h"
#include "string.h"
#include <memory>

namespace Glinde
	{
	class DataSource;
	namespace ActionCompiler
		{
		class Spider;
		class TargetDetails;
		class TargetDefault:public Target
			{
			public:
				TargetDefault(const char* name,const char* name_src
					,std::unique_ptr<TargetDetails>&& details);

				Range<Dependency> dependencies() noexcept
					{return m_dependencies;}

				Range<const Dependency> dependencies() const noexcept
					{return m_dependencies;}

				TargetDefault& dependencyAdd(Dependency&& dep)
					{
					m_dependencies.append(std::move(dep));
					return *this;
					}

				size_t rankGet() const noexcept
					{return m_rank;}

				TargetDefault& rankSet(size_t rank) noexcept
					{
					m_rank=rank;
					return *this;
					}

				bool leafIs() const noexcept
					{return m_child_count==0;}

				bool visited() const noexcept
					{return m_rank!=static_cast<size_t>(-1);}

				size_t& childrenCount() noexcept
					{return m_child_count;}

				const char* nameGet() const noexcept
					{return m_name.begin();}

				const char* nameFullGet() const noexcept
					{return m_name_full.begin();}

				const char* nameSourceGet() const noexcept
					{return m_name_src.begin();}

				TargetDefault& targetPrefixSet(const char* prefix);

				const char* nameSourceFullGet() const noexcept
					{return m_name_src_full.begin();}

				TargetDefault& sourcePrefixSet(const char* prefix);

				bool completed() const noexcept
					{return m_details!=nullptr;}

				void compile(const Range<const Target* const>& targets_rel)
					{
					assert(m_details!=nullptr);
					m_details->compile(*this,targets_rel);
					}

				bool upToDate(const Range<const Target* const>& targets_rel) const noexcept
					{return m_details->upToDate(*this,targets_rel);}

				const TargetDetails& detailsGet() const noexcept
					{return *m_details;}

			private:
				String m_name;
				String m_name_full;
				String m_name_src;
				String m_name_src_full;
				size_t m_rank;
				size_t m_child_count;
				ArrayDynamic<Dependency> m_dependencies;
				std::unique_ptr<TargetDetails> m_details;
			};
		}
	}

#endif
