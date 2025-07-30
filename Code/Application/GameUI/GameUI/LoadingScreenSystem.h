#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
}

namespace eng::level
{
	struct LoadingComponent;
}

namespace gui
{
	class LoadingScreenSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			const eng::level::LoadingComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}