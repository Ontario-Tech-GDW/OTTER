#pragma once
#include <string>
#include <entt.hpp>

/// <summary>
/// Represents information associated with a game object within our scene
/// </summary>
struct GameObjectTag
{
	std::string Name;
	uint32_t    HashedName;
	GameObjectTag() : Name(""), HashedName(""_hs) {}
	GameObjectTag(const GameObjectTag& other) {
		Name = other.Name;
		HashedName = other.HashedName;
	}
	GameObjectTag(GameObjectTag&& other) noexcept {
		Name = other.Name;
		HashedName = other.HashedName;
	}
	GameObjectTag& operator=(GameObjectTag&& other) noexcept{
		Name = other.Name;
		HashedName = other.HashedName;
		return *this;
	}
	GameObjectTag& operator=(const GameObjectTag& other) noexcept {
		Name = other.Name;
		HashedName = other.HashedName;
		return *this;
	}
	~GameObjectTag() = default;
	
	GameObjectTag(const std::string& name) {
		Name = name;
		HashedName = entt::hashed_string::value(name.c_str());
	}

	// TODO: we could expand this in the future for properties that all game objects should have
};