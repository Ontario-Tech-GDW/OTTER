/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Transform.h
Simple class for managing object transforms with basic hierarchy support.
*/

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "GLM/glm.hpp"
#include "GLM/gtx/quaternion.hpp"

#include <vector>

//Simple implementation of a transform component.

namespace nou
{
	class Transform
	{
		public:

		glm::vec3 m_pos;
		glm::vec3 m_scale;
		glm::quat m_rotation;

		Transform();
		Transform(const Transform& other) = default;
		virtual ~Transform();

		//This will update the transform on the object
		//and all of its children.
		//If you are using a Scene concept, then you would
		//call this once per frame before making all of your draw
		//calls on the root node of your Scene.
		//(FK stands for "forward kinematics", by the way.)
		void DoFK();

		//This will recompute and return the global transform
		//of this object.
		const glm::mat4& RecomputeGlobal();

		//This will return the current global transform of the
		//object (it will not recompute it - make sure that it has been
		//computed via Recompute or a DoFK call on its parent before using).
		const glm::mat4& GetGlobal() const;

		//This will return the current normal matrix of the object
		//(used for lighting). As above, make sure you have called
		//the appropriate update first.
		glm::mat3 GetNormal() const;

		//Sets a pointer to the parent object and updates child references
		//for the old and new parent objects accordingly.
		//Pass in nullptr if you wish for the object to not have a parent.
		void SetParent(Transform* parent);

		protected:

		Transform* m_parent;
		std::vector<Transform*> m_children;

		glm::mat4 m_global;

		//These functions are protected since they will be handled
		//by SetParent - we don't want to have to manually update this ourselves
		//whenever we switch an object's parent!
		void AddChild(Transform* child);
		void RemoveChild(Transform* child);
	};
}