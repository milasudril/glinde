//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"actioncompiler_cxxpptokenizer.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_cxxpptokenizer.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ACTIONCOMPILER_CXXPPTOKENIZER_H
#define GLINDE_ACTIONCOMPILER_CXXPPTOKENIZER_H

#include "string.h"
#include "readbuffer.h"

namespace Glinde
	{
	namespace ActionCompiler
		{
		class CxxPPTokenizer
			{
			public:
				struct Token
					{
					Token():type(Type::OTHER){}
					String value;

					enum class Type:int{OTHER,DIRECTIVE,STRING,SYSINCLUDE};

					Type type;
					};

				CxxPPTokenizer(DataSource& source):
					m_reader(source),m_state(State::NEWLINE)
					{}

				bool read(Token& token);

			private:
				ReadBuffer m_reader;
				enum class State:int
					{
					 NEWLINE
					,COMMENT_0
					,COMMENT_1
					,COMMENT_2
					,COMMENT_3
					,PREPROCESSOR_BEGIN
					,PREPROCESSOR
					,PREPROCESSOR_NEXT
					,STRING
					,SYSINCLUDE
					,JUNK
					};
				State m_state;
			};
		}
	}

#endif
