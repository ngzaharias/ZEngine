#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::tactics
{
	struct AbilityPreviewComponent;
	struct AbilityPreviewEvent;
	struct PawnSelectedComponent;
}

namespace eng
{
	class InputManager;
	struct LinesComponent;
	struct TransformComponent;
}

namespace shared::tactics
{
	class AbilityTable;
	struct PawnAbilitiesTemplate;
}

namespace shared::tilemap
{
	struct GridComponent;
}

namespace client::tactics
{
	class AbilityPreviewSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::LinesComponent,
			client::tactics::AbilityPreviewComponent>
			::Read<
			client::tactics::AbilityPreviewEvent,
			client::tactics::PawnSelectedComponent,
			eng::TransformComponent,
			shared::tactics::AbilityTable,
			shared::tactics::PawnAbilitiesTemplate,
			shared::tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}