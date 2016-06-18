#ifdef __WAND__
target[name[message.h] type[include]]
dependency[message.o]
#endif

#ifndef GLINDE_MESSAGE_H
#define GLINDE_MESSAGE_H

#include "vectortype.h"
#include "arrayfixed.h"
#include <utility>
#include <type_traits>
#include <cstdint>
#include <cassert>

namespace Glinde
	{
	constexpr uint64_t makeTime(uint64_t time,uint16_t seq)
		{return ( time<<static_cast<uint64_t>(8*sizeof(seq)) )|seq;}

	class Message
		{
		public:
		//	TODO replace function pointer by a pointer to an
			class alignas(16) Processor
				{
				public:
					virtual void operator()(const Message& data)=0;
				};
			static __attribute__((aligned(16)))  void no_op(const Message& data);

			typedef void ( __attribute__((aligned(16))) *Handler)(const Message& msg);

			Message()
				{
				m_content.x=vec4_t<int64_t>{0,0,0,0};
				}

			template<class T>
			explicit Message(uint64_t time,uint16_t seq,Handler handler,const Range<const T>& data
				,typename std::enable_if< std::is_trivially_copyable<T>::value >::type* dummy=nullptr)
				{
				m_content.data={makeTime(time,seq),mangle(handler,OBJECT_OWNED),nullptr,0};
				auto src=data.begin();
				auto l= data.length()*sizeof(T);
				copyAlloc(src,l);
				}

			Message(const Message& msg)
				{
				m_content.x=msg.m_content.x;
				if(m_content.data.m_handler & OBJECT_OWNED)
					{copyAlloc(msg.m_content.data.r_object,m_content.data.m_content.sizes[0]);}
				}

			template<class T,size_t N>
			explicit Message(uint64_t time,uint16_t seq,Handler handler,void* object
				,const ArrayFixed<T,N>& data)
				{
				static_assert(N<=Content::Data::length<T>()
					,"Array too large for the message");
				m_content.data={makeTime(time,seq),mangle(handler,0),object,0};
				auto r=static_cast< Range<T> >( m_content.data );
				for(unsigned int k=0;k<N;++k)
					{r[k]=data[k];}
				}

			Message(Message&& msg) noexcept
				{
				assert(&msg!=this);
				m_content.x=msg.m_content.x;
				msg.m_content.x=vec4_t<int64_t>{0,0,0,0};
				}

			~Message()
				{dataFree();}

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

			uint64_t timeGet() const noexcept
				{return m_content.data.m_time>>sizeof(uint16_t);}

			bool operator<(const Message& msg) const noexcept
				{
 				if( m_content.data.m_time>msg.m_content.data.m_time )
					{return 1;}
				return 0;
				}

			void clear() noexcept
				{
				dataFree();
				m_content.data.m_handler=mangle(no_op,0);
				}

			void process() const
				{
				handlerGet(m_content.data.m_handler)(*this);
				}

			template<class T>
			Range<const T> paramsGet() const noexcept
				{return static_cast< Range<const T> >(m_content);}

			void* objectGet() const noexcept
				{return m_content.data.r_object;}

			template<class T>
			Range<const T> dataGet() const noexcept
				{
				return Range<const T>{reinterpret_cast<const T*>(objectGet())
					,m_content.data.m_content.sizes[0]/sizeof(T)};
				}

			uint16_t seqGet() const noexcept
				{return static_cast<uint16_t>(m_content.data.m_time&0xffff);}

		private:
			void copyAlloc(const void* src,size_t length);
			void dataFree() noexcept;

			static constexpr uint8_t OBJECT_OWNED=0x1;

			union Content
				{
				vec4_t<int64_t> x;
				struct Data
					{
					uint64_t m_time;
					uintptr_t m_handler;
					void* r_object;
					union Content§
						{
					#if __x86_64__ || __amd64 || __amd64__ || __x86_64 || _M_X64 \
						|| _M_AMD64
						uint8_t bytes[8];
						uint16_t words[4];
						uint32_t dwords[2];
						uint64_t qwords[1];
						size_t sizes[1];
					#else
						uint8_t bytes[16];
						uint16_t words[8];
						uint32_t dwords[4];
						uint64_t qwords[2];
						size_t sizes[4];
					#endif
						} m_content;

					template<class T>
					static constexpr size_t length() noexcept;

					template<class T>
					operator Range<T>() noexcept;

					template<class T>
					operator Range<const T>() const noexcept;
					} data;
				} m_content;

			static inline uintptr_t mangle(Handler handler,uint8_t val) noexcept
				{
				assert((reinterpret_cast<uintptr_t>(handler)&0xf)==0);
				return reinterpret_cast<uintptr_t>(handler)|val;
				}

			static inline Handler handlerGet(uintptr_t x) noexcept
				{
				return reinterpret_cast<Handler>(x&(~0xf) );
				}
		};


	template<>
	constexpr size_t Message::Content::Data::length<uint8_t>() noexcept
		{return sizeof(Content::bytes)/sizeof(uint8_t);}

	template<>
	constexpr size_t Message::Content::Data::length<uint16_t>() noexcept
		{return sizeof(Content::words)/sizeof(uint16_t);}

	template<>
	constexpr size_t Message::Content::Data::length<uint32_t>() noexcept
		{return sizeof(Content::dwords)/sizeof(uint32_t);}

	template<>
	constexpr size_t Message::Content::Data::length<uint64_t>() noexcept
		{return sizeof(Content::qwords)/sizeof(uint64_t);}


	template<>
	Message::Content::Data::operator Range<uint8_t>() noexcept
		{return Range<uint8_t>{m_content.bytes,length<uint8_t>()};}

	template<>
	Message::Content::Data::operator Range<uint16_t>() noexcept
		{return Range<uint16_t>{m_content.words,length<uint16_t>()};}

	template<>
	Message::Content::Data::operator Range<uint32_t>() noexcept
		{return Range<uint32_t>{m_content.dwords,length<uint32_t>()};}

	template<>
	Message::Content::Data::operator Range<uint64_t>() noexcept
		{return Range<uint64_t>{m_content.qwords,length<uint64_t>()};}


	template<>
	Message::Content::Data::operator Range<const uint8_t>() const noexcept
		{return Range<const uint8_t>{m_content.bytes,length<uint8_t>()};}

	template<>
	Message::Content::Data::operator Range<const uint16_t>() const noexcept
		{return Range<const uint16_t>{m_content.words,length<uint16_t>()};}

	template<>
	Message::Content::Data::operator Range<const uint32_t>() const noexcept
		{return Range<const uint32_t>{m_content.dwords,length<uint32_t>()};}

	template<>
	Message::Content::Data::operator Range<const uint64_t>() const noexcept
		{return Range<const uint64_t>{m_content.qwords,length<uint64_t>()};}
	}

#endif
