#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::flipbook
{
	struct SettingsComponent;
}

namespace shared::gamestate
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
			editor::flipbook::SettingsComponent>
			::Read<
			shared::gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
