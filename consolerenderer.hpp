//@	{
//@	 "dependencies_extra":[{"ref":"consolerenderer.o","rel":"implementation"}]
//@	,"targets":[{"name":"consolerenderer.hpp","type":"include"}]
//@	}

#ifndef GLINDE_CONSOLERENDERER_HPP
#define GLINDE_CONSOLERENDERER_HPP

#include "angle/texture2d.hpp"
#include "angle/program.hpp"

namespace Glinde
	{
	class Console;
	class Image;
	class ConsoleRenderer
		{
		public:
			ConsoleRenderer(const Image& charmap);
			void render(const Console& con) noexcept;

		private:
			Angle::Program m_program;
			Angle::Texture2D m_charmap;
		};
	}

#endif
