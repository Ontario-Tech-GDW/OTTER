/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

PathUtility.h
Simple utility for sampling paths and drawing lines with OpenGL.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/GLObjects.h"
#include "NOU/Material.h"
#include "NOU/Entity.h"

#include <memory>
#include <functional>

namespace nou
{
	class PathSampler
	{
		public:

		//These affect how smooth our drawn version
		//of a path will be.
		static const int NUM_SAMPLES;
		static const float SAMPLE_T;

		//These typedefs are just to make things look
		//a little bit cleaner.
		typedef std::vector<std::unique_ptr<Entity>> KeypointSet;

		typedef std::function<glm::vec3(
				    const glm::vec3&, const glm::vec3&,
					const glm::vec3&, const glm::vec3&, float)> 
					CurveInterpolator;

		enum class PathMode
		{
			CATMULL,
			BEZIER
		};

		//This determines which interpolation mode we will use.
		PathMode m_mode;

		static CurveInterpolator Catmull;
		static CurveInterpolator Bezier;

		PathSampler();
		~PathSampler() = default;

		void Resample(const KeypointSet& keypoints);

		const std::vector<glm::vec3>& GetSamples() const;

		private:

		std::vector<glm::vec3> m_samples;
	};

	class CLineRenderer
	{
		public:

		CLineRenderer(Entity& owner, PathSampler& pathSource, Material& mat);
		virtual ~CLineRenderer() = default;

		CLineRenderer(CLineRenderer&&) = default;
		CLineRenderer& operator=(CLineRenderer&&) = default;

		void Draw(const PathSampler::KeypointSet& keypoints);

		protected:

		Entity* m_owner;
		PathSampler* m_pathSource;
		Material* m_mat;

		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<VertexArray> m_vao;	
	};
}