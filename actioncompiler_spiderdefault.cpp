#ifdef __WAND__
target[name[actioncompiler_spiderdefault.o] type[object]]
#endif

#include "actioncompiler_spiderdefault.h"
#include "actioncompiler_targetdefault.h"
#include "actioncompiler_targetcxxloader.h"
#include "actioncompiler_targetdirectory.h"
#include "string.h"
#include "filesystem.h"
#include "logwriter.h"
#include "stringkey.h"
#include "utility.h"
#include "variant.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

namespace
	{
	class FileExtractor:public Filesystem::FileProcessor
		{
		public:
			explicit FileExtractor(ActionCompiler::SpiderDefault& spider
				,const char* src_prefix):
				r_spider(spider),r_src_prefix(src_prefix)
				{
				}

			void operator()(Filesystem& fs,const Filesystem::FileData& data)
				{
				auto pos=strrchr(data.filename,'.');
				if(data.directory || strcmp(pos,".cpp")==0 || strcmp(pos,".h")==0
					|| strcmp(pos,".hpp")==0)
					{
					String extract_to(r_src_prefix);
					extract_to.append(data.filename);
					logWrite(Log::MessageType::INFORMATION
						,"Extracting file to #0;",{extract_to.begin()});
					fs.extract(data.filename,extract_to.begin(),data.directory);
					r_spider.filenameAdd(data.filename + 1,data.directory);
					}
				}
		private:
			SpiderDefault& r_spider;
			const char* r_src_prefix;
		};
	};

static void targetsPatch(std::map<Stringkey,TargetDefault>& targets)
	{
	auto i=targets.begin();
	auto i_end=targets.end();
	while(i!=i_end)
		{
		auto deps=i->second.dependencies();
		auto ptr=deps.begin();
		auto ptr_end=deps.end();
		while(ptr!=ptr_end)
			{
			if(!ptr->externalIs())
				{
				auto x=targets.find(ptr->idGet());
				if(x==i_end)
					{throw ErrorMessage("Action program is incomplete",{});}
				ptr->targetSet(&x->second);
				++(x->second.childrenCount());
				}
			++ptr;
			}
		++i;
		}
	}

static void targetsCreate(Spider& spider
	,const char* name_src,const char* in_dir,bool directory_is)
	{
	if(directory_is)
		{
		logWrite(Log::MessageType::INFORMATION
			,"Adding target for directory #0; (in #1;)",{name_src,in_dir});
		spider.targetAdd(name_src,name_src,TargetDirectory::create())
			.dependencyAdd(Dependency{Stringkey(in_dir)});
		}
	else
		{
		auto loader=spider.loaderGet(name_src);
		if(loader!=nullptr)
			{
			logWrite(Log::MessageType::INFORMATION
				,"Loading targets from #0; (in #1;)",{name_src,in_dir});
			loader->targetsLoad(name_src,in_dir,spider);
			}
		}
	}

SpiderDefault& SpiderDefault::scan(Filesystem& fs)
	{
	fs.filesProcess(nullptr,FileExtractor(*this,sourcePrefixGet()));
	while(m_files.length()!=0)
		{
		auto file_top=std::move(*(m_files.end()-1));
		m_files.truncate();

		auto in_dir=file_top.first;
		in_dir.truncate('/');

		targetsCreate(*this,file_top.first.begin(),in_dir.begin(),file_top.second);
		}

	targetsPatch(r_targets);

	return *this;
	}

SpiderDefault& SpiderDefault::filenameAdd(const char* filename,bool directory_is)
	{
	auto key=Stringkey(filename);
	if(m_files_visited.find(key)==m_files_visited.end())
		{
		m_files_visited.insert(key);
		m_files.append({String(filename),directory_is});
		}
	return *this;
	}

Target& SpiderDefault::targetAdd(const char* name,const char* name_src
	,std::unique_ptr<TargetDetails>&& details)
	{
	auto key=Stringkey(name);
	auto i_key=r_targets.find(key);
	if(i_key!=r_targets.end())
		{
		throw ErrorMessage("Error while processing file #0;: Target has "
			"#1; already been defined in #2;"
			,{name_src,name,i_key->second.nameSourceGet()});
		}

	auto ip=r_targets.emplace(key,TargetDefault(name,name_src,std::move(details)));
	auto& ret=ip.first->second;
	ret.sourcePrefixSet(sourcePrefixGet()).targetPrefixSet(targetPrefixGet());
	return (ip.first)->second;
	}

const TargetLoader* SpiderDefault::loaderGet(const char* filename) const noexcept
	{
	auto i=r_loaders.find(Stringkey(strrchr(filename,'.')));
	if(i==r_loaders.end())
		{return nullptr;}
	return i->second;
	}

SpiderDefault::SpiderDefault(std::map<Stringkey,TargetDefault>& targets
	,const char* target_prefix
	,const std::map<Stringkey,const TargetLoader*>& loaders
	,const char* source_prefix):
	 r_targets(targets),m_target_prefix(target_prefix)
	,r_loaders(loaders),m_src_prefix(source_prefix)
	{
	Utility::mkdir(sourcePrefixGet());
	targets.clear();
	targetAdd("","",TargetDirectory::create());
	}

SpiderDefault::~SpiderDefault()
	{}
