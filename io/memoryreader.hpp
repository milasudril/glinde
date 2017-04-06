//@	{
//@	 "dependencies_extra":[{"ref":"memoryreader.o","rel":"implementation"}]
//@	,"targets":[{"name":"memoryreader.hpp","type":"include"}]
//@	}

#ifndef GLINDE_MEMORYREADER_HPP
#define GLINDE_MEMORYREADER_HPP

#include "datasource.hpp"
#include "../storage/vectortype.hpp"

#include <cstdint>

namespace Glinde
	{
	class MemoryReader:public DataSource
		{
		public:
			explicit MemoryReader(const uint8_t* begin,const uint8_t* end)
				{m_content.fields={begin,static_cast<size_t>(end-begin)};}

			~MemoryReader()=default;

			const char* nameGet() const noexcept
				{return "internal buffer";}

			size_t read(void* buffer,size_t N);

		private:
			union
				{
				vec2_t<intptr_t> x;
				struct
					{
					const uint8_t* data;
					size_t N;
					} fields;
				} m_content;
		};
	}

#endif
