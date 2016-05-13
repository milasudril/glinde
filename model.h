#ifdef __WAND__
target[name[model.h] type[include]]
dependency[model.o]
#endif

#ifndef GLINDE_MODEL_H
#define GLINDE_MODEL_H

#include "arraysimple.h"
#include "mesh.h"
#include <map>

namespace Glinde
	{
	class DataSource;
	class ResourceManager;
	class Stringkey;

	class Model
		{
		public:
			struct Frame
				{
				ArraySimple<Mesh> meshes;
				BoundingBox bounding_box;
				};

			Model(ResourceManager& rc,DataSource&& source):
				Model(rc,source)
				{}

			Model(ResourceManager& rc,DataSource& source);
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
