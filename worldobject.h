#ifdef __WAND__
target[name[worldobject.h] type[include]]
#endif

#ifndef GLINDA_WORLDOBJECT_H
#define GLINDA_WORLDOBJECT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Glinda
	{
	class Mesh;

	class WorldObject
		{
		public:
			WorldObject():
				r_mesh(nullptr)
				,m_location(0,0,0),m_pitch(std::acos(0.0f))
				,m_roll(std::acos(-1.0f)),m_yaw(0.0f)
				{}

			void meshSet(Mesh* mesh)noexcept
				{r_mesh=mesh;}

			WorldObject& locationSet(const glm::vec3& location) noexcept
				{
				m_location=location;
				return *this;
				}

			WorldObject& pitchSet(float pitch) noexcept
				{
				m_pitch=pitch;
				return *this;
				}

			WorldObject& rollSet(float roll) noexcept
				{
				m_roll=roll;
				return *this;
				}

			WorldObject& yawSet(float yaw) noexcept
				{
				m_yaw=yaw;
				return *this;
				}

			void headingUpdate() noexcept
				{
				m_heading=glm::vec3(std::sin(m_pitch)*std::sin(m_yaw)
					,std::sin(m_pitch)*std::cos(m_yaw)
					,std::cos(m_pitch));
				}

			glm::mat4 viewMatrixGet() const noexcept
				{
				glm::mat4 ret;

				ret=glm::rotate(ret,m_pitch,glm::vec3(1.0f,0.0f,0.0f));
				ret=glm::rotate(ret,m_roll,glm::vec3(0.0f,1.0f,0.0f));
				ret=glm::rotate(ret,m_yaw,glm::vec3(0.0f,0.0f,1.0f));
				ret=glm::translate(ret,m_location);

				return ret;
				}

			void walk(float direction)
				{
			//TODO: Get z from ground
				m_location+=0.1f*direction*glm::vec3(m_heading.x,m_heading.y,0.0f);
				}

			void strafe(float direction)
				{
			//TODO: Get z from ground
				m_location+=0.1f*direction*glm::vec3(m_heading.y,-m_heading.x,0.0f);
				}

			const Mesh* meshGet() const noexcept
				{return r_mesh;}

		private:
			Mesh* r_mesh;

			glm::vec3 m_location;

			glm::vec3 m_velocity;
			glm::vec3 m_acceleration;

			glm::vec3 m_heading;
			float m_pitch;
			float m_roll;
			float m_yaw;

			float m_mass;
			float m_charge;
		};
	}

#endif
