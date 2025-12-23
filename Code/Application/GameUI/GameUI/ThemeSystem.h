#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::UIManager>
			::Read<
			eng::settings::GameplaySingleton,
			eng::TablesReloaded,
			eng::ThemeTable>;

		void Update(World& world, const GameTime& gameTime);
	};
}