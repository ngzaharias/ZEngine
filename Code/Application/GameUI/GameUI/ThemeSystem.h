#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class ThemeTable;
	class UIManager;
	struct TablesReloadedEvent;
}

namespace eng::settings
{
	struct GameplayComponent;
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
			// Components
			const eng::settings::GameplayComponent,
			const eng::TablesReloadedEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}