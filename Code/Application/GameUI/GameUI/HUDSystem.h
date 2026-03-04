#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class UIManager;
}

namespace gui
{
	struct HUDComponent;
}

namespace tactics
{
	class AbilityTable;
	struct PawnAbilitiesComponent;
	struct PawnSelectedComponent;
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
			ecs::NameComponent,
			gui::HUDComponent,
			tactics::AbilityTable,
			tactics::PawnAbilitiesComponent,
			tactics::PawnSelectedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}