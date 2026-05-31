#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gamestate
{
	struct EditorComponent;
}

namespace gamestate
{
	class EditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			gamestate::EditorComponent>;

		void Initialise(World& world);
	};
}
