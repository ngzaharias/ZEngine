#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::tactics
{
	struct PawnSelectedComponent;
}

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
	struct HUDTemplate;
}

namespace shared::tactics
{
	class AbilityTable;
	struct PawnAbilitiesTemplate;
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
			client::tactics::PawnSelectedComponent,
			ecs::NameComponent,
			gui::HUDTemplate,
			shared::tactics::AbilityTable,
			shared::tactics::PawnAbilitiesTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}