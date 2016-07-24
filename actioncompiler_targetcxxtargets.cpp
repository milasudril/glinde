//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_targetcxxtargets.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "actioncompiler_targetcxxtargets.h"
#include "actioncompiler_cxxcompiler.h"
#include "arraysimple.h"
#include "logwriter.h"
#include "utility.h"
#include "variant.h"

using namespace Glinde;
using namespace ActionCompiler;

TargetCxxObject::TargetCxxObject(const ResourceObject& target,const CxxCompiler& cxx):
	TargetCxx(cxx)
	{
	options().compileOnlyEnable();
	}

std::unique_ptr<TargetCxxObject>
TargetCxxObject::create(const ResourceObject& target,const CxxCompiler& cxx)
	{
	return std::unique_ptr<TargetCxxObject>( new TargetCxxObject(target,cxx) );
	}

void TargetCxxObject::compile(Target& target
	,const Range<const Target* const>& targets_rel)
	{
	logWrite(Log::MessageType::INFORMATION,"Compiling #0;",{target.nameSourceGet()});

	compilerGet().compile(target.nameSourceFullGet(),target.nameFullGet()
		,options());
	}

bool TargetCxxObject::upToDate(const Target& target,const Range<const Target* const>& targets_related) const noexcept
	{
	GLINDE_DEBUG_PRINT("Checking status for #0;",target.nameGet());
	if(Utility::newer(target.nameSourceGet(),target.nameFullGet()))
		{return 0;}
	auto ptr=targets_related.begin();
	auto ptr_end=targets_related.end();
	while(ptr!=ptr_end)
		{
		auto x=*ptr;
		GLINDE_DEBUG_PRINT("  #0;",x->nameGet());
		if(dynamic_cast<const TargetCxxInclude*>( &(x->detailsGet()) )!=nullptr)
			{
			if(Utility::newer(x->nameSourceFullGet(),x->nameFullGet()))
				{return 0;}
			}
		++ptr;
		}
	return 1;
	}



TargetCxxDLL::TargetCxxDLL(const ResourceObject& target,const CxxCompiler& cxx):
	TargetCxx(cxx)
	{
	options().sharedEnable();
	}

std::unique_ptr<TargetCxxDLL>
TargetCxxDLL::create(const ResourceObject& target,const CxxCompiler& cxx)
	{
	return std::unique_ptr<TargetCxxDLL>( new TargetCxxDLL(target,cxx) );
	}

void TargetCxxDLL::compile(Target& target
	,const Range<const Target* const>& targets_rel)
	{
	logWrite(Log::MessageType::INFORMATION,"Compiling #0;",{target.nameSourceGet()});

//	TODO: External/internal libraries?
	ArrayDynamic<String> files;
	files.append( String(target.nameSourceFullGet()) );
	auto ptr_begin=targets_rel.begin();
	auto ptr_end=targets_rel.end();
	while(ptr_begin!=ptr_end)
		{
		--ptr_end;
		auto x=*ptr_end;
		if(dynamic_cast<const TargetCxxObject*>(&(x->detailsGet()))!=nullptr)
			{
			files.append(String(x->nameFullGet()));
			}
		}

 	compilerGet().link(files,target.nameFullGet(),options());
	}

bool TargetCxxDLL::upToDate(const Target& target,const Range<const Target* const>& targets_related) const noexcept
	{
	GLINDE_DEBUG_PRINT("Checking status for #0;",target.nameGet());
	if(Utility::newer(target.nameSourceGet(),target.nameFullGet()))
		{return 0;}
	auto ptr=targets_related.begin();
	auto ptr_end=targets_related.end();
	while(ptr!=ptr_end)
		{
		auto x=*ptr;
		GLINDE_DEBUG_PRINT("  #0;",x->nameGet());
		if(Utility::newer(x->nameSourceFullGet(),x->nameFullGet()))
			{return 0;}
		++ptr;
		}
	return 1;
	}



TargetCxxInclude::TargetCxxInclude(const ResourceObject& target,const CxxCompiler& cxx):
	TargetCxx(cxx)
	{
	}

std::unique_ptr<TargetCxxInclude>
TargetCxxInclude::create(const ResourceObject& target,const CxxCompiler& cxx)
	{
	return std::unique_ptr<TargetCxxInclude>( new TargetCxxInclude(target,cxx) );
	}

void TargetCxxInclude::compile(Target& target
	,const Range<const Target* const>& targets_rel)
	{
	}

bool TargetCxxInclude::upToDate(const Target& target,const Range<const Target* const>& targets_related) const noexcept
	{
	return 1;
	}
