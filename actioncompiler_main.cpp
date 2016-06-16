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
#include "variant.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

static ArrayDynamic<const TargetDefault*> childrenFind(std::map<Stringkey,TargetDefault>& targets)
	{
	ArrayDynamic<const TargetDefault*> ret;
	auto i=targets.begin();
	auto i_end=targets.end();
	while(i!=i_end)
		{
		auto& node=i->second;
		if(node.leafIs())
			{ret.append(&node);}
		++i;
		}
	return std::move(ret);
	}

static void toposort(const Target& target_current,const Range<uint32_t>& colors
	,ArrayDynamic<const Target*>& targets_out)
	{
	auto id=target_current.idGet();
	colors[id]=1;
	auto deps=target_current.dependencies();
	auto pos=deps.begin();
	while(pos!=deps.end())
		{
		auto target_next=pos->target();
		if(target_next==nullptr)
			{
			throw ErrorMessage("The target #0; contains an undefined reference to #1;."
				,{target_current.nameGet(),pos->nameGet()});
			}
		switch(colors[target_next->idGet()])
			{
			case 1:
				throw ErrorMessage("A cyclic dependency between #0; and #1; was detected."
					,{target_current.nameGet(),target_next->nameGet()});
			case 0:
				toposort(*target_next,colors,targets_out);
			}
		++pos;
		}
	colors[id]=2;
	targets_out.append(&target_current);
	}

static void buildJobExecute(const Range<const Target* const>& targets)
	{
	auto target_current=targets.begin();
	auto targets_end=targets.end();
	while(target_current!=targets_end)
		{
		auto targets_rel=Range<const Target* const>(targets.begin(),target_current);
		if(!( (*target_current)->upToDate(targets_rel) ))
			{const_cast<Target*>(*target_current)->compile(targets_rel);}

		++target_current;
		}
	}

static void buildAll(const Range<const TargetDefault* const>& children,size_t n_nodes)
	{
	GLINDE_DEBUG_PRINT("There are #0; nodes",n_nodes);

	ArraySimple<uint32_t> colors(n_nodes);
	ArrayDynamic<const Target*> targets;

	memset(colors.begin(),0,sizeof(uint32_t)*n_nodes);

	auto child_current=children.begin();
	auto children_end=children.end();
	while(child_current!=children_end)
		{
		memset(colors.begin(),0,sizeof(uint32_t)*n_nodes);
		targets.clear();
		toposort(**child_current,colors,targets);
		buildJobExecute(targets);
		++child_current;
		}
	}

void Glinde::ActionCompiler::programCompile(Filesystem& source
	,const char* source_prefix,const char* target_prefix
	,const CxxOptions& options)
	{
	std::map<Stringkey,TargetDefault> targets;
	CxxCompiler cxx(options);

	logWrite(Log::MessageType::INFORMATION,"Identified the following C++ compiler:\n",{});
	logWrite(Log::MessageType::LINE_QUOTE,"#0;",{cxx.versionGet()});

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


	logWrite(Log::MessageType::INFORMATION,"Preparing action program",{});

	logWrite(Log::MessageType::INFORMATION,"Looking for program source files",{});
	SpiderDefault(targets,target_prefix,loaders,source_prefix).scan(source);

	if(targets.size()==0)
		{throw ErrorMessage("No source files found",{});}
	logWrite(Log::MessageType::INFORMATION,"Searching for child nodes in dependency graph",{});
	auto children=childrenFind(targets);
	logWrite(Log::MessageType::INFORMATION,"Compiling action program",{});

	buildAll(children,targets.size());

	logWrite(Log::MessageType::INFORMATION,"Action program compiled successfully",{});
	}
