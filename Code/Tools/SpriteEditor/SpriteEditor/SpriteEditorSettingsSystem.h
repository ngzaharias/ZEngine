#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::sprite
{
	struct SettingsSingleton;
}

namespace editor::sprite
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::sprite::SettingsSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
