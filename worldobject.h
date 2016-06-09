#ifdef __WAND__
target[name[worldobject.h] type[include]]
dependency[worldobject.o]
#endif

#ifndef GLINDE_WORLDOBJECT_H
#define GLINDE_WORLDOBJECT_H

#define GLM_FORCE_RADIANS

#include "camera.h"
#include "model.h"

namespace Glinde
	{
	class Item;

	class WorldObject
		{
		public:
			WorldObject() noexcept:r_model(nullptr)
				,m_frame_start(0),m_frame_current(0),m_frame_end(0)
				,m_position(0,0,0),m_rot_z(0)
				,m_velocity(0,0,0)
				,m_force(0,0,0)
				,m_mass(0),m_damping(0),m_charge(0)
				{}

			WorldObject(const Item& item);

			void modelSet(const Model* model) noexcept
				{
				r_model=model;
				if(model!=nullptr)
					{
					auto height=frameCurrentGet().bounding_box.size().z;
					m_mass=23*height*height;
				//	TODO: Is damping really connected to the mass
					m_damping=m_mass/0.0625f;
					}
				}

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



			WorldObject& forceSet(const glm::vec3& force) noexcept
				{
				m_force=force;
				return *this;
				}

			const glm::vec3& forceGet() const noexcept
				{return m_force;}



			WorldObject& normalImpulseSet(const glm::vec3& impulse) noexcept
				{
				m_normal_impulse=impulse;
				return *this;
				}

			const glm::vec3& normalImpulseGet() const noexcept
				{return m_normal_impulse;}



			WorldObject& dampingSet(float damping) noexcept
				{
				m_damping=damping;
				return *this;
				}

			float dampingGet() const noexcept
				{
				return m_damping;
				}



			WorldObject& massSet(float mass) noexcept
				{
				m_mass=mass;
				return *this;
				}

			float massGet() const
				{return m_mass;}




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
				M=glm::translate(M,m_position);
				return glm::rotate(M,m_rot_z,glm::vec3(0.0f,0.0f,1.0f));
				}

			WorldObject& rotZSet(float angle) noexcept
				{
				m_rot_z=angle;
				return *this;
				}

		private:
			const Model* r_model;
			size_t m_frame_start;
			size_t m_frame_current;
			size_t m_frame_end;
			intptr_t m_direction;

			Camera m_eyes;
			glm::vec3 m_position;
			float m_rot_z;
			glm::vec3 m_velocity;

			glm::vec3 m_force;
			glm::vec3 m_normal_impulse;
			float m_mass;
			float m_damping;
			float m_charge;
		};
	}

#endif
