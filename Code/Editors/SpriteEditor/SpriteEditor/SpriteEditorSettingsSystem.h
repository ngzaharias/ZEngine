#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::sprite
{
	struct SettingsStaticComponent;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::sprite
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::sprite::SettingsStaticComponent>
			::Read<
			gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
