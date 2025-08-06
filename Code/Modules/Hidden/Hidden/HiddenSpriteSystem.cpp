#include "HiddenPCH.h"
#include "Hidden/HiddenSpriteSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SpriteComponent.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"
#include "Hidden/HiddenSettingsComponents.h"

void hidden::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<hidden::settings::LocalComponent>();

	using Query = ecs::query
		::Added<const hidden::RevealComponent>
		::Include<const eng::SpriteComponent, const hidden::ObjectComponent>;
	for (const ecs::Entity& entity : world.Query<Query>())
	{
		const auto& object = world.ReadComponent<hidden::ObjectComponent>(entity);
		auto& sprite = world.WriteComponent<eng::SpriteComponent>(entity);

		for (const hidden::Effect& effect : object.m_Effects)
		{
			core::VariantMatch(effect,
				[&](const hidden::SetColour& data)
				{
					sprite.m_Colour = settings.m_Highlight;
				},
				[&](const hidden::SetSprite& data)
				{
					sprite.m_Sprite = data.m_Sprite;
				});
		}
	}
}