#ifdef __WAND__
target[name[camera.h] type[include]]
#endif

/**\file camera.h \brief Defines the Camera class.
 */

#ifndef GLINDA_CAMERA_H
#define GLINDA_CAMERA_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Glinda
	{
	/**\brief This class describes a camera
	*/
	class Camera
		{
		public:
			Camera():m_pitch(std::acos(0.0f))
				,m_roll(std::acos(1.0f)),m_yaw(0.0f)
				{}

			Camera& pitchSet(float pitch) noexcept
				{
				m_pitch=pitch;
				return *this;
				}

			Camera& rollSet(float roll) noexcept
				{
				m_roll=roll;
				return *this;
				}

			Camera& yawSet(float yaw) noexcept
				{
				m_yaw=yaw;
				return *this;
				}

			Camera& fovYSet(float fov) noexcept
				{
				m_fov_y=fov;
				return *this;
				}

			float fovYGet(float fov) const noexcept
				{
				return m_fov_y;
				}

			void headingUpdate() noexcept
				{
				m_heading=glm::vec3(std::sin(m_pitch)*std::sin(m_yaw)
					,std::sin(m_pitch)*std::cos(m_yaw)
					,-std::cos(m_pitch));
				}

			glm::mat4 viewMatrixGet() const noexcept
				{
				glm::mat4 ret;
				ret=glm::rotate(ret,m_pitch,glm::vec3(-1.0f,0.0f,0.0f));
				ret=glm::rotate(ret,m_roll,glm::vec3(0.0f,1.0f,0.0f));
				ret=glm::rotate(ret,m_yaw,glm::vec3(0.0f,0.0f,1.0f));
				return ret;
				}

			const glm::vec3& headingGet() const noexcept
				{return m_heading;}

		private:
			glm::vec3 m_heading;
			float m_pitch;
			float m_roll;
			float m_yaw;
			float m_fov_y;
		};
	};

#endif
