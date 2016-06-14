#ifdef __WAND__
target[name[variant.h] type[include]]
#endif

#ifndef GLINDE_VARIANT_H
#define GLINDE_VARIANT_H

#include <limits>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <utility>

namespace Glinde
	{
	template<class T>
	class Range;

	template<class T>
	constexpr T msb()
		{
		return ~(std::numeric_limits<T>::max()>>1);
		}

	class Variant
		{
		public:
			static constexpr uintptr_t POINTER=0|msb<uintptr_t>();
			static constexpr uintptr_t STRING=1|msb<uintptr_t>();
			static constexpr uintptr_t INT=2|msb<uintptr_t>();
			static constexpr uintptr_t UINT=3|msb<uintptr_t>();
			static constexpr uintptr_t CHAR=4|msb<uintptr_t>();
			static constexpr uintptr_t BOOL=5|msb<uintptr_t>();
			static constexpr uintptr_t DOUBLE=6|msb<uintptr_t>();
			static constexpr uintptr_t FLOAT=7|msb<uintptr_t>();

			class Formatter
				{
				public:
					virtual size_t format(const Range<char>& buffer
						,const void* data,const Range<const char>& argstring) const noexcept=0;
				};

			Variant(const void* object,const Formatter& fmt) noexcept
				{
				static_assert(sizeof(&fmt)==sizeof(uintptr_t),"Pointer size incompatible");
				assert( !(reinterpret_cast<uintptr_t>(&fmt) & msb<uintptr_t>()) );
				m_data.x_vptr=object;
				m_type.fmt=&fmt;
				}

			Variant(const void* ptr) noexcept
				{
				m_data.x_vptr=ptr;
				m_type.type=POINTER;
				}

			Variant(const char* str) noexcept
				{
				m_data.x_cptr=str;
				m_type.type=STRING;
				}

			Variant(int64_t x) noexcept
				{
				m_data.x_int64=x;
				m_type.type=INT;
				}

			Variant(uint64_t x) noexcept
				{
				m_data.x_uint64=x;
				m_type.type=UINT;
				}

			Variant(char x) noexcept
				{
				m_data.x_char=x;
				m_type.type=CHAR;
				}

			Variant(bool x) noexcept
				{
				m_data.x_bool=x;
				m_type.type=BOOL;
				}

			Variant(double x) noexcept
				{
				m_data.x_double=x;
				m_type.type=DOUBLE;
				}

			Variant(float x) noexcept
				{
				m_data.x_float=x;
				m_type.type=FLOAT;
				}

			operator std::pair<const void*,const Formatter*>() const noexcept
				{
				assert(!(m_type.type&msb<uintptr_t>()));
				return {m_data.x_vptr,m_type.fmt};
				};

			operator const void*() const noexcept
				{
				assert(m_type.type==POINTER);
				return m_data.x_vptr;
				}

			operator const char*() const noexcept
				{
				assert(m_type.type==STRING);
				return m_data.x_cptr;
				}

			operator int64_t() const noexcept
				{
				assert(m_type.type==INT);
				return m_data.x_int64;
				}

			operator uint64_t() const noexcept
				{
				assert(m_type.type==UINT);
				return m_data.x_uint64;
				}

			operator char() const noexcept
				{
				assert(m_type.type==CHAR);
				return m_data.x_char;
				}

			operator bool() const noexcept
				{
				assert(m_type.type==BOOL);
				return m_data.x_bool;
				}

			operator double() const noexcept
				{
				assert(m_type.type==DOUBLE);
				return m_data.x_double;
				}

			operator float() const noexcept
				{
				assert(m_type.type==FLOAT);
				return m_data.x_float;
				}

			uintptr_t typeGet() const noexcept
				{
				return m_type.type;
				}


		private:
			union
				{
				const void* x_vptr;
				const char* x_cptr;
				int64_t x_int64;
				uint64_t x_uint64;
				char x_char;
				bool x_bool;
				double x_double;
				float x_float;
				} m_data;
			union
				{
				uintptr_t type;
				const Formatter* fmt;
				} m_type;
		};
	}

#endif
