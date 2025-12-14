#pragma once

#include "ECS/System.h"

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
	class HintTable;
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
			const gui::HintTable,
			// Components
			eng::level::LoadingComponent,
			const gui::loading::CloseRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}