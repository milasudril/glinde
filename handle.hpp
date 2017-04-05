//@	{"targets":[{"name":"handle.pp","type":"include"}]}

#ifndef GLINDE_HANDLE_HPP
#define GLINDE_HANDLE_HPP

#include <type_traits>
#include <utility>

namespace Glinde
	{
	template<class T,T null_value,class Deleter>
	class Handle
		{
		public:
			static constexpr T null()
				{return null_value;}

			Handle():m_handle(null_value){}

			explicit Handle(T handle,const Deleter& deleter
				,typename std::enable_if< !std::is_reference<Deleter>::value >::type* dummy=nullptr)
				noexcept:
				m_handle(handle),m_d(deleter)
				{}

			explicit Handle(T handle,Deleter&& deleter
				,typename std::enable_if< !std::is_reference<Deleter>::value >::type* dummy=nullptr)
				noexcept:
				m_handle(handle),m_d(std::move(deleter))
				{}

			Handle(const Handle&)=delete;

			Handle(Handle&& h) noexcept:
				m_handle(h.handle),m_d(std::move(h.m_d))
				{h.m_handle=null();}

			~Handle()
				{
				if(m_handle!=null())
					{m_d(m_handle);}
				}

			Handle& operator=(Handle&& h) noexcept
				{
				std::swap(h.m_handle,m_handle);
				std::swap(h.m_d,m_d);
				return *this;
				}

			Handle& operator=(const Handle&)=delete;

			T get() const noexcept
				{return m_handle;}

			T operator->() const noexcept
				{return m_handle;}

			operator bool() const noexcept
				{return !(m_handle==null());}

			bool operator==(const Handle& other) const
				{return other.m_handle==m_handle;}

		private:
			T m_handle;
			Deleter m_d;
		};
	}

#endif
