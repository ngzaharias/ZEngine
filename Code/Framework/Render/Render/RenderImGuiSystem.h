#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
}

namespace render
{
	struct UIPostComponent;
	struct ImGuiComponent;
}

namespace render
{
	class ImGuiSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			render::ImGuiComponent>
			::Read<
			render::UIPostComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}