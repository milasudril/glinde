//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"consolerenderer.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"consolerenderer.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#include "glvertexarray.h"
#include "gltexture.h"
#include "glprogram.h"
#include "glvertexbuffer.h"
#include "glelementarraybuffer.h"
#include "glframebuffer.h"

namespace Glinde
	{
	class Console;
	class ConsoleRenderer
		{
		public:
			ConsoleRenderer(Console&&)=delete;
			~ConsoleRenderer();

			explicit ConsoleRenderer(const Console& console);
			void render() noexcept;
			const GlTexture& textureGet() const noexcept
				{return m_result;}

		private:
			const Console* r_console;

			GlVertexArray m_array;
			GlProgram m_program;
			GlTexture m_result;
			GlFramebuffer m_fb;

			GlVertexBuffer m_vbo;
			GlVertexBuffer m_uvs;
			GlVertexBuffer m_color_fg;
			GlVertexBuffer m_color_bg;
			GlElementArrayBuffer m_faces;
			GlTexture m_fontmap;
			GLuint m_fontmap_loc;
			GLuint m_voffset_loc;
		};
	}
