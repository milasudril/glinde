#ifdef __WAND__
target[name[intersections.h] type[include]]
dependency[intersections.o]
#endif

#ifndef GLINIE_INTERSECTIONS_H
#define GLINIE_INTERSECTIONS_H

namespace Glinie
	{
	class Mesh;
	class Face;
	class BoundingBox;
	class Line;
	template<class T> class Range;

	bool intersect(const Face& a,const Mesh& mesh) noexcept;
	bool intersect(const Face& a,const Range<const Mesh*>& meshes) noexcept;
	}

#endif
