#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor
{
	struct InputWindowComponent;
	struct InputWindowRequest;
}

namespace eng
{
	class InputManager;
}

namespace editor
{

	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Managers
			eng::InputManager,
			// Components
			ecs::NameComponent,
			editor::InputWindowComponent,
			const editor::InputWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}