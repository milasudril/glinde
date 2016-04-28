#ifdef __WAND__
target[name[facerejectiontree.h] type[include]]
dependency[facerejectiontree.o]
#endif

/**\file facerejectiontree.h \brief Defines the FaceRejectionTree class
*/

#ifndef GLINDA_FACEREJECTIONTREE_H
#define GLINDA_FACEREJECTIONTREE_H

#include "arraydynamic.h"
#include "vectortype.h"
#include <glm/glm.hpp>

namespace Glinda
	{
	class Model;
	class Mesh;
	class BoundingBox;
	class FaceRejectionTree
		{
		public:
			FaceRejectionTree& operator=(const FaceRejectionTree&)=delete;
			FaceRejectionTree(const FaceRejectionTree&)=delete;

			FaceRejectionTree(Model&& model,size_t frame)=delete;

			FaceRejectionTree(const Model& model,size_t frame);

			~FaceRejectionTree();

		/*	const ArrayDynamic<unsigned int>& facesGet(const glm::vec4& position
				,const glm::vec3& size_min) const noexcept;*/

		private:
			class Node;
			Node* m_root;

			class ConstructionParams;

			void treeBuild();


			static Node* nodeCreate(const Range<const unsigned int*>& face_indices
				,const BoundingBox& box,const ConstructionParams& params);

			static ArrayDynamic<unsigned int>
			facesFind(const Range<const unsigned int*>& face_indices
				,const ConstructionParams& params,const BoundingBox& box);

			Range<const Mesh*> r_meshes;
			ArrayDynamic< vec4_t<unsigned int> > m_face_list;


		/*	static const Node& nodeFind(const Node& root,const glm::vec4& position
				,const glm::vec3& size_min) noexcept;*/
		};
	}

#endif
