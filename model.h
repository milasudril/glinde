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
	class Stringkey;

	class Model
		{
		public:
			struct Frame
				{
				ArraySimple<Mesh> meshes;
				BoundingBox bounding_box;
				};

			Model(TextureManager& textures,DataSource&& source):
				Model(textures,source)
				{}

			Model(TextureManager& textures,DataSource& source);
			~Model();

			size_t frameIndexGet(const Stringkey& key) const;

			const Frame& frameGet(size_t index) const noexcept
				{
				assert(index < m_frames.length());
				return m_frames[index];
				}

		private:
			ArraySimple< Frame > m_frames;
			std::map<Stringkey,size_t> m_frame_tags;
		};
	}

#endif
