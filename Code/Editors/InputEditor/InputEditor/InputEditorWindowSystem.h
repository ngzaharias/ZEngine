#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::input
{
	struct WindowComponent;
	struct WindowRequest;
}

namespace eng
{
	class InputManager;
}

namespace editor::input
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::input::WindowComponent,
			eng::InputManager>
			::Read<
			editor::input::WindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}