//@	{
//@	"targets":[{"name":"messagequeue.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGEHEADER_HPP
#define GLINDE_MESSAGEHEADER_HPP

#include "../vectortype.hpp"
#include "../arrayfixed.hpp"

namespace Glinde
	{
	class Message
		{
		private:
			static constexpr size_t DATA_SIZE=24;
			static constexpr size_t DATA_ALIGNMENT=alignof(vec4_t<float>);
			typedef ArrayFixed<uint8_t,DATA_SIZE> Data;

			class ProcBase
				{
				public:
					virtual void process(const Data&)=0;
				};

			class ManagerBase
				{
				public:
					virtual void destroy(Data&)=0;
				};

		public:
			template<class T,class Callback>
			class Processor:public ProcBase
				{
				public:
					Processor(Callback&& cb):m_cb(cb)
						{}

					void process(const Data& data)
						{m_cb( *( reinterpret_cast<T*>(data.begin() ) ) );}

				private:
					Callback m_cb;
				};

			template<class T,class Callback>
			class Manager:public ManagerBase
				{
				public:
					Manager(Callback&& cb):m_cb(cb)
						{}

					void destroy(const Data& data)
						{m_cb( *( reinterpret_cast<T*>(data.begin() ) ) );}

				private:
					Callback m_cb;
				};


			Message(const Message&)=delete;
			Message& operator=(const Message&)=delete;

			template<class T,class Callback>
			Message(Processor<T,Callback>& msgproc,Manager<T,Callback>& msgman
				,T&& data) noexcept:r_proc(msgproc),r_manager(msgman)
				{
				static_assert(sizeof(T)<=DATA_SIZE,"Objects of type T does not fit in the message");
				static_assert(alignof(T)<=DATA_ALIGNMENT,"Objects of type T requires to large alignment");
				new(m_data.begin())T(std::move(data));
				}

			Message& operator=(Message&& msg) noexcept
				{
				std::swap(m_data,msg.m_data);
				std::swap(m_callbacks.vec,msg.m_callbacks.vec);
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
				if(m_callbacks.pointers.r_manager!=nullptr)
					{m_callbacks.pointers.r_manager->destroy(m_data);}
				}

		private:
			alignas(DATA_ALIGNMENT) ArrayFixed<uint8_t,DATA_SIZE> m_data;
			union
				{
				vec2_t<size_t> vec;
				struct
					{
					ProcBase* r_proc;
					ManagerBase* r_manager;
					} pointers;
				} m_callbacks;
			
		};
	};

#endif
