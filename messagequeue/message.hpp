//@	{
//@	"targets":[{"name":"message.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGE_HPP
#define GLINDE_MESSAGE_HPP

#include "../vectortype.hpp"
#include "../arrayfixed.hpp"

namespace Glinde
	{
	class Timeinfo;

	class Message
		{
		private:
			typedef vec4_t<int> DataVec;
			typedef int64_t DataWord;

			typedef void (*DataDestructor)(DataVec&);
			typedef void (*Processor)(void*,const Timeinfo&,DataVec&&);

		public:
			Message()
				{m_callbacks.vec=vec2_t<uintptr_t>{0,0};}

			Message(const Message&)=delete;
			Message& operator=(const Message&)=delete;

			template<class Callback,class T>
			explicit Message(Callback& msgproc,T&& data) noexcept
				{
				static_assert(sizeof(T)<=sizeof(DataVec),"Objects of type T does not fit in the message");
				static_assert(alignof(T)<=alignof(DataVec),"Objects of type T requires to large alignment");
				new(&m_data)T(std::move(data));
				DataDestructor dtor=[](DataVec& d)
					{reinterpret_cast<T*>(&d)->~T();};
				Processor proc=[](void* callback,const Timeinfo& ti,DataVec&& d)
					{
					auto& cb=*reinterpret_cast<Callback*>(callback);
					auto& data=*reinterpret_cast<T*>(&d);
					cb(ti,std::move(data));
					};
				m_callback_data=&msgproc;
				m_callbacks.vec=vec2_t<uintptr_t>
					{
					 reinterpret_cast<uintptr_t>(proc)
					,reinterpret_cast<uintptr_t>(dtor)
					};
				}

			Message& operator=(Message&& msg) noexcept
				{
				std::swap(m_data,msg.m_data);
				std::swap(m_callbacks.vec,msg.m_callbacks.vec);
				m_callback_data=msg.m_callback_data;
				return *this;
				}

			Message(Message&& msg) noexcept:m_data(std::move(msg.m_data))
				{
				m_callback_data=msg.m_callback_data;
				m_callbacks.vec=msg.m_callbacks.vec;
				msg.m_callbacks.vec=vec2_t<size_t>{0,0};
				}
			
			void process(const Timeinfo& ti)
				{
				assert(m_callbacks.pointers.r_proc!=nullptr);
				m_callbacks.pointers.r_proc(m_callback_data,ti,std::move(m_data));
				}

			~Message() noexcept
				{
				if(m_callbacks.pointers.r_dtor!=nullptr)
					{m_callbacks.pointers.r_dtor(m_data);}
				}

		private:
			DataVec m_data;
			int64_t m_data_a;
			void* m_callback_data;
			union
				{
				vec2_t<uintptr_t> vec;
				struct
					{
					Processor r_proc;
					DataDestructor r_dtor;
					} pointers;
				} m_callbacks;
		};
#ifdef SIZE_CHECK
	template<size_t k>
	class X
		{
		public:
			X()
				{}
		};
	
	static constexpr X<sizeof(Message)> z;
#endif
	}

#endif
