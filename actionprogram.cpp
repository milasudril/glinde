#ifdef __WAND__
target[name[actionprogram.o]type[object]]
#endif

#include "actionprogram.h"
#include "actioncompiler_main.h"
#include "actioncompiler_cxxoptions.h"
#include "sdkresource.h"
#include "utility.h"
#include "fileout.h"
#include "resourceobject.h"
#include "memoryreader.h"
#include "blob.h"

GLINDE_BLOB(cxxoptions,"config.json");

using namespace Glinde;
using namespace Glinde::ActionCompiler;

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

static Plugin pluginCreate(Filesystem& source,const char* name
	,const Range<const SdkResource>& sdk,const char* dirtemp)
	{
	auto src_prefix=String(dirtemp).append("/src");
	auto target_prefix=String(dirtemp).append("/obj");
	auto sdk_prefix=String(dirtemp).append("/glinde");

	sdkCopy(sdk,sdk_prefix.begin());

	CxxOptions options
		{ResourceObject(MemoryReader(cxxoptions_begin,cxxoptions_end))};
	options.includeDirAdd(dirtemp).dllSupportEnable();

	programCompile(source,src_prefix.begin(),target_prefix.begin(),options);

	return Plugin(target_prefix.append('/').append(name).begin());
	}

ActionProgram::ActionProgram(Filesystem& source,const char* name
	,const Range<const SdkResource>& sdk):m_dirtemp(Utility::mkdtemp("glinde"))
	,m_plug(pluginCreate(source,name,sdk,m_dirtemp.begin()))
	{
	}
