#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
}

namespace render
{
	struct TranslucentComponent;
	struct UIPostComponent;
	struct UIPreComponent;
}

namespace render
{
	class UIPostSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::UIManager,
			render::UIPostComponent>
			::Read<
			render::TranslucentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}