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
//@  "targets":[{"name":"resourcemanager.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"resourcemanager.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_RESOURCEMANAGER_HPP
#define GLINDE_RESOURCEMANAGER_HPP

#include "image.hpp"
#include "stringkey.hpp"
#include "texture_upload.hpp"
#include "scene/camerabuffer.hpp"
#include "storage/lookuptable.hpp"

namespace Glinde
	{
	class ResourceManager
		{
		public:
			ResourceManager();
			~ResourceManager();

			Image& image(const char* name);
			Angle::Texture2D& texture(const char* name);
			CameraBuffer& camera(const char* name);

		private:
			LookupTable<Stringkey,Image> m_images;
			LookupTable<Stringkey,Angle::Texture2D> m_textures;
			LookupTable<Stringkey,CameraBuffer> m_cambuffers;
		};
	}

#endif // GLINDE_RESOURCEMANAGER_HPP
