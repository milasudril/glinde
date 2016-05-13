#ifdef __WAND__
target[name[mesh.o] type[object]]
#endif

#include "mesh.h"
#include "resourceobject.h"
#include "narrow_cast.h"
#include "resourcemanager.h"
#include "twins.h"
#include "debug.h"
#include <glm/glm.hpp>

using namespace Glinde;

static ArraySimple<glm::vec3> verticesLoad(const ResourceObject& source
	,const char* source_name)
	{
	auto verts=source.objectGet("vertices");
	auto n_verts=verts.objectCountGet();
	if(n_verts%3!=0)
		{throw ErrorMessage("%s: Each vertex needs three components",source_name);}
	n_verts/=3;
	GLINDE_DEBUG_PRINT("Got %zu vertices",n_verts);
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

	GLINDE_DEBUG_PRINT("Got %zu normals",n_normals);

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

	GLINDE_DEBUG_PRINT("Got %zu uv coordinates",n_uvs);

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
	if(n_faces%Mesh::FaceIndirect::length()!=0)
		{throw ErrorMessage("%s: Each face needs three vertices",source_name);}
	n_faces/=Mesh::FaceIndirect::length();

	GLINDE_DEBUG_PRINT("Got %zu faces",n_faces);

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
texturesLoad(ResourceManager& textures,const ResourceObject& source
	,const char* source_name)
	{
	GLINDE_DEBUG_PRINT("Loading textures for %s",source_name);
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



Mesh::Mesh(ResourceManager& textures,const ResourceObject& source
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

BoundingBox Glinde::boundingBoxGet(const Range<const Mesh*>& meshes) noexcept
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

