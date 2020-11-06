#pragma once
#include "entt.hpp"
#include "Utilities/Macros.h"

/// <summary>
/// Represents a callback that may be used to customize how entity stamping works between registries
/// </summary>
typedef void(*StampFunction)(const entt::registry& from, const entt::entity src, entt::registry& to, const entt::entity dst);

typedef entt::handle GameObject;

class GameScene final
{
	SMART_MEMORY_MANAGED(GameScene)
	
public:	
	std::string Name;

	GameScene(const std::string& name = "<default>");
	~GameScene() = default;
	
	entt::handle CreateEntity(const std::string& name = "");
	entt::handle CreateEntity(entt::entity prefab, const std::string& name = "");

	entt::handle FindFirst(const std::string& name);

	entt::registry& Registry() { return _registry; }

	/// <summary>
	/// Perform any tasks that should happen at the end of a loop, such as deleting queued objects
	/// </summary>
	void Poll() {
		for(entt::entity instance : _deletionQueue) {
			_registry.destroy(instance);
		}
		_deletionQueue.clear();
	}

	/// <summary>
	/// Creates a new entity in the <i>to</i> registry, copying the components from the <i>src</i> entity in the from registry
	/// </summary>
	/// <param name="from">The source registry to copy the object from</param>
	/// <param name="src">The source entity within the <i>from</i> registry to copy</param>
	/// <param name="to">The destination registry to store the entity in</param>
	/// <returns>A handle for the newly created entity</returns>
	static entt::handle StampEntity(const entt::registry& from, entt::entity src, entt::registry& to);

	template <typename Type>
	static void RegisterComponentType(StampFunction stampOverride = nullptr) {
		_stampFunctions[entt::type_info<Type>::id()] = stampOverride != nullptr ? stampOverride : &_DefaultComponentStamp<Type>;
	}
	static entt::registry& Prefabs() { return _prefabRegistry; }
	
private:
	entt::registry _registry;
	std::vector<entt::entity> _deletionQueue;

	static entt::registry _prefabRegistry;
	static std::unordered_map<entt::id_type, StampFunction> _stampFunctions;

	template <typename T>
	static void _DefaultComponentStamp(const entt::registry& from, const entt::entity src, entt::registry& to, const entt::entity dst) {
		to.emplace_or_replace<T>(dst, from.get<T>(src));
	}
};
