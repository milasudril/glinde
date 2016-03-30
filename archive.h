#ifdef __WAND__
target[name[archive.h] type[include]]
dependency[archive.o]
#endif

#ifndef GLINDA_ARCHIVE_H
#define GLINDA_ARCHIVE_H

#include "datasource.h"
#include "string.h"
#include "filesystem.h"
#include <utility>

namespace Glinda
	{
	class Archive:public Filesystem
		{
		public:
			class File:public DataSource
				{
				public:
					File(const File&)=delete;
					File& operator=(const File&)=delete;

					File(File&& file):m_handle(file.m_handle)
						,m_filename(std::move(file.m_filename))
						{file.m_handle=nullptr;}

					File& operator=(File&& file) noexcept
						{
						std::swap(file.m_handle,m_handle);
						std::swap(file.m_filename,m_filename);
						return *this;
						}


					File(Archive& archive,const char* filename);
					~File();

					size_t read(void* buffer, size_t n_bytes);

					const char* nameGet() const noexcept
						{return m_filename.begin();}

				private:
					void* m_handle;
					String m_filename;
				};

			explicit Archive(const char* filename);
			~Archive();

			Archive(const Archive&)=delete;
			Archive& operator=(const Archive&)=delete;

			File fileGet(const char* filename)
				{
				File ret(*this,filename);
				return std::move(ret);
				}

			FileReference fileReferenceCreate(const char* filename)
				{
				return {new File(*this,filename),fileReferenceDestroy};
				}

			const char* nameGet() const noexcept
				{return m_filename.begin();}

		private:
			void* m_handle;
			String m_filename;

			static void fileReferenceDestroy(DataSource* source)
				{delete source;}
		};
	};

#endif
