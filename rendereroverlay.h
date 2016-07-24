//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"rendereroverlay.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"rendereroverlay.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_RENDEREROVERLAY_H
#define GLINDE_RENDEREROVERLAY_H

namespace Glinde
	{
	class GlTexture;
	class RendererOverlay
		{
		public:
			RendererOverlay();
			~RendererOverlay();
			void render(const GlTexture& texture) noexcept;
			void viewportSizeSet(int width,int height) noexcept;
		};
	}

#endif
