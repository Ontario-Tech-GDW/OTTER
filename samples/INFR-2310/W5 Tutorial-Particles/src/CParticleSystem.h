/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CParticleSystem.h
Basic SoA (structure of arrays) particle system component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/GLObjects.h"
#include "NOU/Entity.h"
#include "NOU/Material.h"

#include "GLM/glm.hpp"

#include <vector>
#include <deque>
#include <memory>

namespace nou
{
	//A little namespace "container" for us to define
	//some utility functions related to emission, random generation,
	//etc.
	namespace ParticleUtility
	{
		void VerticalConeEmit(float tanTheta, float speed, glm::vec3& pos, glm::vec3& vel);
	}

	//Utility struct for defining attributes of our particle system.
	struct ParticleParam
	{
		//Maximum number of particles alive in this system.
		size_t maxParticles;

		//How long a particle should "live".
		float lifetime;

		//Particle size at the start of its lifetime.
		float startSize;

		//How fast particles should start out.
		float startSpeed;

		//Particle colour at the start of its lifetime.
		glm::vec4 startColor;

		//Particle colour at the end of its lifetime.
		glm::vec4 endColor;

		//How many particles to emit per second.
		float emissionRate;

		//Tangent of the angle defining our emission cone.
		float tanTheta;

		//Specify default values for our particle system specs.
		ParticleParam()
		{
			maxParticles = 100;
			lifetime = 5.0f;
			startSize = 0.2f;
			startSpeed = 1.0f;
			startColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			endColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			emissionRate = 1.0f;
			tanTheta = glm::tan(glm::radians(15.0f));
		}
	};

	class CParticleSystem
	{
	public:

		static const size_t MAX_PARTICLES;

		//This enumerator corresponds to the layout locations we use as a 
		//convention for passing attributes to our vertex shaders.
		enum class Attrib
		{
			POSITION = 0,
			SIZE = 1,
			COLOR = 2
		};

		CParticleSystem(Entity& owner, Material& mat, const ParticleParam& startParam);
		~CParticleSystem() = default;

		CParticleSystem(CParticleSystem&&) = default;
		CParticleSystem& operator=(CParticleSystem&&) = default;

		void Update(float deltaTime);
		void Draw();

	private:

		//Utility struct for depth-sorting our particles on the CPU.
		struct ParticleSort
		{
			GLuint index;
			const bool* alive;
			const glm::vec3* viewPos;

			bool operator<(const ParticleSort& rhs) const
			{
				if (!(*rhs.alive) && *alive)
					return true;
				if (!(*alive) && *rhs.alive)
					return false;

				if(viewPos->z < rhs.viewPos->z)
					return true;
				if(rhs.viewPos->z < viewPos->z)
					return false;

				return index < rhs.index;
			}
		};

		//Utility struct for storing all of the data in our particle
		//system (this is separate since the ParticleSystem component
		//might move around in ENTT, so we just want a *pointer*
		//to all our hefty data).
		struct ParticleData
		{
			typedef std::map<Attrib, std::unique_ptr<VertexBuffer>> VBOLookup;

			//VAO/VBOs.
			std::unique_ptr<VertexArray> vao;
			VBOLookup vbos;

			//For depth sorting.
			std::vector<ParticleSort> sorter;

			//Particle attributes.
			std::vector<glm::vec3>    pos;
			std::vector<glm::vec3>    viewPos;
			std::vector<glm::float32> size;
			std::vector<glm::vec4>    color;

			std::vector<glm::vec3> velocity;

			std::vector<float> lifetime;

			//This is a regular pointer instead of a std::vector
			//due to the fact that there is a std::vector specialization
			//for booleans which is *no longer an STL container*.
			//Don't even get me started.
			bool* alive;

			//For indexing our VAO - telling it to draw only the particles
			//that are alive, and in the correct order.
			std::vector<GLuint> indices;

			ParticleParam param;

			//Total number of particles in the system (dead and alive).
			size_t count;
			//Number of currently living particles.
			size_t numAlive;
			//Index of the last living particle.
			size_t lastAlive;
			//Next available indices to insert a new living particle.
			std::deque<size_t> insertIndices;
			//Timer to track when particles should be emitted.
			float emissionTimer;

			ParticleData(size_t numParticles, const ParticleParam& startParam);
			~ParticleData();
		};

		Entity* m_owner;
		Material* m_mat;
		std::unique_ptr<ParticleData> m_data;

		void Emit();
		void Sort();
	};
}