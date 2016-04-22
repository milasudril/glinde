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

static bool insideIs(const float* verts,const unsigned int* vi
	,const BoundingBox& box)
	{
	for(int k=0;k<3;++k)
		{
		auto v=&verts[vi[k]];
		if(box.inside(glm::vec4{v[0],v[1],v[2],1.0f}))
			{return 1;}
		}
	return 0;
	}

static ArrayDynamic<unsigned int>
facesFind(const unsigned int* face_indices,unsigned int N_faces_indices
 	,const unsigned int* vi,const float* vertices,const BoundingBox& box)
	{
	ArrayDynamic<unsigned int> ret;
	while(N_faces_indices!=0)
		{
		auto face=*face_indices;
		if(insideIs(vertices,&vi[face],box))
			{
			ret.append(face);
			}
		--N_faces_indices;
		}

	return ret;
	}

FaceRejectionTree::Node* FaceRejectionTree::nodeCreate(
	 const unsigned int* face_indices
	,unsigned int N_faces_indices,const unsigned int* vi
	,const float* vertices,const BoundingBox& box)
	{
	auto size=0.5f*glm::vec4(box.size(),0.0f);
//	Terminate if the box becomes too small
	if(std::max(size.x,std::max(size.y,size.z)) < 2.0f)
		{return nullptr;}

	auto faces_in_box=facesFind(face_indices,N_faces_indices,vi,vertices,box);
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
		GLINDA_DEBUG_PRINT("%d Enter node (%.8g, %.8g, %.8g) - (%.8g, %.8g, %.8g)"
			,k
			,boxes[k].min.x,boxes[k].min.y,boxes[k].min.z
			,boxes[k].max.x,boxes[k].max.y,boxes[k].max.z);
		ret->children[k]=nodeCreate(faces_in_box.begin(),faces_in_box.length()
			,vi,vertices,boxes[k]);
		}
	if(ret->leafIs())
		{
		GLINDA_DEBUG_PRINT("Leaf (%.8g, %.8g, %.8g) - (%.8g, %.8g, %.8g)"
			,box.min.x,box.min.y,box.min.z
			,box.max.x,box.max.y,box.max.z);
		ret->faces=std::move(faces_in_box);
		}

	return ret;
	}


void FaceRejectionTree::treeBuild(const Mesh& mesh)
	{
	auto bb=mesh.boundingBoxGet(0);

	auto mid=bb.mid();

	auto bb_min=2.0f*(bb.min - mid) + mid;
	auto bb_max=2.0f*(bb.max - mid) + mid;
	bb={bb_min,bb_max};

	ArraySimple<unsigned int> face_indices_init(mesh.m_faces.length()/3
		,[](size_t k)
			{return static_cast<unsigned int>(k);}
		);
	root=nodeCreate(face_indices_init.begin()
		,static_cast<unsigned int>( face_indices_init.length() )		,mesh.m_faces.begin(),mesh.m_frames[0].m_vertices.begin(),bb);
	}
