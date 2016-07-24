//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[
//@	                {
//@	                    "ref":"zip",
//@	                    "rel":"external"
//@	                }
//@	            ],
//@	            "name":"archive.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "archive.h"
#include "errormessage.h"
#include "utility.h"
#include "debug.h"
#include "fileout.h"
#include "variant.h"
#include <zip.h>
#include <cstring>

using namespace Glinde;

[[noreturn]] static void archiveErrorRaise(const char* message_str
	,const char* filename,int result)
	{
	switch(result)
		{
		case ZIP_ER_EXISTS:
			throw ErrorMessage(message_str,{filename,"The file already exists"});

		case ZIP_ER_INCONS:
			throw ErrorMessage(message_str,{filename,"Bad checksum"});

		case ZIP_ER_MEMORY:
			throw ErrorMessage(message_str,{filename,"Out of memory"});

		case ZIP_ER_NOENT:
			throw ErrorMessage(message_str,{filename,"The file does not exist"});

		case ZIP_ER_NOZIP:
			throw ErrorMessage(message_str,{filename,"The file is not a zip archive"});

		case ZIP_ER_OPEN:
			throw ErrorMessage(message_str,{filename,"The file could not be opened"});

		case ZIP_ER_READ:
			throw ErrorMessage(message_str,{filename,"Read error"});

		case ZIP_ER_SEEK:
			throw ErrorMessage(message_str,{filename,"The file is stored on a non-seeking device"});

		case ZIP_ER_COMPNOTSUPP:
			throw ErrorMessage(message_str,{filename,"Unsupported compression method"});

		case ZIP_ER_ZLIB:
			throw ErrorMessage(message_str,{filename,"Failed to initialize zlib"});

		default:
			throw ErrorMessage(message_str,{filename,"Unknown error"});
		}
	}

static String directoryNormalize(const char* str)
	{
	String ret;
	ret.append('/');
	String elem_current;
	while(*str!='\0')
		{
		switch(*str)
			{
			case '/':
				if(elem_current=="..")
					{
					ret.truncate().truncate('/').append('/');
					}
				else
				if(elem_current!="." && elem_current.length()!=0)
					{ret.append(elem_current).append('/');}

				elem_current.clear();
				break;

			default:
				elem_current.append(*str);
			}
		++str;
		}
	ret.append(elem_current);
	return std::move(  ret );
	}

Archive::Archive(const char* filename)
	{
	int result;
	auto handle=zip_open(filename,ZIP_CHECKCONS,&result);
	if(handle==NULL)
		{
		archiveErrorRaise("It is not possible open the archive \"#0;\". #1;.",filename,result);
		}
	m_handle=handle;

	m_filename=String(filename);
	m_dir_current.append('/');
	}

Archive::~Archive()
	{
	if(m_handle!=NULL)
		{zip_close(static_cast<zip*>(m_handle));}
	}

static const char* prefixStrip(const char* filename)
	{
	auto ptr=filename;
	while(*ptr!='\0')
		{
		if(*ptr==':')
			{return ptr + 1;}
		++ptr;
		}
	return filename;
	}

Archive::File::File(Archive& archive,const char* filename)
	{
	auto a=static_cast<zip*>(archive.m_handle);
	auto filename_temp=archive.m_dir_current;
	filename_temp=directoryNormalize(filename_temp.append(prefixStrip(filename)).begin());

	auto handle=zip_fopen(a,filename_temp.begin() + 1,0);
	if(handle==NULL)
		{
		int status;
		zip_error_get(a,&status,NULL);
		archiveErrorRaise("It is not possible open the file \"#0;\". #1;."
			,filename_temp.begin(),status);
		}
	m_filename=String(archive.m_filename).append(':').append(filename_temp);
	m_handle=handle;
	}

Archive::File::~File()
	{
	if(m_handle!=NULL)
		{zip_fclose(static_cast<zip_file*>(m_handle));}
	}

size_t Archive::File::read(void* buffer,size_t n_bytes)
	{
	auto ret=zip_fread(static_cast<zip_file*>(m_handle),buffer,n_bytes);

	if(ret < 0)
		{
		throw ErrorMessage("An error occured when reading data.",{});
		}

	return static_cast<size_t>(ret);
	}

void Archive::cd(const char* dir_new)
	{
	m_dir_current.append('/').append(dir_new);
	m_dir_current=directoryNormalize(m_dir_current.begin());
	}

String Archive::filenameFromSibling(const char* sibling,const char* filename)
	{
	String ret(sibling);
	return std::move(
		directoryNormalize( ret.truncate('/').append('/').append(filename).begin())
	);
	}

void Archive::filesProcess(const char* root,FileProcessor&& process)
	{
	auto N=zip_get_num_entries(static_cast<zip_t*>(m_handle),0);
	struct zip_stat sb;
	for(decltype(N) k=0;k<N;++k)
		{
		zip_stat_index(static_cast<zip_t*>(m_handle),k,0,&sb);
		auto dir=directoryNormalize(sb.name);
		FileData fd{nullptr,nullptr,0};
		if(*(dir.end() - 1)=='/')
			{
			fd.directory=1;
			dir.truncate();
			}
		fd.filename=dir.begin();
		process(*this,fd);
		}
	}

void Archive::extract(const char* filename_src,const char* file_dest,bool directory)
	{
	if(directory)
		{
		Utility::mkdir(file_dest);
		}
	else
		{
		FileOut dest(file_dest);
		File source(*this,filename_src);

		char buffer[4096];
		size_t n;
		do
			{
			n=source.read(buffer,4096);
			dest.write(buffer,n);
			}
		while(n==4096);
		}
	}
