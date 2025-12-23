#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
}

namespace hidden
{
	struct CountComponent;
}

namespace gui::hidden
{
	class CountSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::UIManager>
			::Read<
			::hidden::CountComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}