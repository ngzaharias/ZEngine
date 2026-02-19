#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::flipbook
{
	struct SettingsSingleton;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::flipbook
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::flipbook::SettingsSingleton>
			::Read<
			gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
