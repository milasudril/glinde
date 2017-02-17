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
			explicit Surface(int width,int height);

			~Surface();

			Surface(Surface&& s) noexcept:
				 m_handle(s.m_handle)
				{s.m_handle.reset();}

			Surface& operator=(Surface&& s) noexcept
				{
				std::swap(s.m_handle,m_handle);
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

			const uint8_t* data() const noexcept;

			uint8_t* data() noexcept;

			void dirtySet() noexcept;

			void save(const char* filename) const;

			void renderRegionAdd(const Rectangle& rect)
				{
				auto size_new=emax(rect.size(),m_dirty_region.size());
				auto min_new=emin(rect.min(),m_dirty_region.min());
	
				m_dirty_region.size()=size_new;	
				m_dirty_region.min()=min_new;				
				}

			void renderRegionClear()
				{
				m_dirty_region.size()=Vec2{0,0};
				m_dirty_region.min()=Vec2{double(width()),double(height())};
				}
	
		private:
			Handle<surface_t> m_handle;
			Rectangle m_dirty_region;
		};
	};

#endif
