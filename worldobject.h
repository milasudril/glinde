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
	class Model;

	class WorldObject
		{
		public:
			WorldObject():r_model(nullptr),m_position(0,0,0),m_velocity(0,0,0)
				{}

			void modelSet(Model* model)noexcept
				{r_model=model;}

			WorldObject& positionSet(const glm::vec3& position) noexcept
				{
				m_position=position;
				return *this;
				}

			const glm::vec3& positionGet() const noexcept
				{return m_position;	}



			WorldObject& velocitySet(const glm::vec3& velocity) noexcept
				{
				m_velocity=velocity;
				return *this;
				}

			const glm::vec3& velocityGet() const noexcept
				{return m_velocity;}



			Camera& eyesGet() noexcept
				{return m_eyes;	}

			const Camera& eyesGet() const noexcept
				{return m_eyes;}

			const Model* modelGet() const noexcept
				{return r_model;}

			glm::mat4 viewMatrixGet() const noexcept
				{return glm::translate(m_eyes.viewMatrixGet(),-m_position);};

			const glm::vec3& headingGet() const noexcept
				{return m_eyes.headingGet();}

		private:
			Model* r_model;

			Camera m_eyes;
			glm::vec3 m_position;
			glm::vec3 m_velocity;
			glm::vec3 m_acceleration;


			float m_mass;
			float m_charge;
		};
	}

#endif
