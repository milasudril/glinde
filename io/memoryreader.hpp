//@	{
//@	 "dependencies_extra":[{"ref":"memoryreader.o","rel":"implementation"}]
//@	,"targets":[{"name":"memoryreader.hpp","type":"include"}]
//@	}

#ifndef GLINDE_MEMORYREADER_HPP
#define GLINDE_MEMORYREADER_HPP

#include "datasource.hpp"
#include "../vectortype.hpp"

#include <cstdint>

namespace Glinde
	{
	class MemoryReader:public DataSource
		{
		public:
			MemoryReader(const uint8_t* begin,const uint8_t* end)
				{m_content.fields={begin,static_cast<size_t>(end-begin)};}

			~MemoryReader()=default;

			const char* nameGet() const noexcept
				{return "internal buffer";}

			size_t read(void* buffer,size_t N);

		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			typedef vec2_t<int64_t> TwoPointers;
#else
			typedef vec2_t<int32_t> TwoPointers;
#endif
			union
				{
				TwoPointers x;
				struct
					{
					const uint8_t* data;
					size_t N;
					} fields;
				} m_content;
		};
	}

#endif
