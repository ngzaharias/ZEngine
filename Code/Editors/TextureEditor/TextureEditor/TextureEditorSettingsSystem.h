#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::texture
{
	struct SettingsComponent;
}

namespace shared::gamestate
{
	struct EditorComponent;
}

namespace editor::texture
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::texture::SettingsComponent>
			::Read<
			shared::gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
