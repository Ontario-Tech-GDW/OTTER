/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Entity.h
Minimal and lightly OOP wrapper for the ENTT framework.
*/

#pragma once

#include "Transform.h"

#include "entt.hpp"

namespace nou
{
	class Entity
	{
		public:

		//We're giving every entity its own transform explicitly, rather than
		//as an ENTT component.
		//This is because pointers in ENTT are NOT stable - they can change
		//whenever we add a new entity, create a new component, etc.
		//because of how ENTT manages memory.
		//By putting the transform here and managing our own Entity objects carefully,
		//we make sure that our pointers will be stable - which is important
		//in a hierarchy with transforms storing pointers to parent/child objects.
		Transform transform;

		static Entity Create();
		static std::unique_ptr<Entity> Allocate();

		Entity(entt::entity id);
		Entity(Entity&&) = delete;

		virtual ~Entity();

		template<typename T, typename... Args>
		T& Add(Args&&... args)
		{
			return ecs.emplace<T>(m_id, std::forward<Args>(args)...);
		}

		template<typename T>
		T& Get()
		{
			return ecs.get<T>(m_id);
		}

		template<typename T>
		void Remove()
		{
			ecs.remove<T>(m_id);
		}

		protected:

		static entt::registry ecs;
		entt::entity m_id;	
	};
}