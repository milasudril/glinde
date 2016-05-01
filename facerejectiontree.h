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
#include "model.h"
#include "twins.h"
#include <glm/glm.hpp>

namespace Glinda
	{
	class Face;

	class FaceRejectionTree
		{
		public:
			class FaceIterator
				{
				public:
					FaceIterator& operator++() noexcept
						{
						++r_position;
						return *this;
						}

					FaceIterator operator++(int) noexcept
						{
						FaceIterator temp(*this);
						++r_position;
						return temp;
						}

					const Face& operator*() const noexcept
						{
						return r_faces[ *r_position ];
						}

					bool operator==(const FaceIterator& i) const noexcept
						{
						return r_position==i.r_position && r_faces==i.r_faces;
						}

					bool operator!=(const FaceIterator& i) const noexcept
						{
						return !(*this==i);
						}

				private:
					FaceIterator(const unsigned int* face_indices
						,const Face* faces) noexcept
						:r_position(face_indices),r_faces(faces)
						{}


					friend class FaceRejectionTree;
					const unsigned int* r_position;
					const Face* r_faces;
				};


			FaceRejectionTree& operator=(const FaceRejectionTree&)=delete;
			FaceRejectionTree(const FaceRejectionTree&)=delete;

			FaceRejectionTree(Model&& model,size_t frame)=delete;

			FaceRejectionTree(const Model& model,size_t frame);

			~FaceRejectionTree();

			Twins<FaceIterator> facesFind(const glm::vec4& position
				,const glm::vec3& size_min) const noexcept;

		private:
			class Node;
			Node* m_root;

			void treeBuild(const Model::Frame& frame);

			static Node* nodeCreate(
				 const Range<const unsigned int*>& face_indices
				,const BoundingBox& box
				,unsigned int depth
				,const Range<const Face*>& faces);

			ArrayDynamic< Face > m_faces;

			static const Node& nodeFind(const Node& root,const glm::vec4& position
				,const glm::vec3& size_min) noexcept;
		};
	}

#endif
