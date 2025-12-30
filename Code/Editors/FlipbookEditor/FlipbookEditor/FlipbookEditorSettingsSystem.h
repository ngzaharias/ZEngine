#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::flipbook
{
	struct SettingsSingleton;
}

namespace editor::flipbook
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::flipbook::SettingsSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
