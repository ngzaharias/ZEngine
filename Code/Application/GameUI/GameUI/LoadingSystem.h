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

namespace gui::loading
{
	struct CloseRequest;
}

namespace gui::loading
{
	class LoadingSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			eng::level::LoadingComponent,
			const gui::loading::CloseRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}