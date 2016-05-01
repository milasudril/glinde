#ifdef __WAND__
target[name[intersections.o] type[object]]
#endif

#include "intersections.h"
#include "mesh.h"
#include "face.h"

using namespace Glinda;

bool Glinda::intersect(const Face& a,const Mesh& mesh)
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

bool Glinda::intersect(const Face& a,const Range<const Mesh*>& meshes)
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
