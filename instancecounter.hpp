//@	{"targets":[{"name":"instancecounter.hpp","type":"include"}]}

#ifndef GLINDE_INSTANCECOUNTER_HPP
#define GLINDE_INSTANCECOUNTER_HPP

#include <cassert>

namespace Glinde
	{
	template<class T>
	class InstanceCounter
		{
		public:
			template<class U,class... Args>
			T& get(Args&&... args)
				{
				if(m_counter==0)
					{m_obj=new T(std::forward<Args>(args)...);}
				++m_counter;
				return *m_obj;
				}

			T& get() noexcept
				{
				assert(m_counter!=0);
				return *m_obj;
				}

			void release() noexcept
				{
				--m_counter;
				if(m_counter==0)
					{delete m_obj;}
				}

		private:
			T* m_obj;
			size_t m_counter;
		};
	}

#endif