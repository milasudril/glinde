//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"filein.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"filein.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_FILEIN_H
#define GLINDE_FILEIN_H

#include "datasource.h"
#include "string.h"

namespace Glinde
	{
	class FileIn:public DataSource
		{
		public:
			explicit FileIn(const char* filename);
			~FileIn();

			size_t read(void* buffer,size_t count);

			const char* nameGet() const noexcept
				{return m_name.begin();}

		private:
			String m_name;
			intptr_t m_handle;
		};
	}

#endif
