#ifdef __WAND__
target[name[datasource.h] type[include]]
#endif

#ifndef GLINDA_DATASOURCE_H
#define GLINDA_DATASOURCE_H

#include <cstddef>

namespace Glinda
	{
	class DataSource
		{
		public:
			virtual size_t read(void* buffer, size_t length)=0;
		};
	}

#endif
