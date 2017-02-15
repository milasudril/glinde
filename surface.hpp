//@	{
//@	 "targets":[{"name":"surface.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"surface.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_SURFACE_HPP
#define PAGECOMPOSER_SURFACE_HPP

#include "handle.hpp"
#include <utility>
#include <cstdint>

namespace PageComposer
	{
	class Surface
		{
		public:
			Surface(int width,int height);

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
	
		private:
			Handle<surface_t> m_handle;
		};
	};

#endif
