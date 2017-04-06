//@	{
//@	"targets":[{"name":"message.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGE_HPP
#define GLINDE_MESSAGE_HPP

#include "../storage/vectortype.hpp"
#include "../storage/arrayfixed.hpp"

namespace Glinde
	{
	class Timeinfo;

	class Message
		{
		private:
			typedef vec4_t<int> DataVec;
			typedef int64_t DataWord;

			typedef void (*DataDestructor)(DataVec&,DataWord&);
			typedef void (*Processor)(void*,const Timeinfo&,DataVec&,DataWord&);

		public:
			Message()
				{m_callbacks.vec=vec2_t<uintptr_t>{0,0};}

			Message(const Message&)=delete;
			Message& operator=(const Message&)=delete;

			template<class Callback,class T,class U>
			explicit Message(Callback& msgproc,T&& data_vec,U&& data_word) noexcept
				{
				static_assert(sizeof(T)<=sizeof(DataVec),"Objects of type T does not fit in the message");
				static_assert(alignof(T)<=alignof(DataVec),"Objects of type T requires to large alignment");
				static_assert(sizeof(U)<=sizeof(DataVec),"Objects of type U does not fit in the message");
				static_assert(alignof(U)<=alignof(DataWord),"Objects of type U requires to large alignment");

				new(&m_data_vec)T(std::move(data_vec));
				new(&m_data_word)U(std::move(data_word));

				DataDestructor dtor=[](DataVec& d1,DataWord& d2)
					{
					reinterpret_cast<T*>(&d1)->~T();
					reinterpret_cast<U*>(&d2)->~U();
					};
				Processor proc=[](void* callback,const Timeinfo& ti,DataVec& d1,DataWord& d2)
					{
					auto& cb=*reinterpret_cast<Callback*>(callback);
					auto& data_vec=*reinterpret_cast<T*>(&d1);
					auto& data_word=*reinterpret_cast<U*>(&d2);
					cb(ti,std::move(data_vec),std::move(data_word));
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
				std::swap(m_data_vec,msg.m_data_vec);
				std::swap(m_data_word,msg.m_data_word);
				std::swap(m_callbacks.vec,msg.m_callbacks.vec);
				m_callback_data=msg.m_callback_data;
				return *this;
				}

			Message(Message&& msg) noexcept:
				m_data_vec(msg.m_data_vec),m_data_word(msg.m_data_word)
				{
				m_callback_data=msg.m_callback_data;
				m_callbacks.vec=msg.m_callbacks.vec;
				msg.m_callbacks.vec=vec2_t<size_t>{0,0};
				}

			void process(const Timeinfo& ti)
				{
				assert(m_callbacks.pointers.r_proc!=nullptr);
				m_callbacks.pointers.r_proc(m_callback_data,ti,m_data_vec,m_data_word);
				}

			~Message() noexcept
				{
				if(m_callbacks.pointers.r_dtor!=nullptr)
					{m_callbacks.pointers.r_dtor(m_data_vec,m_data_word);}
				}

		private:
			DataVec m_data_vec;
			int64_t m_data_word;
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
