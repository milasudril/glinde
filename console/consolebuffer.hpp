/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "targets":[{"name":"consolebuffer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"consolebuffer.o","rel":"implementation"}]
//@	}
#ifndef GLINDE_CONSOLEBUFFER_HPP
#define GLINDE_CONSOLEBUFFER_HPP

#include "../geometry/faceindirect.hpp"
#include "../storage/vectortype.hpp"
#include "../color.hpp"
#include "../storage/new.hpp"
#include <geosimd/point.hpp>
#include <memory>
#include <algorithm>

namespace Glinde
	{
	struct VGACell;

	class ConsoleBuffer
		{
		public:
			explicit ConsoleBuffer(uint32_t n_rows,uint32_t n_cols);

			ConsoleBuffer& colorMask(uint8_t color_mask) noexcept
				{
				m_color=color_mask;
				character_render('\0',size());
				return *this;
				}

			ConsoleBuffer& writeRaw(const char* string) noexcept;
			ConsoleBuffer& writeUTF8(const char* string) noexcept;
			ConsoleBuffer& write(char ch) noexcept;
			ConsoleBuffer& write(uint32_t codepoint) noexcept;
			ConsoleBuffer& writeVGADump(Range<const VGACell> dump) noexcept;

			uint32_t colsCount() const noexcept
				{return m_n_cols;}

			uint32_t rowsCount() const noexcept
				{return m_n_rows;}



			auto vertices() const noexcept
				{return Range<const GeoSIMD::Point<float>>{m_vertices.get(),size()*4};}

			auto vertices(size_t k) const noexcept
				{return row(m_vertices.get(),k,4);}

			auto verticesFull() const noexcept
				{return Range<const GeoSIMD::Point<float>>{m_vertices.get(),sizeFull()*4};}



			auto color() const noexcept
				{return Range<uint32_t>{m_colors.get(),size()*4};}

			auto colorsFull() const noexcept
				{return Range<uint32_t>{m_colors.get(),sizeFull()*4};}



			auto uvs() const noexcept
				{return Range<const vec2_t<float>>{m_uvs.get(),size()*4};}

			auto uvsFull() const noexcept
				{return Range<const vec2_t<float>>{m_uvs.get(),sizeFull()*4};}

			auto uvs(size_t k) const noexcept
				{return row(m_uvs.get(),k,4);}



			auto faces() const noexcept
				{return Range<const FaceIndirect<>>{m_faces.get(),size()*2};}

			auto facesFull() const noexcept
				{return Range<const FaceIndirect<>>{m_faces.get(),sizeFull()*2};}

			Range<const FaceIndirect<>> faces(size_t k) const noexcept
				{return row(m_faces.get(),k,2);}



			float lineOffset(size_t k) const noexcept
				{return -static_cast<float>(2*k)/static_cast<float>(m_n_rows);}

			size_t size() const noexcept
				{return colsCount()*rowsCount();}

			size_t lineCurrent() const noexcept
				{return m_line_current;}

			void erase() noexcept
				{
				if(m_full)
					{
					if(m_position%m_n_cols==0)
						{scroll_up();}
					}
				go_back();	
				character_render(' ',m_position);
				}

			void eraseLinefeed(intptr_t l) noexcept
				{
				if(m_full)
					{scroll_up();}
				auto line_current=m_position/m_n_cols;
				line_current=line_current!=0?line_current-1:m_n_rows-1;
				m_position=(line_current*m_n_cols + l%m_n_cols)%size();
				}

			vec2_t<float> cursorPosition() const noexcept
				{
				auto row=m_position/m_n_cols;
				auto col=m_position%m_n_cols;
				return vec2_t<float>
					{
					 static_cast<float>(2*col)/static_cast<float>(m_n_cols)
					,m_full?-2.0f*static_cast<float>(m_n_rows - 1)/static_cast<float>(m_n_rows)
						:-static_cast<float>(2*row)/static_cast<float>(m_n_rows)
					};
				}

			size_t sizeFull() const noexcept
				{return size() + 1;}

			ConsoleBuffer& fill(int n,uint32_t ch) noexcept;

		private:
			void go_back() noexcept
				{m_position=m_position!=0? m_position - 1:size() - 1;}

			void scroll_up() noexcept
				{m_line_current=m_line_current!=0? m_line_current - 1 : m_n_rows - 1;}

			void scroll_down() noexcept;

			void character_render(uint16_t ch,size_t position);

			void position_advance() noexcept
				{
				auto position=(m_position+1)%size();
				if(position==0)
					{m_full=1;}
				m_position=position;
				if(m_full && m_position%m_n_cols==0)
					{scroll_down();}
				}

			void position_advance_newline() noexcept
				{
				auto n_cols=m_n_cols;
				auto line_current=m_position/n_cols;
				auto position=((line_current + 1)*n_cols)%size();
				if(position==0)
					{m_full=1;}
				m_position=position;
				if(m_full && position%n_cols==0)
					{scroll_down();}
				}



			std::unique_ptr<GeoSIMD::Point<float>[]> m_vertices;
			std::unique_ptr<uint32_t[]> m_colors;
			std::unique_ptr<vec2_t<float>[]> m_uvs;
			std::unique_ptr<FaceIndirect<>[]> m_faces;

			uint32_t m_n_cols;
			uint32_t m_n_rows;

			uint8_t m_color;
			size_t m_position;
			size_t m_line_current;
			uint32_t m_utf8_state;
			uint32_t m_codepoint;
			bool m_full;
			bool m_scroll_pending;

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

