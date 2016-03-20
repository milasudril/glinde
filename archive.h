#ifdef __WAND__
target[name[archive.h] type[include]]
dependency[archive.o]
#endif

#ifndef GLINDA_ARCHIVE_H
#define GLINDA_ARCHIVE_H

#include "datasource.h"

namespace Glinda
	{
	class Archive
		{
		public:
			class File:public DataSource
				{
				public:
					File(Archive& archive,const char* filename);
					~File();

					size_t read(void* buffer, size_t n_bytes);

				private:
					void* m_handle;
				};

			Archive(const char* filename);
			~Archive();

			Archive(const Archive&)=delete;
			Archive& operator=(const Archive&)=delete;

		private:
			void* m_handle;
		};
	};

#endif
