#pragma once

#include "ECS/System.h"

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
		using World = ecs::WorldView<
			gui::level_complete::WindowComponent,
			const ::hidden::ObjectComponent,
			const ::hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}