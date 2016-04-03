#ifdef __WAND__
target[name[filesystem.h] type[include]]
#endif

/**\file filesystem.h \brief Definition of an abstract fil system
 */

#ifndef GLINDA_FILESYSTEM_H
#define GLINDA_FILESYSTEM_H

#include "datasource.h"
#include <memory>

namespace Glinda
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
			typedef std::unique_ptr<DataSource,DataSource::Deleter>
			FileReference;

			/**\brief File access.
			 *
			 * This method shall return a FileReference representing the file
			 * with the name `filename`. If no such file exists, it shall throw
			 * an exception.
			*/
			virtual FileReference fileReferenceCreate(const char* filename)=0;
		};
	}


#endif
