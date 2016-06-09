#ifdef __WAND__
target[name[actioncompiler_main.o] type[object]]
#endif

#include "actioncompiler_main.h"
#include "actioncompiler_spiderdefault.h"
#include "actioncompiler_targetdefault.h"
#include "actioncompiler_targetcxxloader.h"
#include "actioncompiler_cxxcompiler.h"
#include "stringkey.h"
#include "arraysimple.h"
#include "logwriter.h"
#include "utility.h"
#include "fileout.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

static ArrayDynamic<TargetDefault*> childrenFind(std::map<Stringkey,TargetDefault>& targets)
	{
	ArrayDynamic<TargetDefault*> ret;
	auto i=targets.begin();
	auto i_end=targets.end();
	while(i!=i_end)
		{
		auto& node=i->second;
		if(node.leafIs())
			{
			node.rankSet(0);
			ret.append(&node);
			}
		++i;
		}
	return std::move(ret);
	}

static size_t targetsRank(const Range<Dependency>& deps,size_t rank_current
	,size_t depth_max)
	{
	if(depth_max==0)
		{throw ErrorMessage("It is not possible to compile the action program. Cyclic dependency detected.");}
	auto ptr=deps.begin();
	auto ptr_end=deps.end();
	auto ret=rank_current;
	while(ptr!=ptr_end)
		{
		auto target=ptr->target();
		if(target!=nullptr)
			{
			auto rank=target->visited()?target->rankGet():0;

		//	Update ranks for next levels
			auto rank_next=std::max(rank_current + 1,rank);
			if(rank_next!=rank)
				{
			//	Only process next level if rank changed.
				target->rankSet(rank_next);
				ret=std::max( targetsRank(target->dependencies(),rank_next,depth_max-1), ret);
				}
			else
				{ret=std::max(ret,rank_next);}
			++ptr;
			}
		}
	return ret;
	}

static size_t targetsRank(const Range<TargetDefault*>& children,size_t n_nodes)
	{
	size_t ret=0;
	auto ptr=children.begin();
	auto ptr_end=children.end();
	while(ptr!=ptr_end)
		{
		GLINDE_DEBUG_PRINT("Computing ranks for %s",(*ptr)->nameGet());
		ret=std::max(targetsRank((*ptr)->dependencies(),0,n_nodes),ret);
		++ptr;
		}
	return ret;
	}

static void buildJobCreate(const Range<Dependency>& deps
	,Range<Target*> compile_list)
	{
	auto ptr=deps.begin();
	auto ptr_end=deps.end();
	while(ptr!=ptr_end)
		{
		auto target=ptr->target();
		if(target!=nullptr)
			{
			auto rank=target->rankGet();
			assert(rank>=1);
			if(compile_list[rank]==nullptr)
				{
				GLINDE_DEBUG_PRINT("Target %s has rank %zu/%zu",target->nameGet(),rank,compile_list.length());
				compile_list[rank]=target;
				buildJobCreate(target->dependencies(),compile_list);
				}
			++ptr;
			}
		}
	}

static ArraySimple<Target*> buildJobCreate(TargetDefault& child
	,size_t path_length)
	{
	GLINDE_DEBUG_PRINT("\"%s\" path_length: %zu",child.nameGet(),path_length);
	ArraySimple<Target*> ret(path_length + 1);
	memset(ret.begin(),0,ret.length()*sizeof(Target*));
	ret[0]=&child;
	buildJobCreate(child.dependencies(),ret);
	return std::move(ret);
	}

static void buildJobExecute(const Range<Target*>& targets_rel)
	{
	auto end=targets_rel.end();
	auto begin=targets_rel.begin();
	auto N=0;
	while(end!=begin)
		{
		--end;
		if(*end!=nullptr)
			{
			auto r=Range<const Target* const>
				(const_cast<const Target* const*>(end) + 1,N);
			if(! (*end)->upToDate(r) )
				{(*end)->compile(r);}
			}
		++N;
		}
	}

static void buildAll(const Range<TargetDefault*>& targets,size_t path_length)
	{
	auto ptr=targets.begin();
	auto ptr_end=targets.end();
	while(ptr!=ptr_end)
		{
		buildJobExecute(buildJobCreate(**ptr,path_length));
		++ptr;
		}
	}

void Glinde::ActionCompiler::programCompile(Filesystem& source
	,const char* source_prefix,const char* target_prefix
	,const CxxOptions& options)
	{
	std::map<Stringkey,TargetDefault> targets;
	CxxCompiler cxx(options);

	logWrite(LogMessageType::INFORMATION,"Identified the following C++ compiler:\n");
	logWrite(LogMessageType::LINE_QUOTE,"%s",cxx.versionGet());

	TargetCxxLoader cxxloader(cxx);

	std::map< Stringkey,const TargetLoader* >loaders;
	loaders[Stringkey(".cpp")]=&cxxloader;
	loaders[Stringkey(".cxx")]=&cxxloader;
	loaders[Stringkey(".c++")]=&cxxloader;
	loaders[Stringkey(".CPP")]=&cxxloader;
	loaders[Stringkey(".CXX")]=&cxxloader;
	loaders[Stringkey(".C++")]=&cxxloader;
	loaders[Stringkey(".CC")]=&cxxloader;
	loaders[Stringkey(".cc")]=&cxxloader;
	loaders[Stringkey(".hpp")]=&cxxloader;
	loaders[Stringkey(".hxx")]=&cxxloader;
	loaders[Stringkey(".hh")]=&cxxloader;
	loaders[Stringkey(".h")]=&cxxloader;
	loaders[Stringkey(".h++")]=&cxxloader;
	loaders[Stringkey(".HPP")]=&cxxloader;
	loaders[Stringkey(".HXX")]=&cxxloader;
	loaders[Stringkey(".H++")]=&cxxloader;
	loaders[Stringkey(".HH")]=&cxxloader;
	loaders[Stringkey(".h")]=&cxxloader;


	logWrite(LogMessageType::INFORMATION,"Preparing action program");

	logWrite(LogMessageType::INFORMATION,"Looking for program source files");
	SpiderDefault(targets,target_prefix,loaders,source_prefix).scan(source);

	if(targets.size()==0)
		{throw ErrorMessage("No source files found");}
	logWrite(LogMessageType::INFORMATION,"Searching for child nodes in dependency graph");
	auto children=childrenFind(targets);
	logWrite(LogMessageType::INFORMATION,"Ranking targets");
	auto length_max=targetsRank(children,targets.size());
	logWrite(LogMessageType::INFORMATION,"Compiling action program");
	buildAll(children,length_max);
	logWrite(LogMessageType::INFORMATION,"Action program compiled successfully");
	}
