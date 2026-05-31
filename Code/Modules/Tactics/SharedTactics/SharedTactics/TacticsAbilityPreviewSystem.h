#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	struct LinesComponent;
	struct TransformComponent;
}

namespace shared::tactics
{
	class AbilityTable;
	struct AbilityPreviewComponent;
	struct AbilityPreviewEvent;
	struct PawnAbilitiesComponent;
	struct PawnSelectedComponent;
}

namespace shared::tilemap
{
	struct GridComponent;
}

namespace shared::tactics
{
	class AbilityPreviewSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::LinesComponent,
			shared::tactics::AbilityPreviewComponent>
			::Read<
			eng::TransformComponent,
			shared::tactics::AbilityPreviewEvent,
			shared::tactics::AbilityTable,
			shared::tactics::PawnAbilitiesComponent,
			shared::tactics::PawnSelectedComponent,
			shared::tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}