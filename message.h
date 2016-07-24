//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"message.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"message.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
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
			class alignas(16) Processor
				{
				public:
					virtual void operator()(const Message& data){}
				};

			Message()
				{
				m_msg.x=vec4_t<int64_t>{0,0,0,0};
				m_msg.content.handler=mangle(s_noop,0);
				}

			template<class T,size_t N>
			explicit Message(uint64_t time,uint16_t seq,Processor& handler,const Range<const T>& data
				,const ArrayFixed<uint32_t,N>& params
				,typename std::enable_if< std::is_trivially_copyable<T>::value >::type* dummy=nullptr)
				{
				static_assert(N + 1 <= length<uint32_t>(),"Parameter array too large for a message pack");
				m_msg.content={makeTime(time,seq),mangle(handler,OBJECT_OWNED)};
				auto src=data.begin();
				auto l= data.length()*sizeof(T);
				copyAlloc(src,l);
				auto r=paramPointerGet<uint32_t>();
				for(unsigned int k=0;k<N;++k)
					{r[k + 1]=params[k];}
				}

			Message(const Message& msg)
				{
				m_msg.x=msg.m_msg.x;
				if(m_msg.content.handler & OBJECT_OWNED)
					{
					copyAlloc(m_msg.content.pointer,m_msg.content.params.dwords[0]);
					}
				}

			template<class T,size_t N>
			explicit Message(uint64_t time,uint16_t seq,Processor& handler
				,void* pointer,const ArrayFixed<T,N>& params)
				{
				static_assert(N<=length<T>(),"Array too large for the message");
				m_msg.content={makeTime(time,seq),mangle(handler,0),pointer};
				auto r=paramPointerGet<T>();
				for(unsigned int k=0;k<N;++k)
					{r[k]=params[k];}
				}

			Message(Message&& msg) noexcept
				{
				assert(&msg!=this);
				m_msg.x=msg.m_msg.x;
				Message null;
				msg.m_msg.x=null.m_msg.x;
				}

			~Message()
				{dataFree();}

			Message& operator=(Message&& msg) noexcept
				{
				std::swap(msg.m_msg.x,m_msg.x);
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
				{return m_msg.content.time>>sizeof(uint16_t);}

			uint16_t seqGet() const noexcept
				{return static_cast<uint16_t>(m_msg.content.time&0xffff);}

			bool operator<(const Message& msg) const noexcept
				{return m_msg.content.time > msg.m_msg.content.time;}

			void clear() noexcept
				{
				dataFree();
				m_msg.content.handler=mangle(s_noop,0);
				}

			void process() const
				{handlerGet(m_msg.content.handler)(*this);}

			template<class T>
			Range<const T> paramsGet() const noexcept
				{return Range<const T>(paramPointerGet<T>(),length<T>());}

			template<class T>
			Range<const T> dataGet() const noexcept
				{
				return Range<const T>
					{
					 reinterpret_cast<const T*>(m_msg.content.pointer)
					,m_msg.content.params.dwords[0]
					};
				}

			void* objectGet() const noexcept
				{
				return m_msg.content.pointer;
				}


		private:
			void copyAlloc(const void* src,size_t length);
			void dataFree() noexcept;

			static constexpr uint8_t OBJECT_OWNED=0x1;
			static Processor s_noop;

			template<size_t N,class D=void> union Parameters{};

			template<class D> union Parameters<4,D>
				{
				uint64_t qwords[2];
				uint32_t dwords[4];
				uint16_t words[8];
				uint8_t bytes[16];
				};

			template<class D> union Parameters<8,D>
				{
				uint64_t qwords[1];
				uint32_t dwords[2];
				uint16_t words[4];
				uint8_t bytes[8];
				};

			struct Msg
				{
				uint64_t time;
				uintptr_t handler;
				void* pointer;
				Parameters<sizeof(void*)> params;
				};

			union
				{
				vec4_t<int64_t> x;
				Msg content;
				} m_msg;


			template<class T>
			inline static constexpr size_t length();

			template<class T>
			inline const T* paramPointerGet() const noexcept;

			template<class T>
			inline T* paramPointerGet() noexcept;

			static inline uintptr_t mangle(Processor& handler,uint8_t val) noexcept
				{
				assert((reinterpret_cast<uintptr_t>(&handler)&0xf)==0);
				return reinterpret_cast<uintptr_t>(&handler)|val;
				}

			static inline Processor& handlerGet(uintptr_t x) noexcept
				{return *( reinterpret_cast<Processor*>(x&(~0xf) ) );}
		};


	template<>
	constexpr size_t Message::length<uint8_t>()
		{return sizeof(Parameters<sizeof(void*)>::bytes)/sizeof(uint8_t);}

	template<>
	constexpr size_t Message::length<uint16_t>()
		{return sizeof(Parameters<sizeof(void*)>::words)/sizeof(uint16_t);}

	template<>
	constexpr size_t Message::length<uint32_t>()
		{return sizeof(Parameters<sizeof(void*)>::dwords)/sizeof(uint32_t);}

	template<>
	constexpr size_t Message::length<uint64_t>()
		{return sizeof(Parameters<sizeof(void*)>::qwords)/sizeof(uint64_t);}


	template<>
	inline const uint8_t* Message::paramPointerGet<uint8_t>() const noexcept
		{return m_msg.content.params.bytes;}

	template<>
	inline const uint16_t* Message::paramPointerGet<uint16_t>() const noexcept
		{return m_msg.content.params.words;}

	template<>
	inline const uint32_t* Message::paramPointerGet<uint32_t>() const noexcept
		{return m_msg.content.params.dwords;}

	template<>
	inline const uint64_t* Message::paramPointerGet<uint64_t>() const noexcept
		{return m_msg.content.params.qwords;}


	template<>
	inline uint8_t* Message::paramPointerGet<uint8_t>() noexcept
		{return m_msg.content.params.bytes;}

	template<>
	inline uint16_t* Message::paramPointerGet<uint16_t>() noexcept
		{return m_msg.content.params.words;}

	template<>
	inline uint32_t* Message::paramPointerGet<uint32_t>() noexcept
		{return m_msg.content.params.dwords;}

	template<>
	inline uint64_t* Message::paramPointerGet<uint64_t>() noexcept
		{return m_msg.content.params.qwords;}
	}

#endif
