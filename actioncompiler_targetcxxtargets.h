#ifdef __WAND__
target[name[actioncompiler_targetcxxtargets.h] type[include]]
dependency[actioncompiler_targetcxxtargets.o]
#endif

#ifndef GLINDE_ACTIONCOMPILER_TARGETCXXTARGETS_H
#define GLINDE_ACTIONCOMPILER_TARGETCXXTARGETS_H

#include "actioncompiler_targetdetails.h"
#include "actioncompiler_cxxoptions.h"
#include "string.h"
#include <memory>

namespace Glinde
	{
	class ResourceObject;

	namespace ActionCompiler
		{
		class CxxCompiler;

		class TargetCxx:public TargetDetails
			{
			public:
				TargetCxx(CxxCompiler&&)=delete;

				explicit TargetCxx(const CxxCompiler& compiler):
					r_compiler(compiler){}

				const CxxCompiler& compilerGet() const noexcept
					{return r_compiler;}

				CxxOptions& options() noexcept
					{return m_options;}

				const CxxOptions& options() const noexcept
					{return m_options;}

			private:
				const CxxCompiler& r_compiler;
				CxxOptions m_options;
			};


		class TargetCxxInclude:public TargetCxx
			{
			public:
				TargetCxxInclude(const ResourceObject& target,CxxCompiler&& cxx)=delete;
				TargetCxxInclude(const ResourceObject& target,const CxxCompiler& cxx);

				static std::unique_ptr<TargetCxxInclude>
				create(const ResourceObject& target, CxxCompiler&& cxx)=delete;

				static std::unique_ptr<TargetCxxInclude>
				create(const ResourceObject& target	,const CxxCompiler& cxx);

				void compile(Target& target,const Range<const Target* const>& targets_related);

				bool upToDate(const Target& target,const Range<const Target* const>& targets_related) const noexcept;
			};

		class TargetCxxObject:public TargetCxx
			{
			public:
				TargetCxxObject(const ResourceObject& target,CxxCompiler&& cxx)=delete;
				TargetCxxObject(const ResourceObject& target,const CxxCompiler& cxx);

				static std::unique_ptr<TargetCxxObject>
				create(const ResourceObject& target,CxxCompiler&& cxx)=delete;
				static std::unique_ptr<TargetCxxObject>
				create(const ResourceObject& target,const CxxCompiler& cxx);

				void compile(Target& target,const Range<const Target* const>& targets_related);

				bool upToDate(const Target& target,const Range<const Target* const>& targets_related) const noexcept;
			};

		class TargetCxxDLL:public TargetCxx
			{
			public:
				TargetCxxDLL(const ResourceObject& target,CxxCompiler&& cxx)=delete;
				TargetCxxDLL(const ResourceObject& target,const CxxCompiler& cxx);

				static std::unique_ptr<TargetCxxDLL>
				create(const ResourceObject& target,CxxCompiler&& cxx)=delete;

				static std::unique_ptr<TargetCxxDLL>
				create(const ResourceObject& target,const CxxCompiler& cxx);

				void compile(Target& target,const Range<const Target* const>& targets_related);

				bool upToDate(const Target& target,const Range<const Target* const>& targets_related) const noexcept;
			};
		}
	}

#endif
