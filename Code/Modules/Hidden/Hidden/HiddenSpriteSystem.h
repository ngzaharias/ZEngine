#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

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

namespace hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace hidden
{
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent>
			::Read<
			eng::settings::GameplaySingleton,
			eng::TablesReloaded,
			eng::ThemeTable,
			hidden::ObjectComponent,
			hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}