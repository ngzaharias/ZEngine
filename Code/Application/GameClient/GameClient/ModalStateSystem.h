#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gui::modal
{
	struct MessageComponent;

	class StateSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Read<gui::modal::MessageComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}