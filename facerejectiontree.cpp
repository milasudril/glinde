#ifdef __WAND__
target[name[facerejectiontree.o] type[object]]
#endif

#include "facerejectiontree.h"
#include "arraydynamic.h"
#include "model.h"
#include "narrow_cast.h"
#include "face.h"
#include <algorithm>

using namespace Glinde;
static constexpr unsigned int CHILD_COUNT=8;

struct FaceRejectionTree::Node
	{
	Node()=default;
	Node(const Node&)=delete;
	Node& operator=(const Node&)=delete;
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

static ArrayDynamic< Face >
facesCollect(const Range<const Mesh>& meshes)
	{
	ArrayDynamic<Face> ret;
	auto mesh_current=meshes.begin();
	auto end=meshes.end();
	while(mesh_current!=end)
		{
		auto face_current=mesh_current->facesBegin();
		auto face_end=mesh_current->facesEnd();
		while(face_current!=face_end)
			{
			ret.append(*face_current);
			++face_current;
			}
		++mesh_current;
		}

	return ret;
	}

ArrayDynamic<unsigned int> facesFind(const Range<const unsigned int>& face_indices
	,const BoundingBox& box
	,const Range< const Face >& faces)
	{
	ArrayDynamic<unsigned int> ret;

	auto face_index=face_indices.begin();
	auto face_index_end=face_indices.end();
	auto f=faces.begin();

	auto boxFaces=facesGet(box);

	while(face_index!=face_index_end)
		{
		auto index=*face_index;
		assert(index < faces.length());

	//	TODO add line cross check
		if(insideAny(f[index],box) || intersect(f[index],boxFaces))
			{
			ret.append(index);
			}
		++face_index;
		}

	return ret;
	}

FaceRejectionTree::Node* FaceRejectionTree::nodeCreate(
	 const Range<const unsigned int>& face_indices
	,const BoundingBox& box
	,unsigned int depth
	,const Range<const Face>& faces)
	{
	auto size=0.5f*glm::vec4(box.size(),0.0f);

//	Terminate if the box becomes too small
	if(std::max(size.x,std::max(size.y,size.z)) < 4.8828125e-04f)
//	if(std::max(size.x,std::max(size.y,size.z)) < 2.0f)
//	if(depth>1)
		{return nullptr;}

	auto faces_in_box=::facesFind(face_indices,box,faces);
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
			  box.min + glm::vec4(0,size.y,0.0f,0.0f)
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
		ret->children[k]=nodeCreate(faces_in_box,boxes[k],depth+1,faces);
		}

	ret->faces=std::move(faces_in_box);
	return ret;
	}

void FaceRejectionTree::treeBuild(const Model::Frame& frame)
	{
	m_faces=facesCollect(frame.meshes);

	auto bb=frame.bounding_box;
	auto mid=frame.bounding_box.mid();

	auto bb_min=2.0f*(bb.min - mid) + mid;
	auto bb_max=2.0f*(bb.max - mid) + mid;

	bb={bb_min,bb_max};

	ArraySimple<unsigned int> face_indices_init(m_faces.length()
		,[](size_t k)
			{return narrow_cast<unsigned int>(k);}
		);

	m_root=nodeCreate(face_indices_init,bb,0,m_faces);
	}

FaceRejectionTree::FaceRejectionTree(const Model& model,size_t frame)
	{
	treeBuild(model.frameGet(frame));
	}

FaceRejectionTree::~FaceRejectionTree()
	{
	delete m_root;
	}

Twins<FaceRejectionTree::FaceIterator> FaceRejectionTree::facesFind(const glm::vec4& position
	,const glm::vec3& size_min) const noexcept
	{
	auto& node=nodeFind(*m_root,position,size_min);
	return Twins<FaceIterator>
		{
		 FaceIterator(node.faces.begin(),m_faces.begin())
		,FaceIterator(node.faces.end(),m_faces.begin())
		};
	}

const FaceRejectionTree::Node& FaceRejectionTree::nodeFind(const Node& root
	,const glm::vec4& position,const glm::vec3& size_min) noexcept
	{
	auto size_root=root.box.size();
//	Return when we reach minimum size. This is because we want to avoid objects
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
			if(inside(position,child_current->box))
				{
				return nodeFind(*child_current,position,size_min);
				}
			}
		}

	return root;
	}
