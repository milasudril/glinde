//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"console.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"console.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_CONSOLE_H
#define GLINDE_CONSOLE_H

#include "image.h"
#include "logwriter.h"
#include "faceindirect.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Glinde
	{
	class Console
		{
		public:
			static constexpr size_t CHARCELL_WIDTH=9;
			static constexpr size_t CHARCELL_HEIGHT=16;

			Console(uint32_t n_rows,uint32_t n_cols);

			Console& colorMaskSet(uint8_t color_mask);
			Console& writeRaw(const char* string);
			Console& writeUTF8(const char* string);
			Console& write(char ch);
			Console& write(uint32_t codepoint);

			size_t colsCountGet() const noexcept
				{return m_n_cols;}

			size_t rowsCountGet() const noexcept
				{return m_vertices.length()/(4*m_n_cols);}



			Range<const glm::vec3> verticesGet() const noexcept
				{return m_vertices;}

			Range<const glm::vec3> verticesGet(size_t k) const noexcept
				{return rowGet(m_vertices.begin(),k,4);}



			Range<const glm::vec3> colorsFgGet() const noexcept
				{return m_colors_fg;}

			Range<const glm::vec3> colorsFgGet(size_t k) const noexcept
				{return rowGet(m_colors_fg.begin(),k,4);}



			Range<const glm::vec3> colorsBgGet() const noexcept
				{return m_colors_bg;}

			Range<const glm::vec3> colorsBgGet(size_t k) const noexcept
				{return rowGet(m_colors_bg.begin(),k,4);}



			Range<const glm::vec2> uvsGet() const noexcept
				{return m_uvs;}

			Range<const glm::vec2> uvsGet(size_t k) const noexcept
				{return rowGet(m_uvs.begin(),k,4);}



			Range<const FaceIndirect> facesGet() const noexcept
				{return m_faces;}

			Range<const FaceIndirect> facesGet(size_t k) const noexcept
				{return rowGet(m_faces.begin(),k,2);}

			float lineOffsetGet(size_t k) const noexcept
				{
				auto n_rows=rowsCountGet();
				return -static_cast<float>(2*k)/static_cast<float>(n_rows);
				}

			const Image& fontmapGet() const noexcept
				{return m_fontmap;}

			size_t windowWidthGet() const noexcept
				{return CHARCELL_WIDTH*colsCountGet();}

			size_t windowHeightGet() const noexcept
				{return CHARCELL_HEIGHT*rowsCountGet();}

		private:
			Image m_fontmap;
			ArraySimple<glm::vec3> m_vertices;
			ArraySimple<glm::vec3> m_colors_fg;
			ArraySimple<glm::vec3> m_colors_bg;
			ArraySimple<glm::vec2> m_uvs;
			ArraySimple<FaceIndirect> m_faces;
			size_t m_n_cols;
			glm::vec3 m_color_fg;
			glm::vec3 m_color_bg;
			size_t m_position;
			size_t m_line_current;
			uint32_t m_utf8_state;
			uint32_t m_codepoint;

			template<class T>
			Range<const T> rowGet(const T* what,size_t k,size_t count) const noexcept
				{
				auto N=colsCountGet()*count;
				auto n_rows=rowsCountGet();
				return Range<const T>
					{
 					 what+N*( (k + m_line_current)%n_rows)
					,N
					};
				}
		};
	}

#endif

