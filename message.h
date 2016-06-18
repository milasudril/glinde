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
			class alignas(16) Processor
				{
				public:
					virtual void operator()(const Message& data){}
				};

			inline Message();

			template<class T>
			explicit Message(uint64_t time,uint16_t seq,Processor& handler,const Range<const T>& data
				,typename std::enable_if< std::is_trivially_copyable<T>::value >::type* dummy=nullptr)
				{
				m_msg.content={makeTime(time,seq),mangle(handler,OBJECT_OWNED)};
				auto src=data.begin();
				auto l= data.length()*sizeof(T);
				copyAlloc(src,l);
				}

			inline Message(const Message& msg);

			template<class T,size_t N>
			explicit Message(uint64_t time,uint16_t seq,Processor& handler
				,const ArrayFixed<T,N>& data);

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

			inline uint64_t timeGet() const noexcept;

			inline uint16_t seqGet() const noexcept;

			inline bool operator<(const Message& msg) const noexcept;

			inline void clear() noexcept;

			inline void process() const;

			template<class T>
			Range<const T> paramsGet() const noexcept
				{return Range<const T>(dataPointerGet<const T>(),length<T>());}

			template<class T>
			Range<const T> dataGet() const noexcept;


		private:
			void copyAlloc(const void* src,size_t length);
			void dataFree() noexcept;

			static constexpr uint8_t OBJECT_OWNED=0x1;
			static Processor s_noop;

			template<size_t N,class D=void> union Data{};

			template<size_t N,class D=void> struct Msg{};

			template<class D> union Data<4,D>
				{
				uintptr_t intptrs[5];
				uint64_t qwords[2];
				uint32_t dwords[5];
				uint16_t words[10];
				uint8_t bytes[20];
				};

			template<class D> union Data<8,D>
				{
				uintptr_t intptrs[2];
				uint64_t qwords[2];
				uint32_t dwords[4];
				uint16_t words[8];
				uint8_t bytes[16];
				};

			template<class D> struct Msg<4,D>
				{
				uint64_t time;
				uintptr_t handler;
				uint32_t filler;
				Data<4> data;
				};

			template<class D> struct Msg<8,D>
				{
				uint64_t time;
				uintptr_t handler;
				Data<8> data;
				};

			union
				{
				vec4_t<int64_t> x;
				Msg<sizeof(void*)> content;
				} m_msg;


			template<class T>
			static constexpr size_t length();

			template<class T>
			const T* dataPointerGet() const noexcept;

			template<class T>
			T* dataPointerGet() noexcept;

			static inline uintptr_t mangle(Processor& handler,uint8_t val) noexcept
				{
				assert((reinterpret_cast<uintptr_t>(&handler)&0xf)==0);
				return reinterpret_cast<uintptr_t>(&handler)|val;
				}

			static inline Processor& handlerGet(uintptr_t x) noexcept
				{return *( reinterpret_cast<Processor*>(x&(~0xf) ) );}
		};


	Message::Message()
		{
		m_msg.x=vec4_t<int64_t>{0,0,0,0};
		m_msg.content.handler=mangle(s_noop,0);
		}

	Message::Message(const Message& msg)
		{
		m_msg.x=msg.m_msg.x;
		if(m_msg.content.handler & OBJECT_OWNED)
			{
			copyAlloc(
				 reinterpret_cast<const void*>(m_msg.content.data.intptrs[0])
				,m_msg.content.data.intptrs[1]);
			}
		}

	template<class T,size_t N>
	Message::Message(uint64_t time,uint16_t seq,Processor& handler
		,const ArrayFixed<T,N>& data)
		{
		static_assert(N<=length<T>(),"Array too large for the message");
		m_msg.content={makeTime(time,seq),mangle(handler,0)};
		auto r=dataPointerGet<T>();
		for(unsigned int k=0;k<N;++k)
			{r[k]=data[k];}
		}

	uint64_t Message::timeGet() const noexcept
		{return m_msg.content.time>>sizeof(uint16_t);}

	uint16_t Message::seqGet() const noexcept
		{return static_cast<uint16_t>(m_msg.content.time&0xffff);}

	bool Message::operator<(const Message& msg) const noexcept
		{
		if(m_msg.content.time > msg.m_msg.content.time)
			{return 1;}
		return 0;
		}

	void Message::clear() noexcept
		{
		dataFree();
		m_msg.content.handler=mangle(s_noop,0);
		}

	void Message::process() const
		{handlerGet(m_msg.content.handler)(*this);}

	template<class T>
	Range<const T> Message::dataGet() const noexcept
		{
		return Range<const T>(
			 reinterpret_cast<const T*>(m_msg.content.data.intptrs[0])
			,m_msg.content.data.intptrs[1]
			);
		}



	template<>
	constexpr size_t Message::length<uint8_t>()
		{return sizeof(Data<sizeof(void*)>::bytes)/sizeof(uint8_t);}

	template<>
	constexpr size_t Message::length<uint16_t>()
		{return sizeof(Data<sizeof(void*)>::words)/sizeof(uint16_t);}

	template<>
	constexpr size_t Message::length<uint32_t>()
		{return sizeof(Data<sizeof(void*)>::dwords)/sizeof(uint32_t);}

	template<>
	constexpr size_t Message::length<uint64_t>()
		{return sizeof(Data<sizeof(void*)>::qwords)/sizeof(uint64_t);}


	template<>
	const uint8_t* Message::dataPointerGet() const noexcept
		{return m_msg.content.data.bytes;}

	template<>
	const uint16_t* Message::dataPointerGet() const noexcept
		{return m_msg.content.data.words;}

	template<>
	const uint32_t* Message::dataPointerGet() const noexcept
		{return m_msg.content.data.dwords;}

	template<>
	const uint64_t* Message::dataPointerGet() const noexcept
		{return m_msg.content.data.qwords;}


	template<>
	uint8_t* Message::dataPointerGet() noexcept
		{return m_msg.content.data.bytes;}

	template<>
	uint16_t* Message::dataPointerGet() noexcept
		{return m_msg.content.data.words;}

	template<>
	uint32_t* Message::dataPointerGet() noexcept
		{return m_msg.content.data.dwords;}

	template<>
	uint64_t* Message::dataPointerGet() noexcept
		{return m_msg.content.data.qwords;}
	}

#endif
