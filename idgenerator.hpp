//@	{"targets":[{"name":"idgenerator.hpp","type":"include"}]}

#ifndef GLINDE_IDGENERATOR_HPP
#define GLINDE_IDGENERATOR_HPP

#include "arraydynamic.hpp"

namespace Glinde
	{
	template<class IdType>
	class IdGenerator
		{
		public:
			IdGenerator() noexcept:m_id_next(-1){}

			IdType get() noexcept
				{
				if(m_freelist.length()==0)
					{
					++m_id_next;
					return m_id_next;
					}
				auto ret=std::move( *(m_freelist.end() - 1) );
				m_freelist.truncate();
				return ret;
				}

			IdGenerator& release(const IdType& id)
				{
				m_freelist.append(id);
				return *this;
				}

			IdGenerator& reset() noexcept
				{
				m_freelist.clear();
				m_id_next=-1;
				return *this;
				}

		private:
			ArrayDynamic<IdType> m_freelist;
			IdType m_id_next;
		};
	}

#endif
