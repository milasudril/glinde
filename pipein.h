//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"pipein.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"pipein.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_PIPEIN_H
#define GLINDE_PIPEIN_H

#include "datasource.h"
#include "string.h"
#include <memory>
#include <initializer_list>

namespace Glinde
	{
	class PipeIn:public DataSource
		{
		public:
			enum class Stream:int{STDOUT=1,STDERR};

			PipeIn(Stream stream_capture,const char* command
				,std::initializer_list<const char*> args):
				PipeIn(stream_capture,command,Range<const char* const>(args.begin(),args.size()))
				{}

			PipeIn(Stream stream_capture,const char* command
				,const Range<const char* const>& args);

			~PipeIn();

			size_t read(void* buffer,size_t N);

			const char* nameGet() const noexcept
				{return m_name.begin();}

			int exitStatusGet() noexcept;


		private:
			String m_name;
			struct Impl;
			std::unique_ptr<Impl> m_impl;
		};

	}

#endif
