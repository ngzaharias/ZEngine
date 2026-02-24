#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AbilityTable;
	class UIManager;
}

namespace gui
{
	struct HUDComponent;
}

namespace gui
{
	class HUDSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::UIManager>
			::Read<
			eng::AbilityTable,
			gui::HUDComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}