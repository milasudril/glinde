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
				 m_handle(s.m_handle),m_dirty_rect(s.m_dirty_rect)
				{s.m_handle.reset();}

			Surface& operator=(Surface&& s) noexcept
				{
				std::swap(s.m_handle,m_handle);
				std::swap(s.m_dirty_rect,m_dirty_rect);
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

			template<class Callback>
			Surface& contentModify(Callback&& cb)
				{
				cb(data_nc(),width(),height());
				return dirtySet();
				}

			void save(const char* filename) const;

			Surface& dirtyRectangleAdd(const Rectangle& rect)
				{
				m_dirty_rect=boundingRectangle(m_dirty_rect,rect);
				rect_clamp();
				return *this;
				}

			Surface& dirtyRectangleClear()
				{
				m_dirty_rect.max()=Vec2(0,0);
				m_dirty_rect.min()=Vec2(width(),height());
				return *this;
				}

			Rectangle dirtyRectangle() const noexcept
				{return m_dirty_rect;}

			void rectangleGet(const Rectangle& rect,Pixel* dest) const noexcept;

		private:
			Pixel* data_nc() noexcept;
			Surface& dirtySet() noexcept;
			void rect_clamp() noexcept;
			Handle<surface_t> m_handle;
			Rectangle m_dirty_rect;
		};
	};

#endif
