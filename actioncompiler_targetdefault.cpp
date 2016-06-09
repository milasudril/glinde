#ifdef __WAND__
target[name[actioncompiler_targetdefault.o] type[object]]
#endif

#include "actioncompiler_targetdefault.h"
#include "logwriter.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

TargetDefault::TargetDefault(const char* name,const char* name_src
	,std::unique_ptr<TargetDetails>&& details):
	 m_name(name),m_name_full(name)
	,m_name_src(name_src),m_name_src_full(name_src)
	,m_rank(static_cast<size_t>(-1))
	,m_child_count(0),m_details(std::move(details))
	{
	}

TargetDefault& TargetDefault::sourcePrefixSet(const char* prefix)
	{
	m_name_src_full=String(prefix).append('/').append(m_name_src);
	return *this;
	}

TargetDefault& TargetDefault::targetPrefixSet(const char* prefix)
	{
	m_name_full=String(prefix).append('/').append(m_name);
	if(m_name.length()==0)
		{m_name_full.truncate();}
	return *this;
	}
