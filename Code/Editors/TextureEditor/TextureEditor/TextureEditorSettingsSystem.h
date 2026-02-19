#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::texture
{
	struct SettingsSingleton;
}

namespace gamestate
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
			editor::texture::SettingsSingleton>
			::Read<
			gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
