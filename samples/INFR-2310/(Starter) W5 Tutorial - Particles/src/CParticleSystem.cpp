/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CParticleSystem.cpp
Basic SoA (structure of arrays) particle system component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CParticleSystem.h"
#include "NOU/CCamera.h"

namespace nou
{
	const size_t CParticleSystem::MAX_PARTICLES = 10000;

	//Generate a random velocity out of a cone in the Y-direction
	//specified with angle theta.
	void ParticleUtility::VerticalConeEmit(float tanTheta, float speed, glm::vec3& pos, glm::vec3& vel)
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		float x, z;

		//This doesn't produce perfectly distributed directions,
		//but it is one of the fastest/easiest ways to get a 
		//"good enough" random distribution.

		//This first step is called the "rejection method" for
		//generating a point in a unit circle.
		//(It spawns a random point in the unit square from -1.0 to 1.0,
		//and then tries again if the point doesn't fall 
		//inside the unit circle.)
		do
		{
			x = 2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f;
			z = 2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f;
		} 
		while ((x*x + z*z) > 1.0f);

		vel = speed * glm::normalize(
		              glm::vec3(tanTheta*x, 1.0f, tanTheta*z));
	}

	CParticleSystem::CParticleSystem(Entity& owner, Material& mat, const ParticleParam& startParam)
	{
		m_owner = &owner;
		m_mat = &mat;

		size_t numParticles = (startParam.maxParticles < MAX_PARTICLES)
			? startParam.maxParticles
			: MAX_PARTICLES;

		//This is what effectively initializes our particle system.
		m_data = std::make_unique<ParticleData>(numParticles, startParam);
	}

	CParticleSystem::ParticleData::ParticleData(size_t numParticles,
		const ParticleParam& startParam)
	{
		count = numParticles;
		param = startParam;

		pos.resize(count);
		viewPos.resize(count);
		//We will be passing view-space position to the GPU.
		vbos.insert({ Attrib::POSITION,
					  std::make_unique<VertexBuffer>(3, viewPos) });

		size.resize(count);
		vbos.insert({ Attrib::SIZE,
					  std::make_unique<VertexBuffer>(1, size) });
		
		color.resize(count);
		vbos.insert({ Attrib::COLOR,
					  std::make_unique<VertexBuffer>(4, color) });

		vao = std::make_unique<VertexArray>();
		vao->SetDrawMode(VertexArray::DrawMode::POINTS);

		vao->BindAttrib(*(vbos[Attrib::POSITION]),
			static_cast<GLint>(Attrib::POSITION));
		vao->BindAttrib(*(vbos[Attrib::SIZE]),
			static_cast<GLint>(Attrib::SIZE));
		vao->BindAttrib(*(vbos[Attrib::COLOR]),
			static_cast<GLint>(Attrib::COLOR));

		velocity.resize(count);

		lifetime.resize(count);

		alive = static_cast<bool*>(calloc(count, sizeof(bool)));

		indices.resize(count);
		std::fill(indices.begin(), indices.end(), 0);
		
		sorter.resize(count);

		for (size_t i = 0; i < count; ++i)
		{
			sorter[i].index = static_cast<GLuint>(i);
			sorter[i].alive = &(alive[i]);
			sorter[i].viewPos = &(viewPos[i]);
		}

		insertIndices.resize(count);

		for (size_t i = 0; i < count; ++i)
		{
			insertIndices[i] = i;
		}

		numAlive = 0;
		lastAlive = 0;
		emissionTimer = 0.0f;
	}

	CParticleSystem::ParticleData::~ParticleData()
	{
		free(alive);
	}

	void CParticleSystem::Update(float deltaTime)
	{
		//TODO: Emit particles!

		auto& camera = CCamera::current->Get<CCamera>();
		auto& transform = m_owner->transform;
		glm::mat4 modelview = camera.GetView() * transform.GetGlobal();

		//Update all our particles...
		//We do this backwards to keep our deque sorted.
		//(Smallest available index should be at the front.).
		for (int index = static_cast<int>(m_data->lastAlive); index >= 0; --index)
		{
			size_t i = static_cast<size_t>(index);

			//TODO: Pass through if the current particle is dead.
			//TODO: What should we update first?
			//TODO: How do we check if the particle should still be alive?
			//TODO: Update our particle if it's still alive!
		}

		Sort();
	}

	void CParticleSystem::Draw()
	{
		//Update our per-particle data.
		m_data->vbos[Attrib::POSITION]->UpdateData(m_data->viewPos);
		m_data->vbos[Attrib::SIZE]->UpdateData(m_data->size);
		m_data->vbos[Attrib::COLOR]->UpdateData(m_data->color);

		m_mat->Use();

		auto& camera = CCamera::current->Get<CCamera>();
		ShaderProgram::Current()->SetUniform("proj", camera.GetProj());

		m_data->vao->DrawElements(m_data->indices, m_data->numAlive);
	}

	void CParticleSystem::Emit()
	{
		//We can only emit if we have some "dead" particles
		//we can "respawn".
		if (m_data->numAlive < m_data->count)
		{
			size_t i = m_data->insertIndices.front();
			m_data->insertIndices.pop_front();

			//TODO: Initialize our particle values.

			m_data->alive[i] = true;

			++(m_data->numAlive);

			if (i > m_data->lastAlive)
				m_data->lastAlive = i;
		}
	}

	//Depth sorting of particles on the CPU.
	void CParticleSystem::Sort()
	{
		std::sort(m_data->sorter.begin(), m_data->sorter.end());

		for (size_t i = 0; i < m_data->numAlive; ++i)
		{
			m_data->indices[i] = m_data->sorter[i].index;
		}
	}
}