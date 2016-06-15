#ifdef __WAND__
target[name[utility.o] type[object] platform[;GNU/Linux] dependency[uuid;external]]
#endif

#include "utility.h"
#include "errormessage.h"
#include "strerror.h"
#include "variant.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <uuid/uuid.h>
#include <errno.h>

using namespace Glinde;

void Utility::mkdir(const char* dirname)
	{
	if(::mkdir(dirname,0700)!=0)
		{
		char errbuff[256];
		strerror(errno,errbuff,256);
		throw ErrorMessage("It was not possible to create directory #0;. #1;"
			,{dirname,errbuff});
		}
	}


String Utility::mkdtemp(const char* prefix)
	{
	String name("/tmp/");
	name.append(prefix).append('_');
	uuid_t uuid;
	uuid_generate(uuid);
	char uuid_buffer[48];
	uuid_unparse(uuid,uuid_buffer);
	name.append(uuid_buffer);
	mkdir(name.begin());
	return std::move(name);
	}

void Utility::rmdir(const char* dirname)
	{}

bool Utility::exists(const char* filename) noexcept
	{
	struct stat st;
	return stat(filename,&st)==0;
	}

bool Utility::newer(const char* filename_a,const char* filename_b) noexcept
	{
	struct stat st_b;
	if(stat(filename_b,&st_b)!=0)
		{return 1;}

	struct stat st_a;
	if(stat(filename_a,&st_a)!=0)
		{return 0;}

	return st_a.st_mtime >= st_b.st_mtime;
	}
