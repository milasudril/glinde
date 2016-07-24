//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"fileout.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"fileout.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
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
