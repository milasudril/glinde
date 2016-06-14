#ifdef __WAND__
target[name[actioncompiler_targetcxxloader.o] type[object]]
#endif

#include "actioncompiler_targetcxxloader.h"
#include "actioncompiler_targetcxxtargets.h"
#include "actioncompiler_cxxpptokenizer.h"
#include "actioncompiler_cxxcompiler.h"
#include "actioncompiler_dependency.h"
#include "actioncompiler_spider.h"
#include "readbuffer.h"
#include "filein.h"
#include "resourceobject.h"
#include "logwriter.h"
#include "stringkey.h"
#include "debug.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

namespace
	{
	class TagFilter:public DataSource
		{
		public:
			TagFilter(DataSource& source):m_reader(source)
				,m_state(State::NEWLINE)
				{}

			TagFilter(DataSource&& source):TagFilter(source)
				{}

			size_t read(void* buffer,size_t length);

			const char* nameGet() const noexcept
				{return m_reader.nameGet();}

		private:
			ReadBuffer m_reader;
			enum class State:int{NEWLINE,COMMENT_0,JUNK,COMMENT_1,DATA};
			State m_state;
		};
	}

size_t TagFilter::read(void* buffer,size_t length)
	{
	auto buffer_out=reinterpret_cast<uint8_t*>(buffer);
	size_t n_read=0;
	auto state=m_state;
	while(n_read!=length && !m_reader.eof())
		{
		auto ch_in=m_reader.byteRead();
		switch(state)
			{
			case State::NEWLINE:
				if(!(ch_in<=' '))
					{
					switch(ch_in)
						{
						case '/':
							state=State::COMMENT_0;
							break;
						default:
							state=State::JUNK;
						}
					}
				break;

			case State::COMMENT_0:
				switch(ch_in)
					{
					case '/':
						state=State::COMMENT_1;
						break;
					default:
						state=State::JUNK;
					}
				break;

			case State::COMMENT_1:
				switch(ch_in)
					{
					case '@':
						state=State::DATA;
						break;
					default:
						state=State::JUNK;
					}
				break;

			case State::DATA:
				switch(ch_in)
					{
					case '\n':
					case '\r':
						*buffer_out='\n';
						++buffer_out;
						++n_read;
						state=State::NEWLINE;
						break;
					default:
						*buffer_out=ch_in;
						++buffer_out;
						++n_read;
					}
				break;

			case State::JUNK:
				switch(ch_in)
					{
					case '\n':
					case '\r':
						state=State::NEWLINE;
						break;
					}
				break;
			}
		}
	m_state=state;
	return n_read;
	}

namespace
	{
	struct Include
		{
		String name;
		enum class Type:uint8_t{LOCAL,SYSTEM};
		Type type;
		};
	}

static ArrayDynamic<Include> includesGet(DataSource&& src,const char* in_dir)
	{
	ArrayDynamic<Include> ret;
	CxxPPTokenizer tok(src);
	CxxPPTokenizer::Token token;
	enum class Mode:uint8_t{NORMAL,INCLUDE};
	auto mode=Mode::NORMAL;
	while(tok.read(token))
		{
		switch(mode)
			{
			case Mode::NORMAL:
				switch(token.type)
					{
					case CxxPPTokenizer::Token::Type::DIRECTIVE:
						if(token.value=="include")
							{
							mode=Mode::INCLUDE;
							}
					//	TODO: Handle more cases
						break;
					default:
						break;
					}
				break;
			case Mode::INCLUDE:
				switch(token.type)
					{
					case CxxPPTokenizer::Token::Type::STRING:
						{
						auto p=String(in_dir);
						if(p.length()!=0)
							{p.append('/');}
						ret.append(Include
							{
							 std::move(p.append(token.value))
							,Include::Type::LOCAL
							});
						}
						break;
					case CxxPPTokenizer::Token::Type::SYSINCLUDE:
						ret.append(Include{token.value,Include::Type::SYSTEM});
						break;
					default:
						logWrite(Log::MessageType::WARNING,"Illegal preprocessor token %s",token.value.begin());
						break;
					}
				mode=Mode::NORMAL;
				break;
			}
		}
	return ret;
	}

