//@ {"targets":[{"name":"message2.hpp","type":"include"}]}

#ifndef GLINDE_MESSAGE_HPP
#define GLINDE_MESSAGE_HPP

#include "vectortype.hpp"
#include "debug.hpp"
#include "variant.hpp"
#include <cstdint>
#include <cstddef>
#include <utility>
#include <new>

namespace Glinde
	{
	class Message
		{
		public:
			class ProcessorImpl
				{
				public:
					virtual void operator()(Message&& message)=0;
				};
		public:
			bool operator>(const Message& msg) const noexcept
				{return m_content.msg.seq > msg.m_content.msg.seq;}

			template<class T>
			T dataRelease() noexcept
				{
				static_assert(sizeof(T)<=16,"The given type is larger than data");
				return std::move(reinterpret_cast<T&>(m_content.msg.data));
				};

			template<class T,class Callback>
			class Processor:public ProcessorImpl
				{
				public:
					Processor(Callback&& process):m_process(std::move(process)){}

					Callback& callback() noexcept
						{return m_process;}

				private:
					void operator()(Message&& msg)
						{
					//	GLINDE_DEBUG_PRINT("m_process [#1;]: #0;",&m_process,__PRETTY_FUNCTION__);
						auto obj=msg.dataRelease<T>();
						m_process(msg.time(),obj);
						}
					Callback m_process;
				};

			template<class T,class Callback>
			explicit Message(Time seq,Processor<T,Callback>& receiver,T&& data) noexcept
				{
				static_assert(sizeof(T)<=16,"Objects of type T does not fit in the message");
				m_content.msg.seq=seq;
				m_content.msg.receiver=&receiver;
			//	GLINDE_DEBUG_PRINT("receiver [#1;]: #0;",&receiver,__PRETTY_FUNCTION__);
				new(&m_content.msg.data)T(std::move(data));
				}

			Message(const Message&)=delete;
			Message& operator=(const Message&)=delete;

			Message()
				{m_content.x=vec4_t<uint64_t>{0,0,0,0};}
	
			Message(Message&& msg) noexcept
				{
				m_content.x=msg.m_content.x;
				msg.m_content.x=vec4_t<uint64_t>{0,0,0,0};
				}

			void clear() noexcept
				{m_content.x=vec4_t<uint64_t>{0,0,0,0};}

			Message& operator=(Message&& msg) noexcept
				{
				std::swap(m_content.x,msg.m_content.x);
				return *this;
				}

			void process()
				{
			//	GLINDE_DEBUG_PRINT("receiver [#1;]: #0;",m_content.msg.receiver,__PRETTY_FUNCTION__);
				if(m_content.msg.receiver!=nullptr)
					{(*m_content.msg.receiver)(std::move(*this));}
				}

			uint64_t time() const noexcept
				{return m_content.msg.seq.time();}

			uint16_t seq() const noexcept
				{return m_content.msg.seq.seq();}

			Message& seq(uint16_t seq) noexcept
				{
				m_content.msg.seq.seq(seq);
				return *this;
				}

			

		private:
			template<size_t N=sizeof(void*),bool dummy=true>
			struct MessageData{};

			template<bool dummy>
			struct MessageData<4,dummy>
				{
				Time seq;
				ProcessorImpl* receiver;
				int32_t padding;
				uint8_t data[16];
				};

			template<bool dummy>
			struct MessageData<8,dummy>
				{
				double time;
				int32_t id;
				ProcessorImpl* receiver;
				
				};

			union
				{
				vec4_t<uint64_t> x;
				MessageData< sizeof(void*) > msg;
				} m_content;
		};
	}

#endif