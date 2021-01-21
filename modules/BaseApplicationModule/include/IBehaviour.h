#pragma once
#include <memory>
#include <entt.hpp>
#include <typeindex>
struct BehaviourBinding;

/*
 * Represents a behaviour that can be tied to a single GameObject
 */
class IBehaviour {
public:
	/*
	 * Whether or not this component will fire it's events
	 */
	bool    Enabled = true;
	virtual ~IBehaviour() = default;

	/*
	 * Invoked when the behaviour is added to the scene, or the scene has been loaded
	 * @param entity The entity that the behaviour is bound to
	 */
	virtual void OnLoad(entt::handle entity) {}
	/*
	 * Invoked when the behaviour is removed from the entity, or the scene is unloaded
	 * @param entity The entity that the behaviour is bound to
	 */
	virtual void OnUnload(entt::handle entity) {}
	/*
	 * Invoked during the variable rate update. This is generally where we want to add our updates.
	 * To get the time since the last update, use florp::app::Timing::DeltaTime
	 * @param entity The entity that the behaviour is bound to
	 */
	virtual void Update(entt::handle entity) {}
	/*
	 * Invoked during the fixed rate update phase.
	 * To get the approximate time since the last fixed update, use florp::app::Timing::FixedTimeStep
	 * @param entity The entity that the behaviour is bound to
	 */
	virtual void FixedUpdate(entt::handle entity) {}
	/*
	 * Invoked during the variable rate update. This is called after all behaviours/layers have called Update
	 * To get the time since the last update, use florp::app::Timing::DeltaTime
	 * @param entity The entity that the behaviour is bound to
	 */
	virtual void LateUpdate(entt::handle entity) {}
	/*
	 * Invoked during the GUI render phase.
	 * @param entity The entity that the behaviour is bound to
	 */
	virtual void RenderGUI(entt::handle entity) {}

protected:
	IBehaviour() = default;
};

/*
 * The component added to an entt entity to connect a behaviour to a specific entity
 */
struct BehaviourBinding {
	std::vector<std::shared_ptr<IBehaviour>> Behaviours;

	/*
	 * Binds an IBehaviour interface to the given entt entity
	 * @param T The type of behaviour to add
	 * @param TArgs The argument types to forward to the behaviour's constructor
	 * @param registry The registry that the entity is part of
	 * @param entity The entity to add the behaviour to
	 * @param args The arguments to forward to the behaviour's constructor
	 */
	template <typename T, typename ... TArgs, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
	static std::shared_ptr<T> Bind(entt::handle entity, TArgs&&... args) {
		// Get the binding component
		BehaviourBinding& binding = entity.get_or_emplace<BehaviourBinding>();
		// Make a new behaviour, forwarding the arguments
		const std::shared_ptr<IBehaviour> behaviour = std::make_shared<T>(std::forward<TArgs>(args)...);
		// Append it to the binding component's storage, and invoke the OnLoad
		binding.Behaviours.push_back(behaviour);
		behaviour->OnLoad(entity);
		return std::dynamic_pointer_cast<T>(behaviour);
	}

	/*
	 * Binds an IBehaviour interface to the given entt entity, setting it to disabled by default
	 * @param T The type of behaviour to add
	 * @param TArgs The argument types to forward to the behaviour's constructor
	 * @param registry The registry that the entity is part of
	 * @param entity The entity to add the behaviour to
	 * @param args The arguments to forward to the behaviour's constructor
	 */
	template <typename T, typename ... TArgs, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
	static std::shared_ptr<T> BindDisabled(entt::handle entity, TArgs&&... args) {
		// Get the binding component
		BehaviourBinding& binding = entity.get_or_emplace<BehaviourBinding>();
		// Make a new behaviour, forwarding the arguments
		const std::shared_ptr<IBehaviour> behaviour = std::make_shared<T>(std::forward<TArgs>(args)...);
		behaviour->Enabled = false;
		// Append it to the binding component's storage, and invoke the OnLoad
		binding.Behaviours.push_back(behaviour);
		behaviour->OnLoad(entity);
		return std::dynamic_pointer_cast<T>(behaviour);
	}

	
	/*
	 * Checks whether the given entity has a behaviour of the given type
	 * @param T The type of behaviour to check for
	 * @param registry The registry that the entity is part of
	 * @param entity The entity to check
	 * @returns True if a behaviour of type T is attached to entity, or false if otherwise
	 */
	template <typename T, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
	static bool Has(entt::handle entity) {
		// Check to see if the entity has a behaviour binding attached
		if (entity.has<BehaviourBinding>()) {
			// Get the binding component
			const auto& binding = entity.get<BehaviourBinding>();
			// Iterate over all the pointers in the binding list
			for (const auto& ptr : binding.Behaviours) {
				// If the pointer type matches T, we return true
				if (std::type_index(typeid(*ptr.get())) == std::type_index(typeid(T))) {
					return true;
				}
			}
			return false;
		}
		else return false;
	}

	/*
	 * Gets the behaviour with the given type from the entity, or nullptr if none exists
	 * @param T The type of behaviour to check for
	 * @param registry The registry that the entity is part of
	 * @param entity The entity to search
	 * @returns The behaviour of type T that is attached to entity, or nullptr if no behaviour of that type is attached
	 */
	template <typename T, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
	static std::shared_ptr<T> Get(entt::handle entity) {
		// Check to see if the entity has a behaviour binding attached
		if (entity.has<BehaviourBinding>()) {
			// Get the binding component
			const auto& binding = entity.get<BehaviourBinding>();
			// Iterate over all the pointers in the binding list
			for (const auto& ptr : binding.Behaviours) {
				// If the pointer type matches T, we return that behaviour, making sure to cast it back to the requested type
				if (std::type_index(typeid(*ptr.get())) == std::type_index(typeid(T))) {
					return std::dynamic_pointer_cast<T>(ptr);
				}
			}
			return nullptr;
		}
		else return nullptr;
	}
};