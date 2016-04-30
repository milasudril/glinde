#ifdef __WAND__
target[name[mesh.o] type[object]]
#endif

#include "mesh.h"
#include "resourceobject.h"
#include "narrow_cast.h"
#include "texturemanager.h"
#include "twins.h"
#include "debug.h"
#include <glm/glm.hpp>

using namespace Glinda;

static ArraySimple<glm::vec3> verticesLoad(const ResourceObject& source
	,const char* source_name)
	{
	auto verts=source.objectGet("vertices");
	auto n_verts=verts.objectCountGet();
	if(n_verts%3!=0)
		{throw ErrorMessage("%s: Each vertex needs three components",source_name);}
	n_verts/=3;
	GLINDA_DEBUG_PRINT("Got %zu vertices",n_verts);
	return std::move(ArraySimple<glm::vec3>(n_verts,[&verts](size_t k)
		{
		glm::vec3 ret;
		for(int l=0;l<3;++l)
			{
			auto vertex_obj=verts.objectGet(3*k + l);
			ret[l]=static_cast<float>( static_cast<double>(vertex_obj) );
			}
		return ret;
		}));
	}

static ArraySimple<glm::vec3> normalsLoad(const ResourceObject& source
	,const char* source_name
	,size_t vertex_count)
	{
	auto normals=source.objectGet("normals");
	auto n_normals=normals.objectCountGet();
	if(n_normals%3!=0)
		{throw ErrorMessage("%s: Each normal needs three components",source_name);}
	n_normals/=3;
	if(n_normals!=vertex_count)
		{
		throw ErrorMessage("%s: There has to be exactly one normal vector per vertex. "
			"The number of normal vectors are %zu, and the number of vertices are %zu"
			,source_name,n_normals,vertex_count);
		}

	GLINDA_DEBUG_PRINT("Got %zu normals",n_normals);

	return std::move(ArraySimple<glm::vec3>(n_normals,[&normals](size_t k)
		{
		glm::vec3 ret;
		for(int l=0;l<3;++l)
			{
			auto normal_obj=normals.objectGet(3*k + l);
			ret[l]=static_cast<float>( static_cast<double>(normal_obj) );
			}
		return ret;
		}));
	}

static ArraySimple<glm::vec2> uvsLoad(const ResourceObject& source
	,const char* source_name
	,size_t vertex_count)
	{
	auto uvs=source.objectGet("uvs");
	auto n_uvs=uvs.objectCountGet();
	if(n_uvs%2!=0)
		{throw ErrorMessage("%s: Each UV coordinate needs two components",source_name);}
	n_uvs/=2;
	if(n_uvs!=vertex_count)
		{
		throw ErrorMessage("%s: There has to be exactly one UV coordinate per vertex. "
			"The number of UV coordinats are %zu, and the number of vertices are %zu"
			,source_name
			,n_uvs,vertex_count);
		}

	GLINDA_DEBUG_PRINT("Got %zu uv coordinates",n_uvs);

	return std::move(ArraySimple<glm::vec2>(n_uvs,[&uvs](size_t k)
		{
		glm::vec2 ret;
		for(int l=0;l<2;++l)
			{
			auto uv_obj=uvs.objectGet(2*k + l);
			ret[l]=static_cast<float>( static_cast<double>(uv_obj) );
			}
	//	Invert y coordinate
		ret[1]=1.0f - ret[1];
		return ret;
		}));
	}

static ArraySimple<Mesh::FaceIndirect> facesLoad(const ResourceObject& source
	,const char* source_name
	,size_t vertex_count)
	{
	auto faces=source.objectGet("faces");
	auto n_faces=faces.objectCountGet();
	if(n_faces%Mesh::Face::length()!=0)
		{throw ErrorMessage("%s: Each face needs three vertices",source_name);}
	n_faces/=Mesh::Face::length();

	GLINDA_DEBUG_PRINT("Got %zu faces",n_faces);

	return std::move(ArraySimple<Mesh::FaceIndirect>(n_faces,[&faces,source_name,vertex_count](size_t k)
		{
		Mesh::FaceIndirect ret;
		for(size_t l=0;l<ret.length();++l)
			{
			auto face_obj=faces.objectGet(ret.length()*k + l);
			auto x=static_cast<long long int>(face_obj);
			if(x<0 || static_cast<size_t>(x)>=vertex_count)
				{throw ErrorMessage("%s: Invalid vertex index",source_name);}
			ret[l]=narrow_cast<unsigned int>(x);
			}
		return ret;
		}));
	}

static ArrayFixed<const Image*,Mesh::textureCountGet()>
texturesLoad(TextureManager& textures,const ResourceObject& source
	,const char* source_name)
	{
	GLINDA_DEBUG_PRINT("Loading textures for %s",source_name);
	auto textures_lump=source.objectGet("textures");
	auto n_textures=textures_lump.objectCountGet();
	if(n_textures>=Mesh::textureCountGet())
		{
		throw ErrorMessage("%s: A mesh can only have %zu different textures"
			,source_name,Mesh::textureCountGet());
		}

	ArrayFixed<const Image*,Mesh::textureCountGet()> ret;
	memset(ret.begin(),0,ret.length()*sizeof(const Image*));
	for(size_t k=0;k<n_textures;++k)
		{
		ret[k]=&textures.textureGet(source_name
			,static_cast<const char*>(textures_lump.objectGet(k)));
		}
	return std::move(ret);
	}



