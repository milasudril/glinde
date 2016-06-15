#ifdef __WAND__
target[name[fileout.o] type[object] platform[;GNU/Linux]]
#endif

#include "fileout.h"
#include "errormessage.h"
#include "strerror.h"
#include "variant.h"

#define _FILE_OFFSET_BITS 64

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

using namespace Glinde;

FileOut::FileOut(const char* filename)
	{
	static_assert(sizeof(m_handle)>=sizeof(int),"Handle is too small");
	m_handle=open(filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	if(m_handle==-1)
		{
		char errbuff[256];
		strerror(errno,errbuff,256);
		throw ErrorMessage("It was not possible to create the file #0;. #1;"
			,{filename,errbuff});
		}
	}

FileOut::~FileOut()
	{
	fsync(static_cast<int>(m_handle));
	close(static_cast<int>(m_handle));
	}

size_t FileOut::write(const void* buffer,size_t count)
	{
	auto n=::write(static_cast<int>(m_handle),buffer,count);
	if(n==-1)
		{throw ErrorMessage("I/O error",{});}
	return n;
	}
