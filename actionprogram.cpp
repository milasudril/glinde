//@	{
//@	    "targets":[
//@	        {
//@	            "dependencies":[{"ref":"maike","rel":"external"}],
//@	            "name":"actionprogram.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}

#include "actionprogram.h"
#include "sdkresource.h"
#include "utility.h"
#include "fileout.h"
#include "resourceobject.h"
#include "memoryreader.h"
#include "blob.h"
#include "filesystem.h"
#include "logwriter.h"
#include <maike/maike.hpp>

//TODO: We should use the configuration used when compiling maike.
//      => Add python script
GLINDE_BLOB(maikeconfig,"maikeconfig.json");

using namespace Glinde;

static void sdkCopy(const Range<const SdkResource>& sdk,const char* path)
	{
	auto ptr=sdk.begin();
	auto ptr_end=sdk.end();
	Utility::mkdir(path);
	auto path_base=String(path).append('/');
	while(ptr!=ptr_end)
		{
		FileOut file_out(String(path_base).append(ptr->name).begin());

		file_out.write(ptr->data.begin(),ptr->data.length());
		++ptr;
		}
	}

namespace 
	{
	class FileExtractor:public Filesystem::FileProcessor
		{
		public:
			FileExtractor(const char* dir_target):r_dir_target(dir_target)
				{}

			void operator()(Filesystem& fs,const Filesystem::FileData& data)
				{
				logWrite(Log::MessageType::INFORMATION,"Extracting #0;"
					,{data.filename});
			//TODO: Check if data.filename is a program source file
				fs.extract(data.filename
					,String(r_dir_target).append(data.filename).begin()
					,data.directory);
				}
			
		private:
			const char* r_dir_target;
		};
	}

namespace 
	{
	class MaikeConfigurator:public Maike::DataSource
		{
		public:
			MaikeConfigurator(const uint8_t* mem_begin
				,const uint8_t* mem_end):m_mem_reader(mem_begin,mem_end)
				{}

			size_t read(void* data,size_t length)
				{return m_mem_reader.read(data,length);}

			const char* nameGet() const noexcept
				{return m_mem_reader.nameGet();}

		private:
			void destroy(){}
			MemoryReader m_mem_reader;
		};
	}

static void maikeException(const Maike::ErrorMessage& msg)
	{
	throw ErrorMessage("maike: #0;",{msg.messageGet()});
	}

static void maikeConfig(Maike::Session& maike,const char* dirtemp)
	{
	Maike::rootSet(maike,dirtemp);
	
		{
		MaikeConfigurator config(maikeconfig_begin,maikeconfig_end);
		Maike::configAppend(maike,config);
		}

		{
		auto maikeconfig=Maike::configDump(maike);
		auto target_hooks=maikeconfig.objectGet("target_hooks");
		auto N=target_hooks.objectCountGet();
		for(decltype(N) k=0;k<N;++k)
			{
			auto hook=target_hooks.objectGet(k);
			auto plugin=static_cast<const char*>(hook.objectGet("plugin"));
			if(strcmp(plugin,"targetcxx")==0)
				{
				auto config=hook.objectGet("config");
				auto includedir_noscan=config.objectGet("includedir_noscan");
				includedir_noscan.objectAppend(includedir_noscan.create(dirtemp));
				}
			}
		Maike::configClear(maike);
		Maike::sysvarsLoad(maike);
		Maike::configAppend(maike,maikeconfig);
		}
	}

static Plugin pluginCreate(Filesystem& source,const char* name
	,const Range<const SdkResource>& sdk,const char* dirtemp)
	{
	auto src_prefix=String(dirtemp).append("/src");
	auto sdk_prefix=String(dirtemp).append("/glinde");

//	Maike::versionPrint();
	Maike::init(maikeException);
	auto maike=Maike::sessionCreate();
	maikeConfig(*maike,dirtemp);

	source.filesProcess("/",FileExtractor(dirtemp));
	sdkCopy(sdk,sdk_prefix.begin());
	Maike::targetCompile(*maike,name);

	auto target_prefix=Maike::targetDirectoryGet(*maike);
	return Plugin(String(target_prefix).append('/').append(name).begin());
	}

ActionProgram::ActionProgram(Filesystem& source,const char* name
	,const Range<const SdkResource>& sdk):m_dirtemp(Utility::mkdtemp("glinde"))
	,m_plug(pluginCreate(source,name,sdk,m_dirtemp.begin()))
	{
	}
