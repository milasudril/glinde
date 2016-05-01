#ifdef __WAND__
target[name[face.o] type[object]]
#endif

#include "face.h"
#include "twins.h"
#include <glm/glm.hpp>

using namespace Glinda;

inline static ArrayFixed<float,3> distancesGet(const glm::vec3& normal
	,const glm::vec3& v_0
	,const Face& face)
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
projectOnLine(const glm::vec3& direction,const Face& face
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
		{return ArrayFixed<int,3>{1,2,0};}

//	v[0] and v[2] are on the same side. Use v[1] as the opposite vertex
	if(projections[0]*projections[2] > 0)
		{return ArrayFixed<int,3>{0,1,2};}

//	v[2] and v[1] are on the same side. Use v[0] as the opposite vertex
	return ArrayFixed<int,3>{2,0,1};
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
	,const Face& a,const Face& b,const glm::vec3& n_b)
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

bool Glinda::intersect(const Face& a,const Face& b)
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
