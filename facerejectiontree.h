#ifdef __WAND__
target[name[facerejectiontree.h] type[include]]
dependency[facerejectiontree.o]
#endif

/**\file facerejectiontree.h \brief Defines the FaceRejectionTree class
*/

#ifndef GLINDA_FACEREJECTIONTREE_H
#define GLINDA_FACEREJECTIONTREE_H

namespace Glinda
	{
	class Mesh;
	class WorldObject;
	class BoundingBox;

	class FaceRejectionTree
		{
		public:
			FaceRejectionTree& operator=(const FaceRejectionTree&)=delete;
			FaceRejectionTree(const FaceRejectionTree&)=delete;

			FaceRejectionTree(Mesh&& mesh)
				{treeBuild(mesh);}

			FaceRejectionTree(const Mesh& mesh)
				{treeBuild(mesh);}

			~FaceRejectionTree();


			const unsigned int* facesGet(const WorldObject* object) const noexcept;


		private:
			class Node;
			Node* root;

			void treeBuild(const Mesh& mesh);
			Node* nodeCreate(const unsigned int* face_indices
				,unsigned int N_faces_indices
				,const unsigned int* faces
				,const float* vertices
				,const BoundingBox& box);
		};
	}

#endif
