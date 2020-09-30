/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

PathUtility.cpp
Simple utility for sampling paths and drawing lines with OpenGL.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "PathUtility.h"
#include "NOU/Mesh.h"
#include "NOU/CCamera.h"

namespace nou
{
	const int PathSampler::NUM_SAMPLES = 8;
	const float PathSampler::SAMPLE_T = 1.0f / static_cast<float>(PathSampler::NUM_SAMPLES);
	PathSampler::CurveInterpolator PathSampler::Catmull = nullptr;
	PathSampler::CurveInterpolator PathSampler::Bezier = nullptr;

	PathSampler::PathSampler()
	{
		m_mode = PathMode::CATMULL;
		m_samples.push_back(glm::vec3(0.0f));
	}

	void PathSampler::Resample(const KeypointSet& keypoints)
	{
		CurveInterpolator interp = (m_mode == PathMode::CATMULL) ? Catmull : Bezier;

		if (interp == nullptr)
			m_samples.clear();
		else
		{
			switch (m_mode)
			{
			case PathMode::CATMULL:

			{
				if (keypoints.size() < 4)
				{
					m_samples.clear();
					break;
				}

				//NUM_SAMPLES points for each of our segments, plus
				//one to close our loop.
				m_samples.resize((NUM_SAMPLES) * (keypoints.size()) + 1);

				size_t s = 0;

				size_t p0_ind, p1_ind, p2_ind, p3_ind;
				glm::vec3 p0, p1, p2, p3;

				for (size_t i = 0; i < keypoints.size(); ++i)
				{
					p1_ind = i;
					p0_ind = (p1_ind == 0) ? keypoints.size() - 1 : p1_ind - 1;
					p2_ind = (p1_ind + 1) % keypoints.size();
					p3_ind = (p2_ind + 1) % keypoints.size();

					p0 = keypoints[p0_ind]->transform.m_pos;
					p1 = keypoints[p1_ind]->transform.m_pos;
					p2 = keypoints[p2_ind]->transform.m_pos;
					p3 = keypoints[p3_ind]->transform.m_pos;

					for (size_t j = 0; j < NUM_SAMPLES; ++j)
					{
						m_samples[s] = Catmull(p0, p1, p2, p3, j * SAMPLE_T);
						++s;
					}
				}

				m_samples[s] = keypoints[0]->transform.m_pos;
			}

			break;

			case PathMode::BEZIER:
			{
				if (keypoints.size() < 6 || keypoints.size() % 3 != 0)
				{
					m_samples.clear();
					break;
				}

				//NUM_SAMPLES points for each of our segments, plus
				//4 points to draw our tangents.
				m_samples.resize((NUM_SAMPLES + 4) * (keypoints.size() / 3));

				size_t s = 0;

				size_t p0_ind, p1_ind, p2_ind, p3_ind;
				glm::vec3 p0, p1, p2, p3;

				for (size_t i = 0; i < keypoints.size() - 2; i += 3)
				{
					p0_ind = i;
					p1_ind = (p0_ind + 1) % keypoints.size();
					p2_ind = (p1_ind + 1) % keypoints.size();
					p3_ind = (p2_ind + 1) % keypoints.size();

					p0 = keypoints[p0_ind]->transform.m_pos;
					p1 = keypoints[p1_ind]->transform.m_pos;
					p2 = keypoints[p2_ind]->transform.m_pos;
					p3 = keypoints[p3_ind]->transform.m_pos;

					m_samples[s] = p0;
					++s;
					m_samples[s] = p1;
					++s;

					for (size_t j = 0; j < NUM_SAMPLES; ++j)
					{
						m_samples[s] = Bezier(p0, p1, p2, p3, j * SAMPLE_T);
						++s;
					}

					m_samples[s] = p3;
					++s;
					m_samples[s] = p2;
					++s;
				}
			}

			break;
			}
		}

		if (m_samples.size() == 0)
			m_samples.push_back(glm::vec3(0.0f));
	}

	const std::vector<glm::vec3>& PathSampler::GetSamples() const
	{
		return m_samples;
	}

	CLineRenderer::CLineRenderer(Entity& owner, PathSampler& pathSource, Material& mat)
	{
		m_owner = &owner;
		m_pathSource = &pathSource;
		m_mat = &mat;

		m_vao = std::make_unique<VertexArray>();
		m_vao->SetDrawMode(VertexArray::DrawMode::LINE_STRIP);

		m_vbo = std::make_unique<VertexBuffer>(3, m_pathSource->GetSamples(), true);

		m_vao->BindAttrib(*m_vbo, (GLint)(Mesh::Attrib::POSITION));
	}

	void CLineRenderer::Draw(const PathSampler::KeypointSet& keypoints)
	{
		m_pathSource->Resample(keypoints);
		m_vbo->UpdateData(m_pathSource->GetSamples());
		m_mat->Use();

		//We are assuming the names used by uniform shader variables as a convention here.
		//In a larger project, we would have a more elegant system for registering
		//or even automatically detecting uniform names.
		ShaderProgram::Current()->SetUniform("viewproj", CCamera::current->Get<CCamera>().GetVP());
		ShaderProgram::Current()->SetUniform("model", m_owner->transform.GetGlobal());

		m_vao->Draw();
	}
}