#pragma once

#include "ECS/System.h"

namespace gui::modal
{
	struct MessageComponent;
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
			gui::modal::MessageComponent,
			const ::hidden::ObjectComponent,
			const ::hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}