static void includeAdd(Target& target,const Include& include,const char* src_prefix)
	{
	if(include.type==Include::Type::LOCAL)
		{
		target.dependencyAdd(Dependency{Stringkey(include.name.begin())});
		auto filename=String(src_prefix).append('/').append(include.name.begin());
		ResourceObject file_info{TagFilter(FileIn(filename.begin()))};
		if(!file_info.objectExists("dependencies_extra"))
			{return;}
		auto deps_extra=file_info.objectGet("dependencies_extra");
		auto N=deps_extra.objectCountGet();
		auto base=include.name;
		base.truncate('/').append('/');
		for(decltype(N) k=0;k<N;++k)
			{
			auto dep=deps_extra.objectGet(k);
			auto ref=static_cast<const char*>(dep.objectGet("ref"));
			auto rel=dep.stringGet("rel");
			if(rel==nullptr)
				{
				auto path=String(base).append(ref);
				if(strcmp(path.begin(),target.nameGet())!=0)
					{
					GLINDE_DEBUG_PRINT("Got extra dependency %s %s",target.nameGet(),path.begin());
					target.dependencyAdd(Dependency{Stringkey(path.begin())});
					}
				}
			}
		}
	}

static void targetAdd(Spider& spider,const char* name_src,const char* in_dir
	,const ResourceObject& target,const Range<const Include>& includes
	,const CxxCompiler& compiler)
	{
	auto name=String(in_dir);
	if(name.length()!=0)
		{name.append('/');}

	name.append(static_cast<const char*>( target.objectGet("name") ));
	auto type=static_cast<const char*>( target.objectGet("type") );

 	std::unique_ptr<TargetCxx> details;
	if(strcmp(type,"object")==0)
		{details=TargetCxxObject::create(target,compiler);}
	else
	if(strcmp(type,"include")==0)
		{details=TargetCxxInclude::create(target,compiler);}
	else
	if(strcmp(type,"plugin")==0 || strcmp(type,"dynlib")==0  || strcmp(type,"so")==0
		 || strcmp(type,"dll")==0)
		{details=TargetCxxDLL::create(target,compiler);}

	if(details)
		{
		GLINDE_DEBUG_PRINT("Creating target %s",name.begin());
		auto& target=spider.targetAdd(name.begin(),name_src,std::move(details));
		auto ptr=includes.begin();
		auto ptr_end=includes.end();
		while(ptr!=ptr_end)
			{
			includeAdd(target,*ptr,spider.sourcePrefixGet());
			++ptr;
			}
		target.dependencyAdd(Dependency{Stringkey(in_dir)});
		}
	else
		{
		logWrite(Log::MessageType::WARNING
			,"Target %s in %s has an unkown target type %s",name,name_src,type);
		}
	}

void TargetCxxLoader::targetsLoad(const char* name_src,const char* in_dir
	,Spider& spider) const
	{
	auto filename=String(spider.sourcePrefixGet()).append('/').append(name_src);
	ResourceObject file_info{TagFilter(FileIn(filename.begin()))};

	auto includes=includesGet(FileIn(filename.begin()),in_dir);


	auto targets=file_info.objectGet("targets");
	auto N_targets=targets.objectCountGet();

	if(N_targets==0)
		{return;}

	for(decltype(N_targets) k=0;k<N_targets;++k)
		{
		targetAdd(spider,name_src,in_dir,targets.objectGet(k),includes,r_compiler);
		}
	}

TargetCxxLoader& TargetCxxLoader::includeDirAdd(const char* dir_name)
	{
	m_include_dir.append(String(dir_name));
	r_compiler.options().includeDirAdd(dir_name);
	return *this;
	}

