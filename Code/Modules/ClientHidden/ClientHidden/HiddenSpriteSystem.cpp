#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenSpriteSystem.h"

#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsGameplaySingleton.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TablesReloaded.h"
#include "Engine/ThemeTable.h"

namespace
{
	void Refresh(client::hidden::SpriteSystem::World& world)
	{
		const auto& themes = world.ReadResource<eng::ThemeTable>();
		const auto& settings = world.ReadSingleton<eng::settings::GameplaySingleton>();

		using Query = ecs::query
			::Include<const eng::SpriteComponent, const client::hidden::ObjectComponent>
			::Optional<const client::hidden::RevealComponent>;
		for (auto&& view : world.Query<Query>())
		{
			const eng::Theme& theme = themes.GetTheme(settings.m_Theme);
			const bool wasRevealed = view.HasOptional<client::hidden::RevealComponent>();

			auto& sprite = view.WriteRequired<eng::SpriteComponent>();
			sprite.m_Colour = wasRevealed
				? theme.m_Highlight
				: theme.m_Background0;
		}
	}
}

void client::hidden::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& themes = world.ReadResource<eng::ThemeTable>();
	const auto& settings = world.ReadSingleton<eng::settings::GameplaySingleton>();

	using ObjectQuery = ecs::query
		::Added<const client::hidden::ObjectComponent>
		::Include<const eng::SpriteComponent>
		::Optional<const client::hidden::RevealComponent>;
	for (auto&& view : world.Query<ObjectQuery>())
	{
		const eng::Theme& theme = themes.GetTheme(settings.m_Theme);
		const bool wasRevealed = view.HasOptional<client::hidden::RevealComponent>();

		auto& sprite = view.WriteRequired<eng::SpriteComponent>();
		sprite.m_Colour = wasRevealed
			? theme.m_Highlight
			: theme.m_Background0;
	}

	using RevealQuery = ecs::query
		::Added<const client::hidden::RevealComponent>
		::Include<const eng::SpriteComponent>;
	for (auto&& view : world.Query<RevealQuery>())
	{
		const eng::Theme& theme = themes.GetTheme(settings.m_Theme);

		auto& sprite = view.WriteRequired<eng::SpriteComponent>();
		sprite.m_Colour = theme.m_Highlight;
	}

	if (world.HasAny<ecs::query::Added<const eng::TablesReloaded>>())
		Refresh(world);
	if (world.HasAny<eng::settings::GameplaySingleton>())
		Refresh(world);
} 