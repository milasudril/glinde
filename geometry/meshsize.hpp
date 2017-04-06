//@ {"targets":[{"name":"meshsize.hpp","type":"include"}]}

#ifndef GLINDE_MESHSIZE_HPP
#define GLINDE_MESHSIZE_HPP

namespace Glinde
	{
	template<class T>
	class MeshSize
		{
		public:
			explicit constexpr MeshSize(T vert_count,T face_count) noexcept:
				m_verts(vert_count),m_faces(face_count)
				{}

			constexpr T facesCount() const noexcept
				{return m_faces;}

			constexpr T verticesCount() const noexcept
				{return m_verts;}
		private:
			T m_verts;
			T m_faces;
		};
	}

#endif // GLINDE_MESHSIZE_HPP
