#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	class ThemeTable;
	struct SpriteComponent;
	struct TablesReloadedEvent;
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
		using World = ecs::WorldView<
			// Resources
			const eng::ThemeTable,
			// Components
			eng::SpriteComponent,
			const eng::TablesReloadedEvent,
			const hidden::ObjectComponent,
			const hidden::RevealComponent,
			// Singletons
			const eng::settings::GameplaySingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}