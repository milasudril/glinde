#ifdef __WAND__
target[name[actioncompiler_targetdirectory.o] type[object]]
#endif

#include "actioncompiler_targetdirectory.h"
#include "logwriter.h"
#include "debug.h"
#include "utility.h"
#include "variant.h"

using namespace Glinde;
using namespace ActionCompiler;

TargetDirectory::TargetDirectory(){}

std::unique_ptr<TargetDirectory> TargetDirectory::create()
	{
	return std::unique_ptr<TargetDirectory>(new TargetDirectory());
	}

void TargetDirectory::compile(Target& target
	,const Range<const Target* const>& targets_related)
	{
	logWrite(Log::MessageType::INFORMATION,"Creating directory #0;"
		,{target.nameGet()});
	Utility::mkdir(target.nameFullGet());
	}

bool TargetDirectory::upToDate(const Target& target
	,const Range<const Target* const>& targets_related) const noexcept
	{
	return Utility::exists(target.nameFullGet());
	}
