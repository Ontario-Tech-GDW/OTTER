#include "Scene.h"

#include "Transform.h"
#include "GameObjectTag.h"
#include "Logging.h"

entt::registry GameScene::_prefabRegistry;
std::unordered_map<entt::id_type, StampFunction> GameScene::_stampFunctions;

GameScene::GameScene(const std::string& name) {
	Name = name;

	RegisterComponentType<Transform>();
	RegisterComponentType<GameObjectTag>();
}

entt::handle GameScene::CreateEntity(const std::string& name) {
	entt::entity entity = _registry.create();
	entt::handle result = entt::handle(_registry, entity);
	// pass the handle to the transform constructor
	auto& transform = _registry.emplace<Transform>(entity, result);
	auto& tag = _registry.emplace<GameObjectTag>(entity, name);
	return result;
}

entt::handle GameScene::CreateEntity(entt::entity prefab, const std::string& name) {
	LOG_ASSERT(_prefabRegistry.valid(prefab), "Entity is not a valid prefab! You may need to call CreatePrefab(entity_id) first!");

	const entt::entity instance = StampEntity(_prefabRegistry, prefab, _registry);
	return entt::handle(_registry, instance);
}

entt::handle GameScene::FindFirst(const std::string& name)
{
	entt::entity result = entt::null;
	uint32_t hash = entt::hashed_string::value(name.c_str());
	const auto view = _registry.view<GameObjectTag>();
	
	for(const entt::entity& entity : view) {
		const GameObjectTag& tag = _registry.get<GameObjectTag>(entity);
		if (tag.HashedName == hash && tag.Name == name) {
			return entt::handle(_registry, entity);
		}
	}
	return entt::handle(_registry, entt::null);
}

entt::handle GameScene::StampEntity(const entt::registry& from, entt::entity src, entt::registry& to) {
	entt::entity dst = to.create();
	from.visit(src, [&from, &to, src, dst](const auto type_id) {
		_stampFunctions[type_id](from, src, to, dst);
	});
	return entt::handle(to, dst);
}
