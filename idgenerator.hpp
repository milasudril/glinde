//@	{"targets":[{"name":"idgenerator.hpp","type":"include"}]}

#ifndef GLINDE_IDGENERATOR_HPP
#define GLINDE_IDGENERATOR_HPP

#include "storage/arraydynamicstl.hpp"
#include <stack>

namespace Glinde
	{
	template<class IdType,class Freelist=std::stack<IdType,ArrayDynamicSTL<IdType> > >
	class IdGenerator
		{
		public:
			IdGenerator() noexcept:m_id_next(-1){}

			IdType get() noexcept
				{
				if(m_freelist.size()==0)
					{
					++m_id_next;
					return m_id_next;
					}
				auto ret=m_freelist.top();
				m_freelist.pop();
				return ret;
				}

			IdGenerator& release(const IdType& id)
				{
				m_freelist.push(id);
				return *this;
				}

			IdGenerator& reset() noexcept
				{
				m_freelist.clear();
				m_id_next=-1;
				return *this;
				}

		private:
			Freelist m_freelist;
			IdType m_id_next;
		};
	}

#endif
