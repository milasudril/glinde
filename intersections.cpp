#ifdef __WAND__
target[name[intersections.o] type[object]]
#endif

#include "intersections.h"
#include "mesh.h"
#include "face.h"

using namespace Glinde;

bool Glinde::intersect(const Face& a,const Mesh& mesh) noexcept
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

bool Glinde::intersect(const Face& a,const Range<const Mesh>& meshes) noexcept
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


#if 0
static inline glm::vec3 intersect(const Line& line,const glm::vec3& plane_normal
	,const glm::vec4& plane_pos)
	{
//	Equation for the line
//	r=(1-t) * (x_0,y_0,z_0) + t*(x_1,y_1,z_1)
//	 =(x_0,y_0,z_0) - t*x_0-t*y_0-t*z_0 + t*x_1 + t*y_1 + t*z_1
//	 <=> r + t*(x_0 - x_1 ,y_0 - y_1, z_0 - z_1) = (x_0,y_0,z_0)

//	Equation for the plane
//	dot(n,r-p) = 0
//	<=>x*n_x + y*n_y + z*n_z = n_x*p_x + n_y*p_y + n_z*p_z

//	System of equations
//	x*n_x + y*n_y + z*n_z = n_x*p_x + n_y*p_y + n_z*p_z
//	x = x_0 - t*x_0 + t*x_1
//	y = y_0 - t*y_0 + t*y_1
//	z = z_0 - t*z_0 + t*z_1

//	x*n_x + y*n_y + z*n_z = n_x*p_x + n_y*p_y + n_z*p_z
//	x + t*(x_0 - x_1) = x_0
//	y + t*(y_0 - y_1) = y_0
//	z + t*(z_0 - z_1) = z_0

//	Matrix:
//	n_x  n_y  n_z   0 = A
//  1    0    0   -dx = x_0
//  0    1    0   -dy = y_0
//  0    0    1   -dz = z_0

//	Solve for t...

	auto dl=glm::vec3(line.to - line.from);
	auto proj=glm::dot(dl,plane_normal);
	if(std::abs(proj)<2.4414062e-04f)
		{
		parallell
		}

	auto t=dot(glm::vec3(pos_plane-line.from),plane_normal)/proj;
	auto intersect=t*dl + glm::vec3(line.from);

	}
#endif
