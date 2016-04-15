#ifdef __WAND__
target[name[worldobject.h] type[include]]
#endif

#ifndef GLINDA_WORLDOBJECT_H
#define GLINDA_WORLDOBJECT_H

#define GLM_FORCE_RADIANS

#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Glinda
	{
	class Mesh;

	class WorldObject
		{
		public:
			WorldObject():r_mesh(nullptr),m_location(0,0,0)
				{}

			void meshSet(Mesh* mesh)noexcept
				{r_mesh=mesh;}

			WorldObject& locationSet(const glm::vec3& location) noexcept
				{
				m_location=location;
				return *this;
				}

			Camera& eyesGet() noexcept
				{
				return m_eyes;
				}

			const Camera& eyesGet() const noexcept
				{
				return m_eyes;
				}

			const Mesh* meshGet() const noexcept
				{return r_mesh;}

		private:
			Mesh* r_mesh;

			glm::vec3 m_location;

			glm::vec3 m_velocity;
			glm::vec3 m_acceleration;

			Camera m_eyes;

			float m_mass;
			float m_charge;
		};
	}

#endif
