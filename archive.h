#ifdef __WAND__
target[name[archive.h] type[include]]
dependency[archive.o]
#endif

/**\file archive.h \brief Definition of a ZIP archive reader class
 */

#ifndef GLINDE_ARCHIVE_H
#define GLINDE_ARCHIVE_H

#include "datasource.h"
#include "string.h"
#include "filesystem.h"
#include <utility>

namespace Glinde
	{
	/**\brief A class describing a ZIP archive reader
	 *
	 * Glinde uses ZIP archives as data storage. To access such files, this
	 * class is used.
	 *
	 * \note This class is implemented using the `libzip` library by Dieter
	 * Baron and Thomas Klausner, released under the "New BSD License".
	 *
	*/
	class Archive:public Filesystem
		{
		public:
			/**\brief A representation of a file stored in a ZIP archive
			*/
			class File:public DataSource
				{
				public:
					File(const File&)=delete;
					File& operator=(const File&)=delete;

					/**\brief Move constructor
					*/
					File(File&& file):m_handle(file.m_handle)
						,m_filename(std::move(file.m_filename))
						{file.m_handle=nullptr;}

					/**\brief Move assignment operator
					*/
					File& operator=(File&& file) noexcept
						{
						std::swap(file.m_handle,m_handle);
						std::swap(file.m_filename,m_filename);
						return *this;
						}

					/**\brief Constructor initializing a new File.
					 *
					 * This constructor initializes the File, by looking for
					 * `filename` in `archive`. If there is no such file in the
					 * archive, an exception is thrown.
					 *
					 * \warning The object refered to by `archive` must survive
					 * at least during the lifetime of the File object.
					 *
					*/
					File(Archive& archive,const char* filename);
					~File();

					size_t read(void* buffer, size_t n_bytes);

					const char* nameGet() const noexcept
						{return m_filename.begin();}

				private:
					void* m_handle;
					String m_filename;
				};

			/**\brief Archive initization.
			 *
			 * This constructor inititializes an Archive object so it reads data
			 * from the file with name `filename`. If the file does not exist,
			 * an exception is thrown.
			 *
			*/
			explicit Archive(const char* filename);
			~Archive();

			Archive(const Archive&)=delete;
			Archive& operator=(const Archive&)=delete;

			/**\brief File access
			 *
			 * This function retrieves a File from the Archive.
			 *
			 * \warning The lifetime of the Archive must exceed the lifetime
			 * of the returned object.
			 */
			File fileGet(const char* filename)
				{
				File ret(*this,filename);
				return std::move(ret);
				}

			/**\brief File access
			 *
			 * This function is a polymorphic version of fileGet(const char*).
			 *
			 * \see fileGet
			 *
			*/
			FileReference fileReferenceCreate(const char* filename)
				{
				return FileReference{new File(*this,filename),fileReferenceDestroy};
				}

			/**\brief Archive filename
			 *
			 * This function returns a pointer to the name of the Archive.
			*/
			const char* nameGet() const noexcept
				{return m_filename.begin();}

			String filepathCreate(const char* wd,const char* path_relative);

			void cd(const char* dir_new);

			String filenameFromSibling(const char* sibling,const char* filename);

		private:
			void* m_handle;
			String m_filename;
			String m_dir_current;

			static void fileReferenceDestroy(DataSource* source)
				{delete source;}
		};
	};

#endif
