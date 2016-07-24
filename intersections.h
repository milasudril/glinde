//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"intersections.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"intersections.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_INTERSECTIONS_H
#define GLINDE_INTERSECTIONS_H

namespace Glinde
	{
	class Mesh;
	class Face;
	class BoundingBox;
	class Line;
	template<class T> class Range;

	bool intersect(const Face& a,const Mesh& mesh) noexcept;
	bool intersect(const Face& a,const Range<const Mesh>& meshes) noexcept;
	}

#endif
