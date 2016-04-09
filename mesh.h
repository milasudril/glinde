#ifdef __WAND__
target[name[mesh.h] type[include]]
dependency[mesh.o]
#endif

#ifndef GLINDA_MESH_H
#define GLINDA_MESH_H

#include "arraysimple.h"

namespace Glinda
	{
	class DataSource;
	class Image;
	class TextureManager;

	struct Mesh
		{
		Mesh(TextureManager& textures,DataSource&& source):Mesh(textures,source){}
		Mesh(TextureManager& textures,DataSource& source);

		ArraySimple<unsigned int> m_faces;

		struct Frame
			{
			ArraySimple<float> m_vertices;
			ArraySimple<float> m_normals;
			ArraySimple<float> m_uv;
			const Image* r_tex_diffuse;
			};

		ArraySimple<Frame> m_frames;
		};
	}

#endif
