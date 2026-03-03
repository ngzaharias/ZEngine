#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct LinesSingleton;
	struct TransformComponent;
}

namespace tactics
{
	class AbilityTable;
	struct AbilityPreviewComponent;
	struct AbilityPreviewEvent;
	struct PawnAbilitiesComponent;
	struct PawnSelectedComponent;
}

namespace tilemap
{
	struct GridComponent;
}

namespace tactics
{
	class AbilityPreviewSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton,
			tactics::AbilityPreviewComponent>
			::Read<
			eng::TransformComponent,
			tactics::AbilityPreviewEvent,
			tactics::AbilityTable,
			tactics::PawnAbilitiesComponent,
			tactics::PawnSelectedComponent,
			tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}