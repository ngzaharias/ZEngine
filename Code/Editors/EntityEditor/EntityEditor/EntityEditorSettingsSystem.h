#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::entity
{
	struct SettingsComponent;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::entity
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::entity::SettingsComponent,
			gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};