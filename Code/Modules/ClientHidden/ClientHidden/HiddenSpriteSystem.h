#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace eng
{
	class ThemeTable;
	struct SpriteComponent;
	struct TablesReloaded;
}

namespace eng::settings
{
	struct GameplaySingleton;
}

namespace client::hidden
{
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent>
			::Read<
			client::hidden::ObjectComponent,
			client::hidden::RevealComponent,
			eng::settings::GameplaySingleton,
			eng::TablesReloaded,
			eng::ThemeTable>;

		void Update(World& world, const GameTime& gameTime);
	};
}