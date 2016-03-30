#ifdef __WAND__
target[name[filesystem.h] type[include]]
#endif

#ifndef GLINDA_FILESYSTEM_H
#define GLINDA_FILESYSTEM_H

#include "datasource.h"
#include <memory>

namespace Glinda
	{
	class Filesystem
		{
		public:
			typedef std::unique_ptr<DataSource,DataSource::Deleter>
			FileReference;

			virtual FileReference fileReferenceCreate(const char* filename)=0;
		};
	}


#endif
