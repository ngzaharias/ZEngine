#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::inspector
{
	struct SettingsSingleton;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::inspector
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::inspector::SettingsSingleton,
			gamestate::EditorComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};