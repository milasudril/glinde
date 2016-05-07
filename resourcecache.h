#ifdef __WAND__
target[name[resourcecache.h] type[include]]
#endif

#ifndef GLINDE_RESOURCECACHE_H
#define GLINDE_RESOURCECACHE_H

#include "arrayfixed.h"
#include "arrayinit.h"
#include <cstdint>

namespace Glinde
	{
	template<class T>
	class ResourceCache
		{
		public:
			class Entry
				{
				public:
					Entry():m_tag(0x0){}

					uint32_t tagGet() const noexcept
						{return m_tag&0xffffff;}

					bool validIs() const noexcept
						{return m_tag&0xff000000;}

					Entry& tagSet(uint32_t id) noexcept
						{
						m_tag=(id>>8)|0xff000000;
						return *this;
						}

					T& objectGet() noexcept
						{return m_object;}
				private:
					uint32_t m_tag;
					T m_object;
				};

			std::pair<Entry&,bool> resourceGet(uint32_t id) noexcept
				{
				auto& row=m_data[id&0xff];
				for(size_t k=0;k<row.length();++k)
					{
					auto& entry=row[k];
					if(entry.validIs() && entry.tagGet()==id>>8)
						{return {entry,1};}
					}
				return {row[0],0};
				}

		private:
			ArrayFixed< ArrayFixed<Entry,4>,256> m_data;
		};
	}

#endif
