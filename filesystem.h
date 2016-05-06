#ifdef __WAND__
target[name[filesystem.h] type[include]]
#endif

/**\file filesystem.h \brief Definition of an abstract fil system
 */

#ifndef GLINIE_FILESYSTEM_H
#define GLINIE_FILESYSTEM_H

#include "datasource.h"
#include "string.h"
#include <memory>

namespace Glinie
	{
	/**\brief An abstract filesystem
	 *
	 * A Filesystem is an object that handles files. A file can be fetched by
	 * calling fileReferenceCreate(const char*), which looks for a file within
	 * the filesystem and returns a FileReference on success.
	 *
	 */
	class Filesystem
		{
		public:
			/**\brief A type holding a reference to a file.
			 *
			 * This type manages the lifetime of a DataSource object or file,
			 * retrieved by the method fileReferenceCreate(const char*).
			 *
			*/
			class FileReference:private std::unique_ptr<DataSource,DataSource::Deleter>
				{
				public:
					explicit FileReference(DataSource* ds,DataSource::Deleter d):
						std::unique_ptr<DataSource,DataSource::Deleter>(ds,d)
						{}

					operator DataSource& () noexcept
						{return *get();}
				};


			/**\brief File access.
			 *
			 * This method shall return a FileReference representing the file
			 * with the name `filename`. If no such file exists, it shall throw
			 * an exception.
			*/
			virtual FileReference fileReferenceCreate(const char* filename)=0;

			/**\brief Change working directory.
			 *
			 * This method shall change the current working directory.
			 *
			 */
			virtual void cd(const char* dir_new)=0;

			/**\brief Construct a path for a sibling file.
			 */
			virtual String filenameFromSibling(const char* sibling,const char* filename)=0;
		};
	}


#endif
