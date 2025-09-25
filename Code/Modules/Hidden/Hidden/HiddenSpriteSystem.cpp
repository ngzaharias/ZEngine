#include "HiddenPCH.h"
#include "Hidden/HiddenSpriteSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/GameplaySettingsComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TablesReloadedEvent.h"
#include "Engine/ThemeTable.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"

namespace
{
	void Refresh(hidden::SpriteSystem::World& world)
	{
		const auto& themes = world.ReadResource<eng::ThemeTable>();
		const auto& settings = world.ReadSingleton<eng::settings::GameplayComponent>();

		using Query = ecs::query::Include<const eng::SpriteComponent, const hidden::ObjectComponent>;
		for (const ecs::Entity& entity : world.Query<Query>())
		{
			const eng::Theme& theme = themes.GetTheme(settings.m_Theme);
			const bool wasRevealed = world.HasComponent<hidden::RevealComponent>(entity);

			auto& sprite = world.WriteComponent<eng::SpriteComponent>(entity);
			sprite.m_Colour = wasRevealed
				? theme.m_Highlight
				: theme.m_Background0;
		}
	}
}

void hidden::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& themes = world.ReadResource<eng::ThemeTable>();
	const auto& settings = world.ReadSingleton<eng::settings::GameplayComponent>();

	using ObjectQuery = ecs::query
		::Added<const hidden::ObjectComponent>
		::Include<const eng::SpriteComponent>;
	for (const ecs::Entity& entity : world.Query<ObjectQuery>())
	{
		const eng::Theme& theme = themes.GetTheme(settings.m_Theme);
		const bool wasRevealed = world.HasComponent<hidden::RevealComponent>(entity);

		auto& sprite = world.WriteComponent<eng::SpriteComponent>(entity);
		sprite.m_Colour = wasRevealed
			? theme.m_Highlight
			: theme.m_Background0;
	}

	using RevealQuery = ecs::query
		::Added<const hidden::RevealComponent>
		::Include<const eng::SpriteComponent>;
	for (const ecs::Entity& entity : world.Query<RevealQuery>())
	{
		const eng::Theme& theme = themes.GetTheme(settings.m_Theme);

		auto& sprite = world.WriteComponent<eng::SpriteComponent>(entity);
		sprite.m_Colour = theme.m_Highlight;
	}

	if (world.HasAny<ecs::query::Added<const eng::TablesReloadedEvent>>())
		Refresh(world);
	if (world.HasAny<ecs::query::Updated<const eng::settings::GameplayComponent>>())
		Refresh(world);
}