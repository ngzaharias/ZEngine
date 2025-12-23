#pragma once

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
		using World = ecs::WorldView
			::Write<
			eng::level::LoadingComponent,
			eng::UIManager>
			::Read<
			gui::HintTable,
			gui::loading::CloseRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}