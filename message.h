#ifdef __WAND__
target[name[message.h] type[include]]
dependency[message.o]
#endif

#ifndef GLINDE_MESSAGE_H
#define GLINDE_MESSAGE_H

#include "vectortype.h"
#include "range.h"
#include <utility>
#include <type_traits>
#include <cstdint>
#include <cassert>

namespace Glinde
	{
	class Message
		{
		public:
			Message()
				{
				m_content.x=vec4_t<int64_t>{0,0,0,0};
				}

			template<class T>
			explicit Message(uint32_t seq,uint32_t type,uint64_t time,const Range<const T>& data
				,typename std::enable_if< std::is_trivially_copyable<T>::value >::type* dummy=nullptr)
				{
				m_content.data={seq,type,time,0,nullptr};
				auto src=data.begin();
				auto l=static_cast<size_t>( data.length()*sizeof(T) );
				copyAlloc(src,l);
				}

			explicit Message(uint32_t seq,uint32_t type,uint64_t time)
				{
				m_content.data={seq,type,time,0,0};
				}

			template<class T>
			explicit Message(uint32_t seq,uint32_t type,uint64_t time,const T& data)
				{
				static_assert(sizeof(m_content.data.length)>=sizeof(T),"Length field is too small for storing data");
				m_content.data={seq,type,time,uint64_t(data),nullptr};
				}

			Message(Message&& msg) noexcept
				{
				assert(&msg!=this);
				m_content.x=msg.m_content.x;
				msg.m_content.x=vec4_t<int64_t>{0,0,0,0};
				}

			Message(const Message& msg)
				{
				m_content.x=msg.m_content.x;
				copyAlloc(msg.m_content.data.data
					,static_cast<size_t>(msg.m_content.data.length));
				}

			~Message();

			Message& operator=(Message&& msg) noexcept
				{
				std::swap(msg.m_content.x,m_content.x);
				return *this;
				}

			Message& operator=(const Message& msg)
				{
				assert(&msg!=this);
				Message copy(msg);
				*this=std::move(copy);
				return *this;
				}

			template<class T>
			Range<const T> dataGetRange() const noexcept
				{
				auto l=m_content.data.length;
				auto ptr=reinterpret_cast<const T*>(m_content.data.data);
				return Range<const T>(ptr,l/sizeof(T));
				}

			template<class T>
			T dataGetValue() const noexcept
				{
				return static_cast<T>(m_content.data.length);
				}

			template<class T>
			T* dataGetPointer() const noexcept
				{
				return reinterpret_cast<T*>(m_content.data.length);
				}

			uint32_t typeGet() const noexcept
				{return m_content.data.type;}

			uint64_t timeGet() const noexcept
				{return m_content.data.time;}

			bool operator<(const Message& msg) const noexcept
				{
 				if( timeGet()>msg.timeGet()
					|| (timeGet()==msg.timeGet() && m_content.data.seq > msg.m_content.data.seq))
					{return 1;}
				return 0;
				}

			void clear()
				{
				m_content.data.type=0;
				}

		private:
			void copyAlloc(const void* src,size_t length);
			union
				{
				vec4_t<int64_t> x;
				struct
					{
					uint32_t seq;
					uint32_t type;
					uint64_t time;
					uint64_t length;
					void* data;
					} data;
				} m_content;
		};
	}

#endif
