#ifdef __WAND__
target[name[facerejectiontree.o] type[object]]
#endif

#include "facerejectiontree.h"
#include "arraydynamic.h"
#include "model.h"
#include "narrow_cast.h"
#include <algorithm>

using namespace Glinda;
static constexpr unsigned int CHILD_COUNT=8;

struct FaceRejectionTree::Node
	{
	Node* children[CHILD_COUNT];
	ArrayDynamic<unsigned int> faces;
	BoundingBox box;

	bool leafIs() const noexcept
		{
		return children[0]==nullptr && children[1]==nullptr
			&& children[2]==nullptr && children[3]==nullptr
			&& children[4]==nullptr && children[5]==nullptr
			&& children[6]==nullptr && children[7]==nullptr;
		}

	~Node()
		{
		for(unsigned int k=0;k<CHILD_COUNT;++k)
			{delete children[k];}
		}
	};

BoundingBox boundingBoxGet(const Range<const Mesh*> meshes)
	{
	auto mesh_current=meshes.begin();
	auto end=meshes.end();
	auto ret=mesh_current->boundingBoxGet();
	++mesh_current;
	while(mesh_current!=end)
		{
		auto& bb_current=mesh_current->boundingBoxGet();
		ret=
			{
			 glm::min(ret.min,bb_current.min)
			,glm::max(ret.max,bb_current.max)
			};
		++mesh_current;
		}

	return ret;
	}

static ArrayDynamic< vec4_t<unsigned int> >
facesCollect(const Range<const Mesh*>& meshes)
	{
	ArrayDynamic< vec4_t<unsigned int> > ret;
	auto mesh_current=meshes.begin();
	auto end=meshes.end();
	unsigned int k=0;
	while(mesh_current!=end)
		{
		auto faces=mesh_current->facesGet();
		auto face_current=faces.begin();
		auto face_end=faces.end();
		while(face_current!=face_end)
			{
			ret.append(vec4_t<unsigned int>
				{
				 (*face_current)[0],(*face_current)[1],(*face_current)[2]
				,k
				});
			++face_current;
			}
		++k;
		++mesh_current;
		}

	return ret;
	}


struct FaceRejectionTree::ConstructionParams
	{
	Range< const vec4_t<unsigned int>* > faces;
	Range<const Mesh*> meshes;
	};

ArrayDynamic<unsigned int>
FaceRejectionTree::facesFind(const Range<const unsigned int*>& face_indices
	,const ConstructionParams& params
	,const BoundingBox& box)
	{
	ArrayDynamic<unsigned int> ret;
//	GLINDA_DEBUG_PRINT("Number of faces: %u",face_indices.length());

	auto face_index=face_indices.begin();
	auto face_index_end=face_indices.end();
	auto faces=params.faces.begin();
	auto meshes=params.meshes.begin();
	while(face_index!=face_index_end)
		{
		auto index=*face_index;
		assert(index < params.faces.length());
		auto v=faces[index];
		auto vertices=meshes[ v[3] ]
			.verticesFromFaceGet(Mesh::Face{v[0],v[1],v[2]});

	//	GLINDA_DEBUG_PRINT("  Hit test face %u@%u",face,3*face);
		if(insideAny(vertices,box))
			{ret.append(index);}
		++face_index;
		}

	return ret;
	}

FaceRejectionTree::Node* FaceRejectionTree::nodeCreate(
	 const Range<const unsigned int*>& face_indices
	,const BoundingBox& box
	,const ConstructionParams& params)
	{
/*	GLINDA_DEBUG_PRINT("Building FaceRejectionTree for box "
		"(%.8g,%.8g,%.8g) - (%.8g,%.8g,%.8g)"
		,box.min.x,box.min.y,box.min.z
		,box.max.x,box.max.y,box.max.z);*/
	auto size=0.5f*glm::vec4(box.size(),0.0f);

//	Terminate if the box becomes too small
	if(std::max(size.x,std::max(size.y,size.z)) < 4.8828125e-04f)
		{return nullptr;}

	auto faces_in_box=facesFind(face_indices,params,box);
//	Terminate recursion if there are no faces left
	if(faces_in_box.length()==0)
		{return nullptr;}

	auto ret=new Node;
	ret->box=box;

//	Create all subvolumes
	BoundingBox boxes[CHILD_COUNT]
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

	for(unsigned int k=0;k<CHILD_COUNT;++k)
		{
		ret->children[k]=nodeCreate(faces_in_box,boxes[k],params);
		}

	ret->faces=std::move(faces_in_box);
	return ret;
	}

void FaceRejectionTree::treeBuild()
	{
	auto bb=boundingBoxGet(r_meshes);
	m_face_list=facesCollect(r_meshes);

	auto mid=bb.mid();

	auto bb_min=2.0f*(bb.min - mid) + mid;
	auto bb_max=2.0f*(bb.max - mid) + mid;
	bb={bb_min,bb_max};

	ArraySimple<unsigned int> face_indices_init(m_face_list.length()
		,[](size_t k)
			{return narrow_cast<unsigned int>(k);}
		);

	m_root=nodeCreate(face_indices_init,bb,{m_face_list,r_meshes});
	}

FaceRejectionTree::FaceRejectionTree(const Model& model,size_t frame):
	r_meshes(model.frameGet(frame))
	{
	treeBuild();
	}

FaceRejectionTree::~FaceRejectionTree()
	{
	delete m_root;
	}

/*const FaceRejectionTree::Node& FaceRejectionTree::nodeFind(const Node& root
	,const glm::vec4& position,const glm::vec3& size_min) noexcept
	{
	auto size_root=root.box.size();
//	Return when we reach maximum size. This is because we want to avoid objects
//	that span multiple sectors
	if(std::min(std::min(size_root.x,size_root.y),size_root.z)
		<= std::max(std::max(size_min.x,size_min.y),size_min.z))
		{
		return root;
		}

	for(unsigned int k=0;k<CHILD_COUNT;++k)
		{
		auto child_current=root.children[k];
		if(child_current!=nullptr)
			{
			if(child_current->box.inside(position))
				{
				return nodeFind(*child_current,position,size_min);
				}
			}
		}

	return root;
	}*/


/*const ArrayDynamic<unsigned int>& FaceRejectionTree::facesGet(const glm::vec4& position
	,const glm::vec3& size_min) const noexcept
	{
	return nodeFind(*m_root,position,size_min).faces;
	}*/
