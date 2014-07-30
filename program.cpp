#ifdef __WAND__
target[name[program.o] type[object] platform[;Windows] dependency[glew32;external]]
target[name[program.o] type[object] dependency[GLEW;external]]
#endif

#include "program.h"
#include <herbs/exception/exception.h>
#include <herbs/stringformat/stringformat.h>
#include <herbs/logwriter/logwriter.h>
#include <herbs/logmessage/logmessage.h>

namespace
	{
	class ProgramCompileException:public Herbs::Exception
		{
		public:
			ProgramCompileException(const Herbs::Stringbase<char>& message)
				{
			//	Assume that the GLSL compiler returns a pure ASCII string
				auto ptr=message.begin();
				while(ptr!=message.end())
					{
					m_message.append(*ptr);
					++ptr;
					}
				}

			void print(Herbs::LogWriter& printer) const
				{
				printer.write(
					{
					Herbs::format(STR("Det gick inte att länka grafikprogrammet: %0")
						,{m_message.begin()}).begin()
					,Herbs::LogMessage::Type::ERROR
					}
					);
				}

		private:
			Herbs::String m_message;
		};

	}

void Angle::Program::linkException()
	{
	int length_log=0;
	glGetProgramiv(handle,GL_INFO_LOG_LENGTH,&length_log);

	Herbs::Stringbase<char> message;
	message.lengthValidSet(length_log);
	glGetProgramInfoLog(handle, length_log, NULL,message.begin());

	throw ProgramCompileException(message);
	}