Mesh::Mesh(TextureManager& textures,const ResourceObject& source
	,const char* source_name):
	 m_vertices(verticesLoad(source,source_name))
	,m_normals(normalsLoad(source,source_name,m_vertices.length()))
	,m_uvs(uvsLoad(source,source_name,m_vertices.length()))
	,m_faces(facesLoad(source,source_name,m_vertices.length()))
	,r_textures(texturesLoad(textures,source,source_name))
	{
	boundingBoxUpdate();
	}

Mesh::~Mesh()
	{}

void Mesh::boundingBoxUpdate() noexcept
	{
	auto v=m_vertices.begin();
	auto v_end=m_vertices.end();

	BoundingBox ret
		{
		 glm::vec4(v[0],1.0f)
		,glm::vec4(v[0],1.0f)
		};

	while(v!=v_end)
		{
		glm::vec4 value=glm::vec4(*v,1.0f);
		ret.min=glm::min(value,ret.min);
		ret.max=glm::max(value,ret.max);

		++v;
		}

	m_box=ret;
	}

BoundingBox Glinda::boundingBoxGet(const Range<const Mesh*>& meshes)
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



inline static ArrayFixed<float,3> distancesGet(const glm::vec3& normal
	,const glm::vec3& v_0
	,const Mesh::Face& face)
	{
	ArrayFixed<float,face.length()> ret;
	for(size_t k=0;k<ret.length();++k)
		{
		ret[k]=glm::dot(normal,face[k]-v_0);
		ret[k]=std::abs(ret[k])>2.4414062e-04f?ret[k]:0.0f;
		}
	return ret;
	}

inline static ArrayFixed<float,3>
projectOnLine(const glm::vec3& direction,const Mesh::Face& face
	,const ArrayFixed<int,3>& order)
	{
	ArrayFixed<float,face.length()> ret;
	for(size_t k=0;k<ret.length();++k)
		{
		ret[k]=glm::dot(direction,face[order[k]]);
		}
	return ret;
	}


inline static ArrayFixed<int,3> vertexPermutationsGet(const ArrayFixed<float,3>& projections)
	{
//	v[0] and v[1] are on the same side. Use v[2] as the opposite vertex
	if(projections[0]*projections[1] > 0)
		{return {1,2,0};}

//	v[0] and v[2] are on the same side. Use v[1] as the opposite vertex
	if(projections[0]*projections[2] > 0)
		{return {0,1,2};}

//	v[2] and v[1] are on the same side. Use v[0] as the opposite vertex
	return {2,0,1};
	}

inline static Twins<float> lineSegment(const ArrayFixed<float,3>& t
	,const ArrayFixed<float,3>& pi
	,const ArrayFixed<int,3>& order)
	{
	Twins<float> ret
		{
		 (t[0]*pi[order[1]] - t[1]*pi[order[0]])/(pi[order[1]] - pi[order[0]])
		,(t[1]*pi[order[2]] - t[2]*pi[order[1]])/(pi[order[2]] - pi[order[1]])
		};
	if(ret.second<ret.first)
		{std::swap(ret.first,ret.second);}
	return ret;
	}

inline static Twins<float> linesegmentIntersection(const glm::vec3& direction
	,const Mesh::Face& a,const Mesh::Face& b,const glm::vec3& n_b)
	{
//	Get distances from the vertices to the plane of the other triangle
	auto distances=distancesGet(n_b,b[0],a);
//	All possible projections have the same sign. Therefore, all vertices of a
//	are on the same side of b. Conclude that there is no intersection.
//	Note: This is also the case for two coplanar triangles.
	if(distances[0]*distances[1]>=0.0f && distances[0]*distances[2]>=0)
		{return {0.0f,0.0f};}

//	Genereate a suitable permutation so element 1 refers to the opposite vertex
//	See fig (2.6) in
//	http://publications.lib.chalmers.se/records/fulltext/234473/234473.pdf
	auto permuts=vertexPermutationsGet(distances);
//	Generate the projection points (in order given by permuts)
	auto t=projectOnLine(direction,a,permuts);

//	Compute line segment between intersection points and opposite point
	return lineSegment(t,distances,permuts);
	}

inline static bool intersect(const Twins<float>& a,const Twins<float>& b)
	{
	if(a.second<=b.first + 2.4414062e-04f || b.second<=a.first + 2.4414062e-04f)
		{return 0;}
	return 1;
	}

bool Glinda::intersect(const Mesh::Face& a,const Mesh::Face& b)
	{
	auto n_a=glm::normalize(glm::cross(a[1]-a[0],a[2]-a[0]));
	auto n_b=glm::normalize(glm::cross(b[1]-b[0],b[2]-b[0]));

//	Direction of intersection line between the two planes
	auto direction=glm::cross(n_a,n_b);

//	First triangle
	auto L_1=linesegmentIntersection(direction,a,b,n_b);
	if(L_1.first==0.0f && L_1.second==0.0f)
		{return 0;}

//	Second triangle
	auto L_2=linesegmentIntersection(direction,b,a,n_a);
	if(L_2.first==0.0f && L_2.second==0.0f)
		{return 0;}

	if(::intersect(L_1,L_2))
		{return 1;}
	return 0;
	}

bool Glinda::intersect(const Mesh::Face& a,const Mesh& mesh)
	{
	auto i=mesh.facesBegin();
	auto i_end=mesh.facesEnd();
	while(i!=i_end)
		{
		if(intersect(a,*i))
			{return 1;}
		++i;
		}
	return 0;
	}

bool Glinda::intersect(const Mesh::Face& a,const Range<const Mesh*>& meshes)
	{
	auto i=meshes.begin();
	auto i_end=meshes.end();
	while(i!=i_end)
		{
		if(intersect(a,*i))
			{return 1;}
		++i;
		}
	return 0;
	}
