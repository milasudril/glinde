#ifdef __WAND__
target[name[mesh.h] type[include]]
dependency[mesh.o]
#endif

#ifndef GLINDA_MESH_H
#define GLINDA_MESH_H

#include "image.h"

namespace Glinda
	{
	class DataSource;

	struct Mesh
		{
		Mesh(DataSource&& source);

		ArraySimple<unsigned int> m_faces;

		struct Frame
			{
			ArraySimple<float> m_vertices;
			ArraySimple<float> m_normals;
			ArraySimple<float> m_uv;
			};

		ArraySimple<Frame> m_frames;
		};
	}

#endif
