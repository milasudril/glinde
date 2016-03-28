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

Mesh::Mesh(DataSource&& source)
	{
	ResourceObject data_raw(std::move(source));
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

		m_faces=ArraySimple<unsigned int>(N);
		for(unsigned int k=0;k<N;++k)
			{
			auto vi=narrow_cast<unsigned int>
				( static_cast<long long int>(faces.objectGet(k)) );
			GLINDA_DEBUG_PRINT("%s: Got vertex index %u",source.nameGet(),vi);
			vi_max=std::max(vi,vi_max);
			m_faces[k]=vi;
			}
		}

		{
		auto frames=data_raw.objectGet("frames");
		auto N=narrow_cast<unsigned int>(frames.objectCountGet());
		GLINDA_DEBUG_PRINT("%s: Got %u frames.",source.nameGet(),N);
		m_frames=ArraySimple<Frame>(N);
		for(unsigned int k=0;k<N;++k)
			{
			auto frame_current=&m_frames[k];
			auto frame_current_in=frames.objectGet(k);

			//	Load vertices
				{
				auto vertices=frame_current_in.objectGet("vertices");
				auto n=narrow_cast<unsigned int>( vertices.objectCountGet() );
				if(n%3!=0)
					{
					throw ErrorMessage("%s: The number of elements in the vertex "
						"array must be divisible by 3.",source.nameGet());
					}

				if(n/3 > vi_max + 1)
					{
					throw ErrorMessage("%s: Mesh contains unconnected vertices. %u > %u"
						,source.nameGet(),n/3,vi_max);
					}

				if(n/3 < vi_max + 1)
					{
					throw ErrorMessage("%s: Mesh non-existent vertices. %u > %u"
						,source.nameGet(),n/3,vi_max);
					}

				GLINDA_DEBUG_PRINT("%s: Got %u vertices.",source.nameGet(),n/3);
				frame_current->m_vertices=ArraySimple<float>(n);
				for(unsigned int l=0;l<n;++l)
					{
					frame_current->m_vertices[l]=static_cast<float>
						(static_cast<double>(vertices.objectGet(l)));
					}
				}

			//	Load UV coordinates
				{
				auto uv=frame_current_in.objectGet("uv");
				auto n=narrow_cast<unsigned int>( uv.objectCountGet() );
				if(n%2!=0)
					{
					throw ErrorMessage("%s: The number of elements in the UV "
						"array must be divisible by 2.",source.nameGet());
					}

				if(n/2 != frame_current->m_vertices.length()/3 )
					{
					throw ErrorMessage("%s: The number of UV coordinates has to "
						"be equal to the number of vertices."
						,source.nameGet());
					}

				GLINDA_DEBUG_PRINT("%s: Got %u UV coordinates.",source.nameGet(),n/2);
				frame_current->m_uv=ArraySimple<float>(n);
				for(unsigned int l=0;l<n;++l)
					{
					frame_current->m_uv[l]=static_cast<float>
						(static_cast<double>(uv.objectGet(l)));
					}
				}

			//	Load vertex normals
				{
				auto normals=frame_current_in.objectGet("normals");
				auto n=narrow_cast<unsigned int>( normals.objectCountGet() );
				if(n%3!=0)
					{
					throw ErrorMessage("%s: The number of elements in the "
						"normal array must be divisible by 3.",source.nameGet());
					}

				if(n != frame_current->m_vertices.length() )
					{
					throw ErrorMessage("%s: The number of normal vectors has to "
						"be equal to the number of vertices."
						,source.nameGet());
					}

				GLINDA_DEBUG_PRINT("%s: Got %u vertex normals coordinates.",source.nameGet(),n/3);
				frame_current->m_normals=ArraySimple<float>(n);
				for(unsigned int l=0;l<n;++l)
					{
					frame_current->m_normals[l]=static_cast<float>
						(static_cast<double>(normals.objectGet(l)));
					}
				}
			}
		}


	}
