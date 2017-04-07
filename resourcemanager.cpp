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
//@	{"targets":[{"name":"resourcemanager.o","type":"object"}]}

#include "resourcemanager.hpp"
#include "io/filein.hpp"

using namespace Glinde;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

Image& ResourceManager::image(const char* name)
	{
	return m_images.find(Stringkey(name),[name]()
		{return Image(FileIn(name),0);});
	}

Angle::Texture2D& ResourceManager::texture(const char* name) 
	{
	return m_textures.find(Stringkey(name),[name,this]()
		{return texture2d(this->image(name));});
	}

CameraBuffer& ResourceManager::camera(const char* name)
	{
	return m_cambuffers.find(Stringkey(name),[]()
		{
		CameraBuffer ret;
		return std::move(ret);
		});
	}
