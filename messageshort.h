//@ {"targets":[{"name":"messageshort.h","type":"include"}]}

#ifndef GLINDE_MESSAGESHORT_H
#define GLINDE_MESSAGESHORT_H

#include "vectortype.h"
#include <cstdint>
#include <cstddef>
#include <utility>
#include <new>

namespace Glinde
	{
	class MessageShort
		{
		public:
			class Time
				{
				public:
					Time()=default;
					Time(const Time&)=default;
					Time(Time&&)=default;
					Time& operator=(const Time&)=default;
					Time& operator=(Time&&)=default;
					~Time()=default;

					explicit Time(uint64_t frame,uint16_t seq) noexcept
						{m_val=((frame<<16)|seq);}

					explicit Time(uint64_t frame) noexcept:m_val(frame<<16)
						{}

					uint64_t timeGet() const noexcept
						{return m_val>>16;}
					
					Time& seqSet(uint16_t seq) noexcept
						{
						m_val=((timeGet()<<16)|seq);
						return *this;
						}

					bool operator>(Time t) const noexcept
						{return m_val>t.m_val;}
				private:
					uint64_t m_val;
				};

		private:
			class ProcessorImpl
				{
				public:
					virtual void operator()(MessageShort&& message)=0;
				};
		public:

			bool operator>(const MessageShort&msg) const noexcept
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

				private:
					void operator()(MessageShort&& msg)
						{return m_process(msg.timeGet(),std::move(msg.dataRelease<T>()));}
					Callback m_process;
				};

			template<class T,class Callback>
			MessageShort(Time seq,Processor<T,Callback>& receiver,T&& data) noexcept
				{
				static_assert(sizeof(T)<=16,"Objects of type T does not fit in the message");
				m_content.msg.seq=seq;
				m_content.msg.receiver=&receiver;
				new(&m_content.msg.data)T(std::move(data));
				}

			MessageShort(const MessageShort&)=delete;
			MessageShort& operator=(const MessageShort&)=delete;

			MessageShort()
				{m_content.x=vec4_t<uint64_t>{0,0,0,0};}
	
			MessageShort(MessageShort&& msg) noexcept
				{
				m_content.x=msg.m_content.x;
				msg.m_content.x=vec4_t<uint64_t>{0,0,0,0};
				}

			MessageShort& operator=(MessageShort&& msg) noexcept
				{
				std::swap(m_content.x,msg.m_content.x);
				return *this;
				}

			void process()
				{(*m_content.msg.receiver)(std::move(*this));}

			Time timeGet() const noexcept
				{return m_content.msg.seq;}

		private:
			template<size_t N=sizeof(void*),bool dummy=true>
			struct Message{};

			template<bool dummy>
			struct Message<4,dummy>
				{
				Time seq;
				ProcessorImpl* receiver;
				int32_t padding;
				uint8_t data[16];
				};

			template<bool dummy>
			struct Message<8,dummy>
				{
				Time seq;
				ProcessorImpl* receiver;
				uint8_t data[16];
				};

			union
				{
				vec4_t<uint64_t> x;
				Message< sizeof(void*) > msg;
				} m_content;
		};
	}

#endif