#pragma once

#include <ECS/System.h>

namespace gui::modal
{
	struct MessageComponent;

	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<const gui::modal::MessageComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}