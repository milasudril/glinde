#ifdef __WAND__
target[name[fileout.h] type[include]]
dependency[fileout.o]
#endif

#ifndef GLINDE_FILEOUT_H
#define GLINDE_FILEOUT_H
#include <cstdint>
#include <cstddef>
namespace Glinde
	{
	class FileOut
		{
		public:
			explicit FileOut(const char* filename);
			~FileOut();

			size_t write(const void* buffer,size_t count);

		private:
			intptr_t m_handle;
		};
	}

#endif
