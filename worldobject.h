#ifdef __WAND__
target[name[worldobject.h] type[include]]
#endif

#ifndef GLINDA_WORLDOBJECT_H
#define GLINDA_WORLDOBJECT_H

#define GLM_FORCE_RADIANS

#include "camera.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Glinda
	{

	class WorldObject
		{
		public:
			WorldObject():r_model(nullptr)
				,m_frame_start(0),m_frame_current(0),m_frame_end(0)
				,m_position(0,0,0)
				,m_velocity(0,0,0),m_mass(0),m_charge(0)
				{}

			void modelSet(Model* model) noexcept
				{r_model=model;}

			void frameStartSet(const Stringkey& key)
				{
				assert(r_model!=nullptr);
				m_frame_start=r_model->frameIndexGet(key);
				m_direction=m_frame_start>m_frame_end?-1:1;
				m_frame_current=m_frame_start;
				}

			void frameEndSet(const Stringkey& key)
				{
				assert(r_model!=nullptr);
				m_frame_start=r_model->frameIndexGet(key);
				m_direction=m_frame_start>m_frame_end?-1:1;
				}

			WorldObject& positionSet(const glm::vec3& position) noexcept
				{
				m_position=position;
				return *this;
				}

			const Model::Frame& frameCurrentGet() const noexcept
				{
				assert(r_model!=nullptr);
				return r_model->frameGet(m_frame_current);
				}

			const glm::vec3& positionGet() const noexcept
				{return m_position;}

			void frameNext()
				{
				m_frame_current=m_frame_start!=m_frame_end?
					m_frame_current+m_direction : m_frame_current;
				}



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


			glm::mat4 modelMatrixGet() const noexcept
				{
				glm::mat4 M;
				return glm::translate(M,m_position);
				}

		private:
			Model* r_model;
			size_t m_frame_start;
			size_t m_frame_current;
			size_t m_frame_end;
			intptr_t m_direction;

			Camera m_eyes;
			glm::vec3 m_position;
			glm::vec3 m_velocity;
			glm::vec3 m_acceleration;


			float m_mass;
			float m_charge;
		};
	}

#endif
