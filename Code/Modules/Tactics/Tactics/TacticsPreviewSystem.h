#pragma once

#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
	struct TransformComponent;
}

namespace tactics
{
	class AbilityTable;
	struct AbilityComponent;
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
		using World = ecs::WorldView<
			// Resources
			const tactics::AbilityTable,
			// Components
			eng::LinesComponent,
			const eng::TransformComponent,
			const tactics::AbilityComponent,
			const tactics::SelectedComponent,
			const tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}