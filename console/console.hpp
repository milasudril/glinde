//@	{
//@	 "dependencies_extra":[{"ref":"console.o","rel":"implementation"}]
//@	,"targets":[{"name":"console.hpp","type":"include"}]
//@	}
#ifndef GLINDE_CONSOLE_HPP
#define GLINDE_CONSOLE_HPP

#include "../faceindirect.hpp"
#include "../vectortype.hpp"
#include "../color.hpp"
#include "../memory/new.hpp"
#include <geosimd/point.hpp>
#include <memory>
#include <algorithm>

namespace Glinde
	{
	struct VGACell;

	class Console
		{
		public:
			Console(uint32_t n_rows,uint32_t n_cols);

			Console& colorMask(uint8_t color_mask) noexcept;
			Console& writeRaw(const char* string) noexcept;
			Console& writeUTF8(const char* string) noexcept;
			Console& write(char ch) noexcept;
			Console& write(uint32_t codepoint) noexcept;
			Console& writeVGADump(Range<const VGACell> dump) noexcept;

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



			auto colorsFg() const noexcept
				{return Range<Color>{m_colors_fg.get(),size()*4};}

			auto colorsFgFull() const noexcept
				{return Range<Color>{m_colors_fg.get(),sizeFull()*4};}

			auto colorsFg(size_t k) const noexcept
				{return row(m_colors_fg.get(),k,4);}



			auto colorsBg() const noexcept
				{return Range<Color>{m_colors_bg.get(),size()*4};}

			auto colorsBgFull() const noexcept
				{return Range<Color>{m_colors_bg.get(),sizeFull()*4};}

			auto colorsBg(size_t k) const noexcept
				{return row(m_colors_bg.get(),k,4);}



			auto uvs() const noexcept
				{return Range<const vec2_t<float>>{m_uvs.get(),size()*4};}

			auto uvsFull() const noexcept
				{return Range<const vec2_t<float>>{m_uvs.get(),sizeFull()*4};}

			auto uvs(size_t k) const noexcept
				{return row(m_uvs.get(),k,4);}



			auto faces() const noexcept
				{return Range<const FaceIndirect>{m_faces.get(),size()*2};}

			auto facesFull() const noexcept
				{return Range<const FaceIndirect>{m_faces.get(),sizeFull()*2};}

			Range<const FaceIndirect> faces(size_t k) const noexcept
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
					 static_cast<float>(2*col)/m_n_cols
					,m_full?-2.0f*(m_n_rows - 1.0f)/m_n_rows:-static_cast<float>(2*row)/m_n_rows
					};
				}

			size_t sizeFull() const noexcept
				{return size() + 1;}

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

