#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class ThemeTable;
	class UIManager;
	struct TablesReloaded;
}

namespace eng::settings
{
	struct GameplaySingleton;
}

namespace gui
{
	class ThemeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			const eng::ThemeTable,
			// Events
			const eng::TablesReloaded,
			// Singletons
			const eng::settings::GameplaySingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}