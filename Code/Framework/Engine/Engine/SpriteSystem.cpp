#include "EnginePCH.h"
#include "Engine/SpriteSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SpriteComponent.h"
#include "Engine/SpriteTemplate.h"

void eng::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const eng::SpriteTemplate>
		::Include<const eng::SpriteTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& spriteTemplate = view.ReadRequired<eng::SpriteTemplate>();
		auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(view);
		spriteComponent.m_Sprite = spriteTemplate.m_Sprite;
		spriteComponent.m_Colour = spriteTemplate.m_Colour;
		spriteComponent.m_Size = spriteTemplate.m_Size;
	}

	using UpdatedQuery = ecs::query
		::Updated<const eng::SpriteTemplate>
		::Include<eng::SpriteComponent, const eng::SpriteTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& spriteTemplate = view.ReadRequired<eng::SpriteTemplate>();
		auto& spriteComponent = view.WriteRequired<eng::SpriteComponent>();
		spriteComponent.m_Sprite = spriteTemplate.m_Sprite;
		spriteComponent.m_Colour = spriteTemplate.m_Colour;
		spriteComponent.m_Size = spriteTemplate.m_Size;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const eng::SpriteTemplate>>())
	{
		world.RemoveComponent<eng::SpriteComponent>(view);
	}
}