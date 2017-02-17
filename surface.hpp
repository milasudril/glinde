//@	{
//@	 "targets":[{"name":"surface.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"surface.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_SURFACE_HPP
#define PAGECOMPOSER_SURFACE_HPP

#include "handle.hpp"
#include "rectangle.hpp"
#include <utility>
#include <cstdint>
#include <algorithm>

namespace PageComposer
	{
	class Surface
		{
		public:
			struct Pixel
				{
				uint8_t blue;
				uint8_t green;
				uint8_t red;
				uint8_t alpha;
				};

			explicit Surface(int width,int height);

			~Surface();

			Surface(Surface&& s) noexcept:
				 m_handle(s.m_handle),m_dirty_region(s.m_dirty_region)
				,m_dirty_data(s.m_dirty_data),m_dirty_fetched(s.m_dirty_fetched)
				{
				s.m_handle.reset();
				s.m_dirty_data=nullptr;
				}

			Surface& operator=(Surface&& s) noexcept
				{
				std::swap(s.m_handle,m_handle);
				std::swap(s.m_dirty_region,m_dirty_region);
				std::swap(s.m_dirty_data,m_dirty_data);
				std::swap(s.m_dirty_fetched,m_dirty_fetched);
				return *this;
				}

			Surface(const Surface& s)=delete;
			Surface& operator=(const Surface& s)=delete;


			const Handle<surface_t>& handle() const noexcept
				{return m_handle;}

			Handle<surface_t>& handle() noexcept
				{return m_handle;}

			int width() const noexcept;

			int height() const noexcept;

			int stride() const noexcept;

			const Pixel* data() const noexcept;

			Pixel* data() noexcept;

			Surface& dirtySet() noexcept;

			void save(const char* filename) const;

			Surface& renderRegionAdd(const Rectangle& rect)
				{
				m_dirty_region.max()=emax(rect.max(),m_dirty_region.max());
				m_dirty_region.min()=emin(rect.min(),m_dirty_region.min());
				region_clamp();
				m_dirty_fetched=0;
				return *this;
				}

			Surface& renderRegionClear()
				{
				m_dirty_region.max()=Vec2{0,0};
				m_dirty_region.min()=Vec2{double(width()),double(height())};
				m_dirty_fetched=0;
				return *this;
				}

			const Pixel* dataDirty() const noexcept
				{
				if(!m_dirty_fetched)
					{dirty_fetch();}
				return m_dirty_data;
				}

			Rectangle regionDirty() const noexcept
				{return m_dirty_region;}

			Pixel* dataDirty() noexcept
				{
				if(!m_dirty_fetched)
					{dirty_fetch();}
				return m_dirty_data;
				}
	
		private:
			void dirty_fetch() const noexcept;
			void region_clamp() noexcept;
			Handle<surface_t> m_handle;
			Rectangle m_dirty_region;
			Pixel* m_dirty_data;
			mutable bool m_dirty_fetched;
		};
	};

#endif
