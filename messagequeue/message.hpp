//@	{
//@	"targets":[{"name":"message.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGE_HPP
#define GLINDE_MESSAGE_HPP

#include "../vectortype.hpp"
#include "../arrayfixed.hpp"

namespace Glinde
	{
	class Message
		{
		private:
			static constexpr size_t DATA_SIZE=24;
			static constexpr size_t DATA_ALIGNMENT=alignof(vec4_t<float>);
			typedef ArrayFixed<uint64_t,DATA_SIZE/3> Data;

			typedef void (*DataDestructor)(Data&);

			class ProcBase
				{
				public:
					virtual void process(const Data&) noexcept=0;
				};

		public:
			template<class T,class Callback>
			class Processor:public ProcBase
				{
				public:
					Processor(Callback&& cb):m_cb(cb)
						{}

					void process(const Data& data) noexcept
						{m_cb( *( reinterpret_cast<const T*>(data.begin() ) ) );}

				private:
					Callback m_cb;
				};

			Message()
				{m_callbacks.vec=vec2_t<uintptr_t>{0,0};}

			Message(const Message&)=delete;
			Message& operator=(const Message&)=delete;

			template<class T,class Callback>
			Message(Processor<T,Callback>& msgproc,T&& data) noexcept
				{
				static_assert(sizeof(T)<=DATA_SIZE,"Objects of type T does not fit in the message");
				static_assert(alignof(T)<=DATA_ALIGNMENT,"Objects of type T requires to large alignment");
				new(m_data.begin())T(std::move(data));
				DataDestructor dtor=[](Data& d)
					{reinterpret_cast<T*>(d.begin())->~T();};

				m_callbacks.vec=vec2_t<uintptr_t>
					{
					 reinterpret_cast<uintptr_t>(&msgproc)
					,reinterpret_cast<uintptr_t>(dtor)
					};
				}

			Message& operator=(Message&& msg) noexcept
				{
				std::swap(m_data,msg.m_data);
				std::swap(m_callbacks.vec,msg.m_callbacks.vec);
				return *this;
				}

			Message(Message&& msg) noexcept:m_data(std::move(msg.m_data))
				{
				m_callbacks.vec=msg.m_callbacks.vec;
				msg.m_callbacks.vec=vec2_t<size_t>{0,0};
				}
			
			void process() const
				{
				if(m_callbacks.pointers.r_proc!=nullptr)
					{m_callbacks.pointers.r_proc->process(m_data);}
				}

			~Message() noexcept
				{
				if(m_callbacks.pointers.r_dtor!=nullptr)
					{m_callbacks.pointers.r_dtor(m_data);}
				}

		private:
			alignas(DATA_ALIGNMENT) Data m_data;
			union
				{
				vec2_t<uintptr_t> vec;
				struct
					{
					ProcBase* r_proc;
					DataDestructor r_dtor;
					} pointers;
				} m_callbacks;
		};
	};

#endif
