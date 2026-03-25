#pragma once

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
		using World = ecs::WorldView
			::Write<
			eng::UIManager,
			eng::ThemeTable>
			::Read<
			eng::settings::GameplayComponent,
			eng::TablesReloadedEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}