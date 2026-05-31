#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::gamestate
{
	struct EditorComponent;
}

namespace shared::gamestate
{
	class EditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::gamestate::EditorComponent>;

		void Initialise(World& world);
	};
}
