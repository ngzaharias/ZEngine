#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gui::level_complete
{
	struct WindowComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace gui::hidden
{
	class LevelSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			gui::level_complete::WindowComponent>
			::Read<
			::hidden::ObjectComponent,
			::hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}