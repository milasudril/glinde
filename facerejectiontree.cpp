#ifdef __WAND__
target[name[facerejectiontree.o] type[object]]
#endif

#include "facerejectiontree.h"
#include "arraydynamic.h"
#include "mesh.h"
#include <algorithm>

using namespace Glinda;

struct FaceRejectionTree::Node
	{
	Node* children[8];
	ArrayDynamic<unsigned int> faces;
	glm::vec3 size;

	bool leafIs() const noexcept
		{
		return children[0]==nullptr && children[1]==nullptr
			&& children[2]==nullptr && children[3]==nullptr
			&& children[4]==nullptr && children[5]==nullptr
			&& children[6]==nullptr && children[7]==nullptr;
		}
	};

static ArrayDynamic<unsigned int>
facesFind(const unsigned int* face_indices,unsigned int N_faces_indices
 	,const unsigned int* faces,const float* vertices,const BoundingBox& box)
	{
	ArrayDynamic<unsigned int> ret;
	while(N_faces_indices!=0)
		{
		auto face=*face_indices;
		auto value=&vertices[faces[face]];
		if(box.inside(glm::vec4{value[0],value[1],value[2],1.0f}))
			{
			ret.append(face);
			}
		--N_faces_indices;
		}

	return ret;
	}

FaceRejectionTree::Node* FaceRejectionTree::nodeCreate(
	 const unsigned int* face_indices
	,unsigned int N_faces_indices,const unsigned int* faces
	,const float* vertices,const BoundingBox& box)
	{
	auto size=0.5f*glm::vec4(box.size(),0.0f);
//	Terminate if the box becomes too small
	if(std::max(size.x,std::max(size.y,size.z)) < 1.0e-4f)
		{return nullptr;}

	auto faces_in_box=facesFind(face_indices,N_faces_indices,faces,vertices,box);
//	Terminate recursion if there are no faces left
	if(faces_in_box.length()==0)
		{return nullptr;}

//	Create all subvolumes
	BoundingBox boxes[8]
		{
			 {box.min,box.min + size}
			,{
			  box.min + glm::vec4(size.x,0.0f,0.0f,0.0f)
			 ,box.min + size + glm::vec4(size.x,0.0f,0.0f,0.0f)
			 }
			,{
			  box.min + glm::vec4(0,size.y,0.0f,0.f)
			 ,box.min + size + glm::vec4(0,size.y,0.0f,0.0f)
			 }
			,{
			  box.min + glm::vec4(size.x,size.y,0.0f,0.0f)
			 ,box.min + size + glm::vec4(size.y,size.y,0.0f,0.0f)
			 }
			,{
			  box.min + glm::vec4(0.0f,0.0f,size.z,0.0f)
			 ,box.min + size + glm::vec4(0.0f,0.0f,size.z,0.0f)
			 }
			,{
			  box.min + glm::vec4(size.x,0.0f,size.z,0.0f)
			 ,box.min + size + glm::vec4(size.x,0,size.z,0.0f)
			 }
			,{
			  box.min + glm::vec4(0,size.y,size.z,0.0f)
			 ,box.min + size + glm::vec4(0,size.y,size.z,0.0f)
			 }
			,{box.min + size, box.min + 2.0f*size}
		};

	auto ret=new Node;

	for(int k=0;k<8;++k)
		{
		ret->children[k]=nodeCreate(faces_in_box.begin(),faces_in_box.length()
			,faces,vertices,boxes[k]);
		}
	if(ret->leafIs())
		{ret->faces=std::move(faces_in_box);}

	return ret;
	}


void FaceRejectionTree::treeBuild(const Mesh& mesh)
	{
	auto bb=mesh.boundingBoxGet(0);
	ArraySimple<unsigned int> face_indices_init(mesh.m_faces.length()
		,[](size_t k)
			{return static_cast<unsigned int>(k);}
		);
	root=nodeCreate(face_indices_init.begin()
		,static_cast<unsigned int>( face_indices_init.length() )		,mesh.m_faces.begin(),mesh.m_frames[0].m_vertices.begin(),bb);
	}
