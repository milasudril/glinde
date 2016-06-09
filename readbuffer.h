#ifdef __WAND__
target[name[readbuffer.h] type[include]]
#endif

#ifndef GLINDE_READBUFFER_H
#define GLINDE_READBUFFER_H

#include "datasource.h"
#include "arraysimple.h"

namespace Glinde
	{
	class ReadBuffer
		{
		public:
			explicit ReadBuffer(DataSource& source):r_source(source),m_buffer(4096)
				{
				r_read_pos=m_buffer.begin();
				m_n_valid=0;
				}
			explicit ReadBuffer(DataSource&& source):ReadBuffer(source)
				{}

			uint8_t byteRead()
				{
				if(bufferEnd())
					{fetch();}
				auto ret=*r_read_pos;
				++r_read_pos;
				return ret;
				}

			bool eof() const
				{
				if(bufferEnd())
					{fetch();}
				return bufferEnd();
				}

			const char* nameGet() const noexcept
				{return r_source.nameGet();}

		private:
			DataSource& r_source;
			mutable ArraySimple<uint8_t> m_buffer;
			mutable uint8_t* r_read_pos;
			mutable size_t m_n_valid;

			bool bufferEnd() const noexcept
				{return r_read_pos==m_buffer.begin() + m_n_valid;}

			void fetch() const
				{
				m_n_valid=r_source.read(m_buffer.begin(),m_buffer.length());
				r_read_pos=m_buffer.begin();
				}
		};
	}

#endif
