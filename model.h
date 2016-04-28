#ifdef __WAND__
target[name[model.h] type[include]]
dependency[model.o]
#endif

#ifndef GLINDA_MODEL_H
#define GLINDA_MODEL_H

#include "arraysimple.h"
#include "mesh.h"
#include <map>

namespace Glinda
	{
	class DataSource;
	class TextureManager;
	class Mesh;
	class Stringkey;

	class Model
		{
		public:
			Model(TextureManager& textures,DataSource&& source):
				Model(textures,source)
				{}

			Model(TextureManager& textures,DataSource& source);
			~Model();

			size_t frameIndexGet(const char* key) const;

			Range< const Mesh* > frameGet(size_t index) const noexcept
				{
				assert(index < m_frames.length());
				return m_frames[index];
				}

		private:

		//	FIXME: If this is a bottleneck (double derererence)
			ArraySimple< ArraySimple<Mesh> > m_frames;

			std::map<Stringkey,size_t> m_frame_tags;
		};
	}

#endif
