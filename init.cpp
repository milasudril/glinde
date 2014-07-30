#ifdef __WAND__
target[name[init.o] type[object] dependency[glew32;external] platform[;Windows]]
target[name[init.o] type[object] dependency[GLEW;external]]
#endif

#include "init.h"
#include <herbs/exception/exception.h>
#include <herbs/stringformat/stringformat.h>
#include <herbs/logwriter/logwriter.h>
#include <herbs/logmessage/logmessage.h>

namespace
	{
	class InitException:public Herbs::Exception
		{
		public:
			InitException(GLenum status):m_status(status)
				{}

			void print(Herbs::LogWriter& printer) const
				{
				Herbs::String message;
				auto ptr_message=glewGetErrorString(m_status);
				while(*ptr_message)
					{
					message.append(*ptr_message);
					++ptr_message;
					}

				printer.write(
					{
					Herbs::format(STR("Det gick inte att initiera OpenGL: %0")
						,{message.begin()}).begin()
					,Herbs::LogMessage::Type::ERROR
					}
					);
				}

		private:
			GLenum m_status;
		};

	}

void Angle::initException(GLenum status)
	{
	throw InitException(status);
	}