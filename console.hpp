//@	{
//@	 "dependencies_extra":[{"ref":"console.o","rel":"implementation"}]
//@	,"targets":[{"name":"console.hpp","type":"include"}]
//@	}
#ifndef GLINDE_CONSOLE_HPP
#define GLINDE_CONSOLE_HPP

#include "faceindirect.hpp"
#include "vectortype.hpp"
#include "new.hpp"
#include "color.hpp"
#include <geosimd/point.hpp>
#include <memory>

namespace Glinde
	{
	class Console
		{
		public:
			Console(uint32_t n_rows,uint32_t n_cols);

			Console& colorMask(uint8_t color_mask);
			Console& writeRaw(const char* string);
			Console& writeUTF8(const char* string);
			Console& write(char ch);
			Console& write(uint32_t codepoint);

			uint32_t colsCount() const noexcept
				{return m_n_cols;}

			uint32_t rowsCount() const noexcept
				{return m_n_rows;}



			auto vertices() const noexcept
				{return Range<const GeoSIMD::Point<float>>{m_vertices.get(),m_n_cols*m_n_rows*4};}

			auto vertices(size_t k) const noexcept
				{return row(m_vertices.get(),k,4);}



			auto colorsFg() const noexcept
				{return Range<Color>{m_colors_fg.get(),m_n_cols*m_n_rows*4};}

			auto colorsFg(size_t k) const noexcept
				{return row(m_colors_fg.get(),k,4);}



			auto colorsBg() const noexcept
				{return Range<Color>{m_colors_bg.get(),m_n_cols*m_n_rows*4};}

			auto colorsBg(size_t k) const noexcept
				{return row(m_colors_bg.get(),k,4);}



			auto uvs() const noexcept
				{return Range<const vec2_t<float>>{m_uvs.get(),m_n_cols*m_n_rows*4};}

			auto uvs(size_t k) const noexcept
				{return row(m_uvs.get(),k,4);}



			auto faces() const noexcept
				{return Range<const FaceIndirect>{m_faces.get(),m_n_cols*m_n_rows*2};}

			Range<const FaceIndirect> faces(size_t k) const noexcept
				{return row(m_faces.get(),k,2);}

			float lineOffset(size_t k) const noexcept
				{return -static_cast<float>(2*k)/static_cast<float>(m_n_rows);}

			size_t size() const noexcept
				{return colsCount()*rowsCount();}

		private:
			std::unique_ptr<GeoSIMD::Point<float>[]> m_vertices;
			std::unique_ptr<Color[]> m_colors_fg;
			std::unique_ptr<Color[]> m_colors_bg;
			std::unique_ptr<vec2_t<float>[]> m_uvs;
			std::unique_ptr<FaceIndirect[]> m_faces;

			uint32_t m_n_cols;
			uint32_t m_n_rows;

			Color m_color_fg;
			Color m_color_bg;
			size_t m_position;
			size_t m_line_current;
			uint32_t m_utf8_state;
			uint32_t m_codepoint;

			template<class T>
			Range<const T> row(const T* what,size_t k,size_t count) const noexcept
				{
				auto N=m_n_cols*count;
				auto n_rows=m_n_rows;
				return Range<const T>
					{
 					 what+N*( (k + m_line_current)%n_rows)
					,N
					};
				}
		};
	}

#endif

