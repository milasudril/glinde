#ifdef __WAND__
target[name[intersections.h] type[include]]
dependency[intersections.o]
#endif

#ifndef GLINDA_INTERSECTIONS_H
#define GLINDA_INTERSECTIONS_H

namespace Glinda
	{
	class Mesh;
	class Face;
	class BoundingBox;
	class Line;
	template<class T> class Range;

	bool intersect(const Face& a,const Mesh& mesh);
	bool intersect(const Face& a,const Range<const Mesh*>& meshes);
	}

#endif
