#ifdef __WAND__
target[name[mesh.o] type[object]]
#endif

#include "mesh.h"
#include "datasource.h"
#include "resourceobject.h"
#include "debug.h"
#include "errormessage.h"
#include "logwriter.h"
#include "narrow_cast.h"

using namespace Glinda;

static ArraySimple<float> verticesGet(const char* source_name
	,ResourceObject&& vertices
	,unsigned int vi_max)
	{
	auto n=narrow_cast<unsigned int>( vertices.objectCountGet() );
	if(n%3!=0)
		{
		throw ErrorMessage("%s: The number of elements in the vertex "
			"array must be divisible by 3.",source_name);
		}

	if(n/3 > vi_max + 1)
		{
		throw ErrorMessage("%s: Mesh contains unconnected vertices. %u > %u"
			,source_name,n/3,vi_max);
		}

	if(n/3 < vi_max + 1)
		{
		throw ErrorMessage("%s: Mesh non-existent vertices. %u > %u"
			,source_name,n/3,vi_max);
		}

	GLINDA_DEBUG_PRINT("%s: Got %u vertices.",source_name,n/3);

	return std::move(ArraySimple<float>
		{
		n,[&vertices](size_t k)
			{
			return static_cast<float>
				(static_cast<double>(vertices.objectGet(k)));
			}
		});
	}

static ArraySimple<float> uvGet(const char* source_name
	,ResourceObject&& uv
	,size_t vertex_count)
	{
	auto n=narrow_cast<unsigned int>( uv.objectCountGet() );
	if(n%2!=0)
		{
		throw ErrorMessage("%s: The number of elements in the UV "
			"array must be divisible by 2.",source_name);
		}

	if(n/2 != vertex_count/3 )
		{
		throw ErrorMessage("%s: The number of UV coordinates has to "
			"be equal to the number of vertices."
			,source_name);
		}

	GLINDA_DEBUG_PRINT("%s: Got %u UV coordinates.",source_name,n/2);
	return std::move(ArraySimple<float>
		{
		n,[&uv](size_t k)
			{
			return static_cast<float>
				(static_cast<double>(uv.objectGet(k)));
			}
		});
	}

static ArraySimple<float> normalsGet(const char* source_name
	,ResourceObject&& normals
	,size_t vertex_count)
	{
	auto n=narrow_cast<unsigned int>( normals.objectCountGet() );
	if(n%3!=0)
		{
		throw ErrorMessage("%s: The number of elements in the "
			"normal array must be divisible by 3.",source_name);
		}

	if(n != vertex_count )
		{
		throw ErrorMessage("%s: The number of normal vectors has to "
			"be equal to the number of vertices."
			,source_name);
		}

	GLINDA_DEBUG_PRINT("%s: Got %u vertex normal vectors.",source_name,n/3);
	return std::move(ArraySimple<float>
		{
		n,[&normals](size_t k)
			{
			return static_cast<float>
				(static_cast<double>(normals.objectGet(k)));
			}
		});
	}


static Mesh::Frame frameGet(const char* source_name
	,ResourceObject&& frame_current_in
	,unsigned int vi_max)
	{
	auto v=verticesGet(source_name,frame_current_in.objectGet("vertices"),vi_max);
	auto vertex_count=v.length();
	return std::move(Mesh::Frame
		{
		 std::move(v)
		,uvGet(source_name,frame_current_in.objectGet("uv"),vertex_count)
		,normalsGet(source_name,frame_current_in.objectGet("normals"),vertex_count)
		});
	}


Mesh::Mesh(DataSource&& source)
	{
	ResourceObject data_raw(source);
	unsigned int vi_max=0;
		{
		auto faces=data_raw.objectGet("faces");
		auto N=narrow_cast<unsigned int>(faces.objectCountGet());
		if(N%3!=0)
			{
			throw ErrorMessage("%s: A mesh must consist of triangles only."
				,source.nameGet());
			}

		GLINDA_DEBUG_PRINT("%s: Got %u faces.",source.nameGet(),N);

		m_faces=ArraySimple<unsigned int>
			{
			N,[&vi_max,&faces](size_t k)
				{
				auto vi=narrow_cast<unsigned int>
					(static_cast<long long int>(faces.objectGet(k)) );
				vi_max=std::max(vi,vi_max);
				return vi;
				}
			};
		}

		{
		auto frames=data_raw.objectGet("frames");
		auto N=narrow_cast<unsigned int>(frames.objectCountGet());
		GLINDA_DEBUG_PRINT("%s: Got %u frames.",source.nameGet(),N);
		m_frames=ArraySimple<Frame>
			{
			N,[&source,&frames,vi_max](size_t k)
				{return frameGet(source.nameGet(),frames.objectGet(k),vi_max);}
			};
		}
	}
