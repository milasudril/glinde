#ifdef __WAND__
target[name[archive.o] type[object] dependency[zip;external]]
#endif

#include "archive.h"
#include "errormessage.h"
#include <zip.h>

using namespace Glinda;

[[noreturn]] static void archiveErrorRaise(const char* message_str
	,const char* filename,int result)
	{
	switch(result)
		{
		case ZIP_ER_EXISTS:
			throw ErrorMessage(message_str,filename,"The file already exists");

		case ZIP_ER_INCONS:
			throw ErrorMessage(message_str,filename,"Bad checksum");

		case ZIP_ER_MEMORY:
			throw ErrorMessage(message_str,filename,"Out of memory");

		case ZIP_ER_NOENT:
			throw ErrorMessage(message_str,filename,"The file does not exist");

		case ZIP_ER_NOZIP:
			throw ErrorMessage(message_str,filename,"The file is not a zip archive");

		case ZIP_ER_OPEN:
			throw ErrorMessage(message_str,filename,"The file could not be opened");

		case ZIP_ER_READ:
			throw ErrorMessage(message_str,filename,"Read error");

		case ZIP_ER_SEEK:
			throw ErrorMessage(message_str,filename,"The file is stored on a non-seeking device");

		case ZIP_ER_COMPNOTSUPP:
			throw ErrorMessage(message_str,filename,"Unsupported compression method");

		case ZIP_ER_ZLIB:
			throw ErrorMessage(message_str,filename,"Failed to initialize zlib");

		default:
			throw ErrorMessage(message_str,filename,"Unknown error");
		}
	}

Archive::Archive(const char* filename)
	{
	int result;
	auto handle=zip_open(filename,ZIP_CHECKCONS,&result);
	if(handle==NULL)
		{
		archiveErrorRaise("It is not possible open the archive \"%s\". %s.",filename,result);
		}
	m_handle=handle;
	}

Archive::~Archive()
	{
	zip_close(static_cast<zip*>(m_handle));
	}

Archive::File::File(Archive& archive,const char* filename)
	{
	auto a=static_cast<zip*>(archive.m_handle);
	auto handle=zip_fopen(a,filename,0);
	if(handle==NULL)
		{
		int status;
		zip_error_get(a,&status,NULL);
		archiveErrorRaise("It is not possible open the file \"%s\". %s.",filename,status);
		}
	m_handle=handle;
	}

Archive::File::~File()
	{
	zip_fclose(static_cast<zip_file*>(m_handle));
	}

size_t Archive::File::read(void* buffer,size_t n_bytes)
	{
	auto ret=zip_fread(static_cast<zip_file*>(m_handle),buffer,n_bytes);

	if(ret < 0)
		{
		throw ErrorMessage("An error occured when reading data.");
		}

	return ret;
	}

