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
	struct AbilityComponent;
	struct PreviewComponent;
	struct PreviewEvent;
	struct SelectedComponent;
}

namespace tilemap
{
	struct GridComponent;
}

namespace tactics
{
	class PreviewSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton>
			::Read<
			eng::TransformComponent,
			tactics::AbilityComponent,
			tactics::AbilityTable,
			tactics::PreviewComponent,
			tactics::PreviewEvent,
			tactics::SelectedComponent,
			tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}