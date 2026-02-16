#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace settings
{
	struct EditorComponent;
	struct EditorToggleEvent;
}

namespace settings
{
	class EditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			settings::EditorComponent>
			::Read<
			settings::EditorToggleEvent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
