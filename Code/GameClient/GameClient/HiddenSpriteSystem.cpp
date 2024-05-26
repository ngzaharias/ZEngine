#include "GameClientPCH.h"
#include "GameClient/HiddenSpriteSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/SpriteComponent.h>

#include "GameClient/HiddenObjectComponents.h"

void hidden::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Added<const hidden::RevealedComponent>
		::Include<const hidden::ObjectComponent>
		::Exclude<const eng::SpriteComponent>;

	for (const ecs::Entity& entity : world.Query<Query>())
	{
		const auto& hiddenComponent = world.GetComponent<const hidden::ObjectComponent>(entity);
		auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(entity);
		spriteComponent.m_Size = hiddenComponent.m_Size;
		spriteComponent.m_Sprite = hiddenComponent.m_Sprite;
	}
}